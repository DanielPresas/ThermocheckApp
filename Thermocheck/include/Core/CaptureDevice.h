#ifndef CAPTURE_DEVICE_H
#define CAPTURE_DEVICE_H

#include <libuvc/libuvc.h>
#include <opencv2/opencv.hpp>

class CaptureDevice {
public:

	bool init(int idx);
	void release();

	bool isInitialized() const { return false; }
	int index()          const { return 0; }

	bool read(cv::OutputArray image);

private:

	uvc_context_t* _context = nullptr;
	uvc_device_t* _device = nullptr;
	uvc_device_handle_t* _deviceHandle = nullptr;
	
	// @Temporary: Do we need this as a class member?
	uvc_stream_ctrl_t _streamControl{};
};


#endif