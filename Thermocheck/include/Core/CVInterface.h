#ifndef CV_INTERFACE_H
#define CV_INTERFACE_H

#include "GUI/Texture2D.h"

#include <opencv2/opencv.hpp>

class CaptureDevice {
public:
	
	operator cv::VideoCapture() const { return _device; }

	void init(const int idx) {
		_device = cv::VideoCapture(idx);
		Logger::logAssert(_device.isOpened(), "Failed to open capture device {}!", idx);
		_idx = idx;
	}

	int index() const { return _idx; }
	bool read(cv::OutputArray image) { return _device.read(image); }
	void release() { _device.release(); }

private:
	
	int _idx = 0;
	cv::VideoCapture _device;

};

class CVInterface {
public:

	static void init();
	static void shutdown();

	static void update();
	static void drawImGui();

	//void onEvent();

private:

	static CaptureDevice _captureDevice;
	static Texture2D* _captureImg;
	
};

#endif
