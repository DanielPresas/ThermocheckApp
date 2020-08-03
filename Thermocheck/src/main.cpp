#include "tcpch.h"
#include <opencv2/opencv.hpp>

#define EXAMPLE 0

#if EXAMPLE

#include <Examples/4_FaceAndFeatureDetection/4-5_HaarCascadingFaceDetection.cpp>

#else

int main(int argc, char** argv) {

#define HSV_SPLIT_DETECT 0
#define HAAR_CASCADE 1

	Logger::init();
	using namespace cv;
	
	int captureDevice = 0;
	auto cam = VideoCapture(captureDevice);
	cv::namedWindow("Capture");

	Timer timer("Face cascade real time");
	
	while(true) {
		UMat frame;
		Logger::logAssert(cam.read(frame), "Failed to read frame from capture device {}!", captureDevice);
		cv::imshow("Capture", frame);
		
		UMat frameCopy = frame.clone();

#if HSV_SPLIT_DETECT
		
		UMat hsv; cv::cvtColor(frame, hsv, COLOR_BGR2HSV);
		std::array<Mat, 3> hsvChannels; cv::split(hsv, hsvChannels);

		UMat minSat, maxHue;
		UMat finalMat;
		cv::threshold(hsvChannels[0], maxHue, 15, 255, THRESH_BINARY_INV);
		cv::threshold(hsvChannels[1], minSat, 40, 255, THRESH_BINARY);
		cv::bitwise_and(minSat, maxHue, finalMat);
		cv::imshow("HSV Split", finalMat);

		std::vector<Mat> contours; cv::findContours(finalMat, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
		
		for(uint32_t i = 0; i < contours.size(); ++i) {
			if(cv::contourArea(contours.at(i)) < 1000.0) continue;
			cv::drawContours(frameCopy, contours, i, { 255, 255, 255 }, 2);
		}
		cv::imshow("Contour Skin Detect", frameCopy);
		
#elif HAAR_CASCADE

		UMat gray; cv::cvtColor(frame, gray, COLOR_BGR2GRAY);
		auto faceCascade = CascadeClassifier("assets/haarcascade_frontalface_default.xml");
		Logger::logAssert(!faceCascade.empty(), "Failed to load cascade classifier!");

		std::vector<Rect> faces;
		{
			timer.start();
			faceCascade.detectMultiScale(gray, faces, 1.1, 5, 0, { 40, 40 });
			timer.stop();
		}
		Logger::trace("Number of faces detected: {}", faces.size());

		for(const auto& rect : faces) {
			cv::rectangle(frameCopy, { rect.x, rect.y }, { rect.x + rect.width, rect.y + rect.height }, { 255 }, 4);
		}

		cv::imshow("Haar Cascade", frameCopy);
		
#endif

		const auto ch = cv::waitKey(1);
		if(ch == 27 || !static_cast<bool>(cv::getWindowProperty("Capture", WND_PROP_VISIBLE))) {
			break;
		}
	}
	
	cam.release();
	cv::destroyAllWindows();
	
	Logger::shutdown();
	
	return 0;
}

#endif
