#ifndef CV_INTERFACE_H
#define CV_INTERFACE_H

#include "GUI/Texture2D.h"

#include <opencv2/opencv.hpp>

class CaptureDevice {
public:
	
	operator cv::VideoCapture() const { return _device; }

	bool init(int idx);

	int index() const { return _index; }
	bool read(cv::OutputArray image) { return _device.isOpened() ? _device.read(image) : false; }
	void release() { _device.release(); _index = -1; }

private:
	
	int _index = -1;
	cv::VideoCapture _device;

};

class CVInterface {
public:

	static void init();
	static void shutdown();

	static void update();
	static void drawImGui();

private:

	static CaptureDevice _captureDevice;
	static Texture2D* _captureImg;
	static inline uint32_t _numDevices = 0;
	
};

#endif
