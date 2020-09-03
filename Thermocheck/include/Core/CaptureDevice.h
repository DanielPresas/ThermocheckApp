#ifndef CAPTURE_DEVICE_H
#define CAPTURE_DEVICE_H

#include "Utils/Math.h"

#include <libuvc/libuvc.h>
#include <opencv2/opencv.hpp>

class CaptureDevice {
public:

	CaptureDevice() = default;
	CaptureDevice(const int idx) { init(idx); }
	~CaptureDevice() noexcept    { release(); }

	bool init(int idx);
	void release();

	bool isInitialized() const { return _context != nullptr && _deviceInfo.isInitialized(); }
	int index()          const { return _index; }

	bool read(cv::OutputArray image);


	static int getAllConnectedDevices();

private:

	static void initContext();
	void setVideoFormat();
	
	struct DeviceInfo {
		uvc_device_t*            device        = nullptr;
		uvc_device_handle_t*     deviceHandle  = nullptr;
		uvc_device_descriptor_t* descriptor    = nullptr;
		
		bool operator==(const DeviceInfo& other) const {
			if(descriptor == nullptr)       return false;
			if(other.descriptor == nullptr) return false;
			
			const bool sameVendor       = descriptor->idVendor  == other.descriptor->idVendor;
			const bool sameProduct      = descriptor->idProduct == other.descriptor->idProduct;
			const bool sameManufacturer = strcmp(descriptor->manufacturer, other.descriptor->manufacturer) == 0;
			const bool sameSerialNumber = strcmp(descriptor->serialNumber, other.descriptor->serialNumber) == 0;

			return sameVendor && sameProduct && sameManufacturer && sameSerialNumber;
		}

		bool isInitialized() const {
			return device    != nullptr &&
				deviceHandle != nullptr &&
				descriptor   != nullptr;
		}
		
	} _deviceInfo;

	struct StreamInfo {
		
		uvc_stream_ctrl_t mode;
		uvc_frame_format  format = UVC_FRAME_FORMAT_UNKNOWN;
		
	} _streamInfo;

	uint32_t _index = -1;
	Vector2 _sensorSize;

	static uvc_context_t* _context;
	static std::vector<uvc_device_descriptor_t*> _devices;

	
};


#endif
