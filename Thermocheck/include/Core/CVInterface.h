#ifndef CV_INTERFACE_H
#define CV_INTERFACE_H

#include "GUI/Texture2D.h"

#include <opencv2/opencv.hpp>

class CaptureDevice {
public:
	
	operator cv::VideoCapture() const { return _device; }

	bool init(int idx);
	void release() { _device.release(); _index = -1; }
	
	bool isInitialized() const { return _index >= 0; }
	int index()          const { return _index; }

	bool read(cv::OutputArray image) { return _device.isOpened() ? _device.read(image) : false; }

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

	static void refreshDeviceList();

	static CaptureDevice _captureDevice;
	static Texture2D* _captureImg;
	static uint32_t _numDevices;
	
};

#endif
