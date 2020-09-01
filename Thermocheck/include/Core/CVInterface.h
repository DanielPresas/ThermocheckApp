#ifndef CV_INTERFACE_H
#define CV_INTERFACE_H

#include "GUI/Texture2D.h"

#include <opencv2/opencv.hpp>

class CaptureDevice {
public:
	
	operator cv::VideoCapture() const { return _device; }

	bool init(int idx);
	void release();

	bool isInitialized() const { return _index >= 0; }
	int index()          const { return _index; }

	bool read(cv::OutputArray image);

private:
	
	int _index = -1;
	cv::VideoCapture _device;

};

struct CVState {
	uint32_t       numDevices    = 0;
	CaptureDevice* captureDevice = nullptr;
	cv::UMat       captureMat;
};

class CVInterface {
public:

	static void init();
	static void shutdown();

	static void update();
	static void drawImGui();

private:

	static void refreshDeviceList();

	static CVState* _state;
	static Texture2D* _captureImg;
	static bool _faceDetect;
	
};


#endif
