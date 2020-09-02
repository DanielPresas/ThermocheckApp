#ifndef CV_INTERFACE_H
#define CV_INTERFACE_H

#include "GUI/Texture2D.h"

class CaptureDevice;

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
