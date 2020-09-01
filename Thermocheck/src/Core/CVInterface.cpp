#include "tcpch.h"
#include "Core/CVInterface.h"


#include "Core/Application.h"
#include "Utils/Math.h"

#include <imgui/imgui.h>

CVState*   CVInterface::_state      = nullptr;
Texture2D* CVInterface::_captureImg = nullptr;
bool       CVInterface::_faceDetect = true;


// ---------------------------------------------------------------------
// ----- CAPTURE DEVICE ------------------------------------------------
// ---------------------------------------------------------------------


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

// ---------------------------------------------------------------------
// ----- CV INTERFACE --------------------------------------------------
// ---------------------------------------------------------------------


void CVInterface::init() {
	_state = new CVState;
	_state->captureDevice = new CaptureDevice;
	
	_captureImg = new Texture2D;
	
	refreshDeviceList();
	Application::pushCvUpdate(_state);
}

void CVInterface::shutdown() {
	delete _captureImg;
	
	_state->captureDevice->release();
	delete _state->captureDevice;
	delete _state;
}

void CVInterface::update() {
	
	using namespace cv;
	UMat frame;

	// Early bail if there is no initialized capture device
	// Send an empty frame so that the UI shows that there is no capture device.
	if(!_state->captureDevice->isInitialized()) {
		_state->captureMat = frame;
		Application::pushCvUpdate(_state);
		return;
	}

	bool success = _state->captureDevice->read(frame) && !frame.empty();
	if(!success) {
		TC_LOG_ERROR("Failed to read frame from capture device {}!", _state->captureDevice->index() + 1);
		TC_LOG_ERROR("Releasing capture device {}...", _state->captureDevice->index() + 1);
		_state->captureDevice->release();
	}
	else {

		constexpr int minWidth = 640;
		if(frame.cols < minWidth) {
			const double ratio = static_cast<double>(minWidth) / static_cast<double>(frame.cols);
			cv::resize(frame, frame, { 0, 0 }, ratio, ratio, cv::INTER_NEAREST);
		}

		UMat gray; cv::cvtColor(frame, gray, COLOR_BGR2GRAY);
		std::vector<Rect> faces;
		
		if(_faceDetect) {
			auto faceCascade = CascadeClassifier("assets/haarcascade_frontalface_default.xml");
			success = !faceCascade.empty();
			TC_ASSERT(success, "Failed to load cascade classifier!");

#if TC_DEBUG || TC_RELEASE
			{
				Timer timer("Haar Cascade", true);
#endif
				faceCascade.detectMultiScale(gray, faces, 1.1, 5, 0, { 40, 40 });
#if TC_DEBUG || TC_RELEASE
			}
			TC_LOG_TRACE("Number of faces detected: {}", faces.size());
#endif

			for(const auto& rect : faces) {
				cv::rectangle(frame, rect, { 255 }, 2);
			}
		}
		else {
			Vector2 start = { static_cast<float>(frame.size().width) * 0.25f, static_cast<float>(frame.size().height) * 0.25f };
			Vector2 end   = { static_cast<float>(frame.size().width) * 0.75f, static_cast<float>(frame.size().height) * 0.75f };

			cv::rectangle(frame, start, end, { 255, 255, 255 }, 2);
		}

	}
	
	_state->captureMat = frame;
	Application::pushCvUpdate(_state);
}

void CVInterface::drawImGui() {
	const CVState* state = Application::getCVState();
	_captureImg->setData(state->captureMat);
	
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
			if(state->captureDevice->index() >= 0) name = "Device " + std::to_string(state->captureDevice->index() + 1);
			else                            name = "Select a device...";

			ImGui::Columns(2, "##Row1", false);
			{
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Current Active Capture Device: ");
				// ImGui::SameLine(ImGui::CalcTextSize("Current Active Capture Device: ").x + 20.0f);
				
				ImGui::NextColumn();
				ImGui::SetNextItemWidth(-1);
				if(ImGui::BeginCombo("##Active Capture Device", name.c_str())) {
					{
						const bool released = !state->captureDevice->isInitialized();
						if(ImGui::Selectable("No device", released)) {
							if(!released) state->captureDevice->release();
						}
					}
				
					for(uint32_t i = 0; i < state->numDevices; ++i) {
						const bool selected = (state->captureDevice->index() == static_cast<int>(i));
						name = "Device " + std::to_string(i + 1);
						if(ImGui::Selectable(name.c_str(), selected)) {
							if(!selected) {
								state->captureDevice->release();
								state->captureDevice->init(i);
							}
						}
					}
					ImGui::EndCombo();
				}
			}
			ImGui::Columns(1);
			
			ImGui::Columns(2, "##Row2", false);
			{
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Devices Connected: %u", state->numDevices);
				// ImGui::SameLine(ImGui::CalcTextSize("Devices Connected: %u").x + 20.0f);
				
				ImGui::NextColumn();
				ImGui::SetNextItemWidth(-1);
				if(ImGui::Button("Refresh Device List")) {
					refreshDeviceList();
				}
			}
			ImGui::Columns(1);

			ImGui::NewLine();
			ImGui::Checkbox("Enable Face Detection", &_faceDetect);
		}
		ImGui::End();
	}
}

void CVInterface::refreshDeviceList() {
	int cap = -1;
	if(_state->captureDevice->isInitialized()) {
		cap = _state->captureDevice->index();
		_state->captureDevice->release();
	}

	int idx = 0;
	while(true) {
		if(!_state->captureDevice->init(idx)) {
			break;
		}

		_state->captureDevice->release();
		++idx;
	}

	TC_LOG_INFO("Number of connected devices: {}", idx);
	_state->numDevices = idx;

	if(cap >= 0) {
		_state->captureDevice->init(cap);
	}
}
