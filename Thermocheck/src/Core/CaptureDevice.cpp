#include "tcpch.h"
#include "Core/CaptureDevice.h"


#define FLIR_PT2_VID 0x1E4E
#define FLIR_PT2_PID 0x0100

uvc_context* CaptureDevice::_context = nullptr;
std::vector<uvc_device_descriptor_t*> CaptureDevice::_devices;

bool CaptureDevice::init(const int idx) {
	initContext();

	uvc_device_descriptor_t* descriptor = _devices[idx];
	uvc_device_t* device;
	// success = uvc_find_device(_info.context, &_info.device, FLIR_PT2_VID, FLIR_PT2_PID, nullptr);
	uvc_error_t success = uvc_find_device(_context, &device, descriptor->idVendor, descriptor->idProduct, nullptr);
	if(success < 0) {
		uvc_unref_device(device);
		
		const char* error = uvc_strerror(success);
		TC_LOG_ERROR("[uvc_find_device] Failed to find UVC device: {}", error);
		return false;
	}
	TC_LOG_DEBUG("UVC device found");

	uvc_device_handle_t* devHandle;
	success = uvc_open(device, &devHandle);
	if(success < 0) {
		uvc_close(devHandle);
		uvc_unref_device(device);
		
		const char* error = uvc_strerror(success);
		TC_LOG_ERROR("[uvc_open] Failed to open UVC device: {}", error);
		return false;
	}
	TC_LOG_DEBUG("UVC device opened");
	uvc_print_diag(devHandle, stdout);

	TC_LOG_INFO("[uvc_get_device_descriptor] Device vendor: {}", descriptor->idVendor);
	TC_LOG_INFO("Using {} {} with firmware {}", descriptor->manufacturer, descriptor->product, descriptor->serialNumber);

	const UVCInfo newDevice = { device, devHandle, descriptor };

	if(_info == newDevice) {
		uvc_close(devHandle);
		uvc_unref_device(device);
		TC_LOG_DEBUG("Device {} {} already initialized", _info.descriptor->manufacturer, _info.descriptor->product);
		return false;
	}

	_info = newDevice;

	const uvc_format_desc_t* formatDesc = uvc_get_format_descs(devHandle);
	_sensorSize = { formatDesc->frame_descs[0].wWidth, formatDesc->frame_descs[0].wHeight };

	double fps = 1.0 / (double)formatDesc->frame_descs[0].dwMaxFrameInterval;
	success = uvc_get_stream_ctrl_format_size(_info.deviceHandle, &_info.streamControl, UVC_FRAME_FORMAT_GRAY16, (int)_sensorSize.x, (int)_sensorSize.y, (int)fps);
	uvc_print_stream_ctrl(&_info.streamControl, stderr);
	if(success < 0) {
		uvc_close(devHandle);
		uvc_unref_device(device);
		
		const char* error = uvc_strerror(success);
		TC_LOG_ERROR("[uvc_get_stream_ctrl_format_size] Failed to get stream control format size: {}", error);
		return false;
	}
	TC_LOG_DEBUG("UVC stream control format size received");

	return true;
}

void CaptureDevice::release() {

	TC_LOG_DEBUG("Freeing descriptor...");
	if(_info.descriptor != nullptr) {
		uvc_free_device_descriptor(_info.descriptor);
		_info.descriptor = nullptr;
	}
	
	TC_LOG_DEBUG("Closing device...");
	if(_info.deviceHandle != nullptr) {
		uvc_close(_info.deviceHandle);
		_info.deviceHandle = nullptr;
	}
	TC_LOG_DEBUG("Releasing device...");
	if(_info.device != nullptr) {
		uvc_unref_device(_info.device);
		_info.device = nullptr;
	}

}

bool CaptureDevice::read(cv::OutputArray image) {
	
	struct CallbackData {
		cv::Mat out;
	} cbData;
	
	const uvc_error_t success = uvc_start_streaming(_info.deviceHandle, &_info.streamControl, [](uvc_frame_t* frame, void* userPtr) {
		uvc_frame_t* bgr = uvc_allocate_frame(frame->width * frame->height * 3);
		if(!bgr) {
			TC_LOG_ERROR("[uvc_allocate_frame] Unable to allocate BGR frame!");
			return;
		}

		const uvc_error_t ret = uvc_any2bgr(frame, bgr);
		if(ret) {
			uvc_free_frame(bgr);
			
			const char* error = uvc_strerror(ret);
			TC_LOG_ERROR("[uvc_any2bgr] Unable to convert format to BGR: {}", error);
			return;
		}
		
		auto* data = static_cast<CallbackData*>(userPtr);
		data->out = cv::Mat(bgr->height, bgr->width, CV_8UC3, bgr->data);
		uvc_free_frame(bgr);
		
	}, &cbData, 0);

	if(success < 0) {
		const char* error = uvc_strerror(success);
		TC_LOG_ERROR("[uvc_start_streaming] Failed to start stream: {}", error);
		return false;
	}
	uvc_set_ae_mode(_info.deviceHandle, 1);
	TC_LOG_DEBUG("UVC stream started");
	
	std::this_thread::sleep_for(std::chrono::seconds(1));
	uvc_stop_streaming(_info.deviceHandle);
	TC_LOG_DEBUG("UVC stream stopped");

	image.assign(cbData.out);
	return true;
}

int CaptureDevice::listAllDevices() {
	initContext();
	
	uvc_device_t** devices;
	const uvc_error_t success = uvc_get_device_list(_context, &devices);
	if(success < 0) {
		const char* error = uvc_strerror(success);
		TC_LOG_ERROR("[uvc_get_device_list] Failed to get UVC device list: {}", error);
	}

	TC_LOG_DEBUG("UVC Device list:");
	uvc_device_t* device;
	int deviceIdx = -1;
	while((device = devices[++deviceIdx]) != nullptr) {
		uvc_device_descriptor_t* desc;
		uvc_get_device_descriptor(device, &desc);
		_devices.push_back(desc);

		TC_LOG_DEBUG("---------------------------");
		TC_LOG_DEBUG("Device {}:", deviceIdx + 1);
		TC_LOG_DEBUG("    Product:\t{} {}", desc->manufacturer, desc->product);
		TC_LOG_DEBUG("    Serial:\t{}", desc->serialNumber);
		TC_LOG_DEBUG("    VID:\t0x{:x}", desc->idVendor);
		TC_LOG_DEBUG("    PID:\t0x{:x}", desc->idProduct);
	}
	
	TC_LOG_DEBUG("Number of devices connected: {}", deviceIdx);
	return deviceIdx;
}

void CaptureDevice::initContext() {
	if(_context != nullptr) return;

	uvc_context_t* context;
	const uvc_error_t success = uvc_init(&context, nullptr);

	if(success < 0) {
		uvc_exit(context);

		const char* error = uvc_strerror(success);
		TC_LOG_ERROR("[uvc_init] Failed to initialize UVC context! {}", error);
		return;
	}
	
	_context = context;
	TC_LOG_DEBUG("UVC context initialized");
}
