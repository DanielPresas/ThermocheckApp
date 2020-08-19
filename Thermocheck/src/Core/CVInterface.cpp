#include "tcpch.h"
#include "Core/CVInterface.h"

#include <imgui.h>

CaptureDevice CVInterface::_captureDevice;
Texture2D* CVInterface::_framebufferImage = new Texture2D;

#define HSV_SPLIT_DETECT 0
#define HAAR_CASCADE 1

void CVInterface::init() {
	_captureDevice.init(1);
}

void CVInterface::shutdown() {
	_captureDevice.release();
	cv::destroyAllWindows();
}

void CVInterface::update() {
	
	using namespace cv;
	
	UMat frame;
	Logger::logAssert(_captureDevice.read(frame) && !frame.empty(), "Failed to read frame from capture device {}!", _captureDevice.index);

	
	// cv::resize(frame, frame, { 160, 120 }, 1.0, 1.0);

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
		cv::drawContours(frame, contours, i, { 255, 255, 255 }, 2);
	}
	cv::imshow("Contour Skin Detect", frame);

#elif HAAR_CASCADE

	UMat gray; cv::cvtColor(frame, gray, COLOR_BGR2GRAY);
	auto faceCascade = CascadeClassifier("assets/haarcascade_frontalface_default.xml");
	Logger::logAssert(!faceCascade.empty(), "Failed to load cascade classifier!");

	std::vector<Rect> faces;
	{
		Timer timer("Haar Cascade", true);
		faceCascade.detectMultiScale(gray, faces, 1.1, 5, 0, { 40, 40 });
	}
	Logger::trace("Number of faces detected: {}", faces.size());

	for(const auto& rect : faces) {
		cv::rectangle(frame, { rect.x, rect.y }, { rect.x + rect.width, rect.y + rect.height }, { 255 }, 4);
	}

	//cv::resize(frame, frame, { 640, 480 }, 1.0, 1.0);
	//cv::imshow("Haar Cascade", frame);

#endif

	_framebufferImage->setData(frame);


	//const auto ch = cv::waitKey(1);
	//if(ch == 27 || !static_cast<bool>(cv::getWindowProperty("Haar Cascade", WND_PROP_VISIBLE))) {
	//	
	//}
}

void CVInterface::drawImGui() {
	
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
	ImGui::SetNextWindowSize(_framebufferImage->getSize());
	ImGui::Begin("Video Capture");
	{
		ImGui::GetContentRegionAvail();
		ImGui::Image(reinterpret_cast<void*>(_framebufferImage->getRendererId()), _framebufferImage->getSize());
	}
	ImGui::End();
	ImGui::PopStyleVar();
}
