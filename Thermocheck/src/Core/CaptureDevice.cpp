#include "tcpch.h"
#include "Core/CaptureDevice.h"


#define FLIR_PT2_VID 0x1E4E
#define FLIR_PT2_PID 0x0100

uvc_context* CaptureDevice::_context = nullptr;
std::vector<uvc_device_descriptor_t*> CaptureDevice::_devices;

std::string enumToString(const uvc_frame_format e) {
	switch(e) {
		case UVC_FRAME_FORMAT_UNKNOWN:      return "UVC_FRAME_FORMAT_UNKNOWN";
		case UVC_FRAME_FORMAT_UNCOMPRESSED: return "UVC_FRAME_FORMAT_UNCOMPRESSED";
		case UVC_FRAME_FORMAT_COMPRESSED:   return "UVC_FRAME_FORMAT_COMPRESSED";
		case UVC_FRAME_FORMAT_YUYV:         return "UVC_FRAME_FORMAT_YUYV";
		case UVC_FRAME_FORMAT_UYVY:         return "UVC_FRAME_FORMAT_UYVY";
		case UVC_FRAME_FORMAT_RGB:          return "UVC_FRAME_FORMAT_RGB";
		case UVC_FRAME_FORMAT_BGR:          return "UVC_FRAME_FORMAT_BGR";
		case UVC_FRAME_FORMAT_MJPEG:        return "UVC_FRAME_FORMAT_MJPEG";
		case UVC_FRAME_FORMAT_GRAY8:        return "UVC_FRAME_FORMAT_GRAY8";
		case UVC_FRAME_FORMAT_GRAY16:       return "UVC_FRAME_FORMAT_GRAY16";
		case UVC_FRAME_FORMAT_BY8:          return "UVC_FRAME_FORMAT_BY8";
		case UVC_FRAME_FORMAT_BA81:         return "UVC_FRAME_FORMAT_BA81";
		case UVC_FRAME_FORMAT_SGRBG8:       return "UVC_FRAME_FORMAT_SGRBG8";
		case UVC_FRAME_FORMAT_SGBRG8:       return "UVC_FRAME_FORMAT_SGBRG8";
		case UVC_FRAME_FORMAT_SRGGB8:       return "UVC_FRAME_FORMAT_SRGGB8";
		case UVC_FRAME_FORMAT_SBGGR8:       return "UVC_FRAME_FORMAT_SBGGR8";
		case UVC_FRAME_FORMAT_COUNT:        return "UVC_FRAME_FORMAT_COUNT";
		default:                            return "Unknown uvc_frame_format type!";
	}
}

inline std::ostream& operator<<(std::ostream& out, const uvc_frame_format format) { return out << enumToString(format); }

bool CaptureDevice::init(const int idx) {
	initContext();

	uvc_device_descriptor_t* descriptor = _devices[idx];
	uvc_device_t* device;
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

	const DeviceInfo newDevice = { device, devHandle, descriptor };

	if(_deviceInfo == newDevice) {
		uvc_close(devHandle);
		uvc_unref_device(device);
		TC_LOG_DEBUG("Device {} {} already initialized", _deviceInfo.descriptor->manufacturer, _deviceInfo.descriptor->product);
		return false;
	}

	_deviceInfo = newDevice;

	const uvc_format_desc_t* formatDesc = uvc_get_format_descs(devHandle);
	_sensorSize = { formatDesc->frame_descs[0].wWidth, formatDesc->frame_descs[0].wHeight };
	
	setVideoFormat();

	_index = idx;
	return true;
}

void CaptureDevice::release() {

	TC_LOG_DEBUG("Freeing descriptor...");
	if(_deviceInfo.descriptor != nullptr) {
		uvc_free_device_descriptor(_deviceInfo.descriptor);
		_deviceInfo.descriptor = nullptr;
	}
	
	TC_LOG_DEBUG("Closing device...");
	if(_deviceInfo.deviceHandle != nullptr) {
		uvc_close(_deviceInfo.deviceHandle);
		_deviceInfo.deviceHandle = nullptr;
	}
	TC_LOG_DEBUG("Releasing device...");
	if(_deviceInfo.device != nullptr) {
		uvc_unref_device(_deviceInfo.device);
		_deviceInfo.device = nullptr;
	}

}

bool CaptureDevice::read(cv::OutputArray image) {
	
	struct CallbackData {
		cv::Mat out;
	} cbData;

	TC_LOG_INFO("Using {} {} (SN: {})", _deviceInfo.descriptor->manufacturer, _deviceInfo.descriptor->product, _deviceInfo.descriptor->serialNumber);
	const uvc_error_t success = uvc_start_streaming(_deviceInfo.deviceHandle, &_streamInfo.mode, [](uvc_frame_t* frame, void* userPtr) {
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
	uvc_set_ae_mode(_deviceInfo.deviceHandle, 1);
	TC_LOG_DEBUG("UVC stream started");
	
	std::this_thread::sleep_for(std::chrono::seconds(1));
	uvc_stop_streaming(_deviceInfo.deviceHandle);
	TC_LOG_DEBUG("UVC stream stopped");

	image.assign(cbData.out);
	return true;
}

void CaptureDevice::setVideoFormat() {
	uvc_frame_format format;

	const std::string serialNum = _deviceInfo.descriptor->serialNumber;
	const std::string product   = _deviceInfo.descriptor->product;

	const bool rtAGCChange = !product.find("fw:v0");                                            // @Note: Supports runtime auto gain control
	const bool y16Firmware = product.find("Y16");                                               // @Temporary: @Note: Has Y16 firmware;
	const bool radiometric = serialNum.find("500-0763-01") || serialNum.find("500-0771-01");    // @Temporary: @Note: Is radiometric model
	
	const bool hwPseudoColor =  rtAGCChange || !y16Firmware;                    // @Note: Supports hardware pseudo-coloring
	const bool radiometry    = (rtAGCChange ||  y16Firmware) && radiometric;    // @Note: Supports radiometry

	if(!hwPseudoColor || radiometry) format = UVC_FRAME_FORMAT_GRAY16;
	else                             format = UVC_FRAME_FORMAT_RGB;

	_streamInfo.format = format;
	const uvc_error_t success = uvc_get_stream_ctrl_format_size(_deviceInfo.deviceHandle, &_streamInfo.mode, format, (int)_sensorSize.x, (int)_sensorSize.y, 0);
	if(success < 0) {
		const char* error = uvc_strerror(success);
		TC_LOG_ERROR("[uvc_get_stream_ctrl_format_size] Failed to get stream control format size: {}", error);
	}
	
	TC_LOG_DEBUG("UVC stream mode received:");
	TC_LOG_DEBUG("    format:\t\t\t"                     "{}", format);
	TC_LOG_DEBUG("    bmHint:\t\t\t"               "0x{:04x}", _streamInfo.mode.bmHint);
	TC_LOG_DEBUG("    bFormatIndex:\t\t"                 "{}", _streamInfo.mode.bFormatIndex);
	TC_LOG_DEBUG("    bFrameIndex:\t\t\t"                "{}", _streamInfo.mode.bFrameIndex);
	TC_LOG_DEBUG("    dwFrameInterval:\t\t"              "{}", _streamInfo.mode.dwFrameInterval);
	TC_LOG_DEBUG("    wKeyFrameRate:\t\t"                "{}", _streamInfo.mode.wKeyFrameRate);
	TC_LOG_DEBUG("    wPFrameRate:\t\t\t"                "{}", _streamInfo.mode.wPFrameRate);
	TC_LOG_DEBUG("    wCompQuality:\t\t"                 "{}", _streamInfo.mode.wCompQuality);
	TC_LOG_DEBUG("    wCompWindowSize:\t\t"              "{}", _streamInfo.mode.wCompWindowSize);
	TC_LOG_DEBUG("    wDelay:\t\t\t"                     "{}", _streamInfo.mode.wDelay);
	TC_LOG_DEBUG("    dwMaxVideoFrameSize:\t\t"  "{} ({}x{})", _streamInfo.mode.dwMaxVideoFrameSize, _sensorSize.x, _sensorSize.y);
	TC_LOG_DEBUG("    dwMaxPayloadTransferSize:\t"       "{}", _streamInfo.mode.dwMaxPayloadTransferSize);
	TC_LOG_DEBUG("    bInterfaceNumber:\t\t"             "{}", _streamInfo.mode.bInterfaceNumber);
}

int CaptureDevice::getAllConnectedDevices() {
	initContext();
	
	uvc_device_t** devices;
	const uvc_error_t success = uvc_get_device_list(_context, &devices);
	if(success < 0) {
		const char* error = uvc_strerror(success);
		TC_LOG_ERROR("[uvc_get_device_list] Failed to get UVC device list: {}", error);
	}

	TC_LOG_DEBUG("UVC Device list:");
	TC_LOG_DEBUG("-------------------------------------");
	uvc_device_t* device;
	int deviceIdx = -1;
	while((device = devices[++deviceIdx]) != nullptr) {
		uvc_device_descriptor_t* desc;
		uvc_get_device_descriptor(device, &desc);
		_devices.push_back(desc);

		TC_LOG_DEBUG("Device {}:", deviceIdx + 1);
		TC_LOG_DEBUG("    Product:\t"          "{} {}", desc->manufacturer, desc->product);
		TC_LOG_DEBUG("    Serial:\t"              "{}", desc->serialNumber);
		TC_LOG_DEBUG("    VID:\t\t"         "0x{:04x}", desc->idVendor);
		TC_LOG_DEBUG("    PID:\t\t"         "0x{:04x}", desc->idProduct);
		TC_LOG_DEBUG("    Compliance:\t"    "0x{:04x}", desc->bcdUVC);
	}

	TC_LOG_DEBUG("-------------------------------------");
	TC_LOG_INFO("Number of devices connected: {}", deviceIdx);
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
