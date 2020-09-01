#include "tcpch.h"
#include "Core/CVInterface.h"

#include "Utils/Math.h"

#include <imgui/imgui.h>

CaptureDevice CVInterface::_captureDevice;
Texture2D* CVInterface::_captureImg = new Texture2D;
uint32_t CVInterface::_numDevices = 0;

void CVInterface::refreshDeviceList() {
	int cap = -1;
	if(_captureDevice.isInitialized()) {
		cap = _captureDevice.index();
		_captureDevice.release();
	}
	
	int idx = 0;
	while(true) {
		if(!_captureDevice.init(idx)) {
			break;
		}

		_captureDevice.release();
		++idx;
	}

	TC_LOG_INFO("Number of connected devices: {}", idx);
	_numDevices = idx;
	
	if(cap >= 0) {
		_captureDevice.init(cap);
	}
}

bool CaptureDevice::init(const int idx) {
	_device = cv::VideoCapture(idx);
	if(!_device.isOpened()) {
		return false;
	}
	
	_index = idx;
	return true;
}

#define HAAR_CASCADE 1

void CVInterface::init() {
	refreshDeviceList();
}

void CVInterface::shutdown() {
	_captureDevice.release();
}

void CVInterface::update() {
	
	using namespace cv;
	UMat frame;

	// Early bail if there is no initialized capture device
	// Send an empty frame so that the UI shows that there is no capture device.
	if(!_captureDevice.isInitialized()) {
		_captureImg->setData(frame);
		return;
	}
	
	TC_LOG_TRACE("Reading from capture device...");
	bool success = _captureDevice.read(frame) && !frame.empty();
	if(!success) {
		TC_LOG_ERROR("Failed to read frame from capture device {}!", _captureDevice.index() + 1);
		TC_LOG_ERROR("Releasing capture device {}...", _captureDevice.index() + 1);
		_captureDevice.release();
	}
	else {

#if HAAR_CASCADE

		UMat gray; cv::cvtColor(frame, gray, COLOR_BGR2GRAY);
		auto faceCascade = CascadeClassifier("assets/haarcascade_frontalface_default.xml");
		success = !faceCascade.empty();
		TC_ASSERT(success, "Failed to load cascade classifier!");

		std::vector<Rect> faces;
		{
#if TC_DEBUG || TC_RELEASE

			Timer timer("Haar Cascade", true);

#endif
			faceCascade.detectMultiScale(gray, faces, 1.1, 5, 0, { 40, 40 });
		}
		TC_LOG_TRACE("Number of faces detected: {}", faces.size());

		for(const auto& rect : faces) {
			cv::rectangle(frame, { rect.x, rect.y }, { rect.x + rect.width, rect.y + rect.height }, { 255 }, 4);
		}

#endif

	}

	_captureImg->setData(frame);
}

void CVInterface::drawImGui() {

	// ---------------------------------------------------
	// ----- VIDEO CAPTURE WINDOW ------------------------
	// ---------------------------------------------------
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		{
			const ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;
			ImGui::Begin("Video Capture", nullptr, flags);
			{
				const Vector2 windowSize = ImGui::GetWindowSize();
				
				if(_captureImg->empty()) {
					ImGui::SetWindowFontScale(3.0f);
					const Vector2 centre = (windowSize - ImGui::CalcTextSize("No video feed!")) * 0.5f;
					ImGui::SetCursorPos(centre);
					ImGui::TextWrapped("No video feed!");
					ImGui::SetWindowFontScale(1.0f);
				}
				else {
					constexpr float minWidth = 640;
					if(_captureImg->getWidth() < minWidth) {
						const double ratio = minWidth / _captureImg->getWidth();
						cv::UMat img = _captureImg->getUMat().clone();
						cv::resize(img, img, { 0, 0 }, ratio, ratio, cv::INTER_NEAREST);
						_captureImg->setData(img);
					}
					
					if(_captureImg->getWidth() > windowSize.x) {
						const double ratio = windowSize.x / _captureImg->getWidth();
						//
						// OpenGL doesn't like textures that aren't multiples of 4 because of GPU word length,
						// so we are ensuring that our width and height are always multiples of 4 by taking
						// advantage of integer division.
						//
						const int width    = static_cast<int>(static_cast<double>(windowSize.x)) / 4 * 4;
						const int height   = static_cast<int>(static_cast<double>(_captureImg->getHeight()) * ratio) / 4 * 4;

						cv::UMat img = _captureImg->getUMat().clone();
						cv::resize(img, img, { width, height });
						_captureImg->setData(img);
					}
					if(_captureImg->getHeight() > windowSize.y) {

						const double ratio = windowSize.y / _captureImg->getHeight();
						//
						// OpenGL doesn't like textures that aren't multiples of 4 because of GPU word length,
						// so we are ensuring that our width and height are always multiples of 4 by taking
						// advantage of integer division.
						//
						const int width    = static_cast<int>(static_cast<double>(_captureImg->getWidth()) * ratio) / 4 * 4;
						const int height   = static_cast<int>(static_cast<double>(windowSize.y)) / 4 * 4;

						cv::UMat img = _captureImg->getUMat().clone();
						cv::resize(img, img, { width, height });
						_captureImg->setData(img);
						
					}
					
					const Vector2 centre = (windowSize - _captureImg->getSize()) * 0.5f;
					ImGui::SetCursorPos(centre);
					ImGui::Image(reinterpret_cast<void*>(_captureImg->getRendererId()), _captureImg->getSize());
				}
			}
			ImGui::End();
		}
		ImGui::PopStyleVar();
	}

	// ---------------------------------------------------
	// ----- INTERFACE OPTIONS WINDOW --------------------
	// ---------------------------------------------------
	{
		ImGui::Begin("Options", nullptr);
		{
			std::string name;
			if(_captureDevice.index() >= 0) name = "Device " + std::to_string(_captureDevice.index() + 1);
			else                            name = "Select a device...";

			ImGui::PushItemWidth(ImGui::CalcTextSize("Select a device...").x * 1.5f);
			if(ImGui::BeginCombo("Active Capture Device", name.c_str())) {
				if(ImGui::Selectable("No device")) {
					if(_captureDevice.isInitialized()) _captureDevice.release();
				}
				
				for(uint32_t i = 0; i < _numDevices; ++i) {
					const bool selected = (_captureDevice.index() == static_cast<int>(i));
					name = "Device " + std::to_string(i + 1);
					if(ImGui::Selectable(name.c_str(), selected)) {
						if(!selected) {
							_captureDevice.release();
							_captureDevice.init(i);
						}
					}
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();

			ImGui::Text("Devices Connected: %u", _numDevices);
			ImGui::SameLine();
			if(ImGui::Button("Refresh Device List")) {
				refreshDeviceList();
			}
		}
		ImGui::End();
	}
}
