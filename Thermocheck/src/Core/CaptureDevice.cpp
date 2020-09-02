#include "tcpch.h"
#include "Core/CaptureDevice.h"

bool CaptureDevice::init(const int idx) {
	_device = cv::VideoCapture(idx);
	if(!_device.isOpened()) {
		return false;
	}

	_index = idx;
	return true;
}

void CaptureDevice::release() {
	_device.release();
	_index = -1;
}

bool CaptureDevice::read(cv::OutputArray image) {
	return _device.isOpened() ? _device.read(image) : false;
}