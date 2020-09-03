#include "tcpch.h"
#include "Core/CaptureDevice.h"

#define FLIR_PT2_VID 0x1E4E
#define FLIR_PT2_PID 0x0100

bool CaptureDevice::init(const int idx) {
	uvc_error_t success = uvc_init(&_context, nullptr);

	if(success < 0) {
		const char* error = uvc_strerror(success);
		return TC_ASSERT(false, "[uvc_init] Failed to initialize UVC context! {}", error);
	}
	TC_LOG_DEBUG("UVC context initialized");

	// @Hack: How to get index? Reference pattern?
	// success = uvc_find_device(_context, &_device, FLIR_PT2_VID, FLIR_PT2_PID, nullptr);
	success = uvc_find_device(_context, &_device, 0, 0, nullptr);
	if(success < 0) {
		const char* error = uvc_strerror(success);
		return TC_ASSERT(false, "[uvc_find_device] Failed to find UVC device: {}", error);
	}
	TC_LOG_DEBUG("UVC device found");

	success = uvc_open(_device, &_deviceHandle);
	if(success < 0) {
		const char* error = uvc_strerror(success);
		uvc_unref_device(_device);
		_device = nullptr;
		return TC_ASSERT(false, "[uvc_open] Failed to open UVC device: {}", error);
	}
	TC_LOG_DEBUG("UVC device opened");
	uvc_print_diag(_deviceHandle, stdout);

	uvc_device_descriptor_t* desc;
	uvc_get_device_descriptor(_device, &desc);
	TC_LOG_INFO("[uvc_get_device_descriptor] Device vendor: {}", desc->idVendor);

	TC_LOG_DEBUG("Initializing Lepton SDK with UVC backend...");
	TC_LOG_DEBUG("Using {} {} with firmware {}", desc->manufacturer, desc->product, desc->serialNumber);

	uvc_free_device_descriptor(desc);
	
	
	success = uvc_get_stream_ctrl_format_size(_deviceHandle, &_streamControl, UVC_FRAME_FORMAT_GRAY16, 80, 60, 9);
	uvc_print_stream_ctrl(&_streamControl, stderr);
	if(success < 0) {
		const char* error = uvc_strerror(success);
		return TC_ASSERT(false, "[uvc_get_stream_ctrl_format_size] Failed to get stream control format size: {}", error);
	}
	TC_LOG_DEBUG("UVC stream control format size received");

	return true;
}

void CaptureDevice::release() {
	TC_LOG_DEBUG("Closing device...");
    uvc_close(_deviceHandle);
	TC_LOG_DEBUG("Releasing device...");
	uvc_unref_device(_device);
	TC_LOG_DEBUG("Closing context...");
	uvc_exit(_context);
}

bool CaptureDevice::read(cv::OutputArray image) {
	
	struct CallbackData {
		cv::Mat out;
	} cbData;
	
	uvc_error_t success = uvc_start_streaming(_deviceHandle, &_streamControl, [](uvc_frame_t* frame, void* userPtr) {
		uvc_frame_t* bgr = uvc_allocate_frame(frame->width * frame->height * 3);
		if(!bgr) {
			printf("unable to allocate bgr frame!");
			return;
		}

		uvc_error_t ret = uvc_any2bgr(frame, bgr);
		if(ret) {
			uvc_perror(ret, "uvc_any2bgr");
			uvc_free_frame(bgr);
			return;
		}
		
		auto* data = static_cast<CallbackData*>(userPtr);
		data->out = cv::Mat(bgr->height, bgr->width, CV_8UC3, bgr->data);
		uvc_free_frame(bgr);
		
	}, &cbData, 0);

	if(success < 0) {
		uvc_perror(success, "uvc_start_streaming");
		return TC_ASSERT(false, "Failed to start stream!");
	}
	uvc_set_ae_mode(_deviceHandle, 1);
	TC_LOG_DEBUG("UVC stream started");
	
	std::this_thread::sleep_for(std::chrono::seconds(1));
	uvc_stop_streaming(_deviceHandle);
	TC_LOG_DEBUG("UVC stream stopped");

	image.assign(cbData.out);
	return true;
}
