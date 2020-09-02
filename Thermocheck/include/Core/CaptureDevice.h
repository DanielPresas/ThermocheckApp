#ifndef CAPTURE_DEVICE_H
#define CAPTURE_DEVICE_H

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


#endif