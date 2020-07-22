#include "tcpch.h"
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
	Logger::init();

	struct CallbackData {
		cv::Mat canvas = cv::Mat::ones({ 500, 500 }, CV_8UC3);
		int radius = 3;
		cv::Scalar color = { 0.0, 255.0, 0.0, 255.0 };
	} callbackData;

	callbackData.canvas = cv::Scalar(255, 255, 255, 255);

	cv::namedWindow("Canvas");
	cv::setMouseCallback("Canvas", [](const int e, const int x, const int y, const int flags, void* userData) {
		CallbackData* params = static_cast<CallbackData*>(userData);

		switch(e) {
			case cv::EVENT_LBUTTONDOWN:
			{
				LOG_INFO("Left button pressed: ({0}, {1})", x, y);
				cv::circle(params->canvas, { x, y }, params->radius, params->color, cv::FILLED);
				break;
			}
			case cv::EVENT_MOUSEMOVE:
			{
				if(flags == cv::EVENT_FLAG_LBUTTON) {
					LOG_TRACE("Mouse moved while pressed: ({0}, {1})", x, y);
					cv::circle(params->canvas, { x, y }, params->radius, params->color, cv::FILLED);
				}
				break;
			}
			case cv::EVENT_LBUTTONUP:
			{
				LOG_INFO("Left button released: ({0}, {1})", x, y);
				break;
			}
			default: break;
		}
	}, &callbackData);

	// Draw loop
	while(true) {
		cv::imshow("Canvas", callbackData.canvas);

		const int ch = cv::waitKey(1);
		if(ch == 27 || !static_cast<bool>(cv::getWindowProperty("Canvas", cv::WindowPropertyFlags::WND_PROP_VISIBLE))) {
			break;
		}

		switch(static_cast<char>(ch)) {
			case 'b':
			{
				callbackData.color = { 255.0, 0.0, 0.0 };
				break;
			}
			case 'g':
			{
				callbackData.color = { 0.0, 255.0, 0.0 };
				break;
			}
			case 'r':
			{
				callbackData.color = { 0.0, 0.0, 255.0 };
				break;
			}
			default: break;
		}
	}

	cv::destroyAllWindows();

	Logger::shutdown();
	return 0;
}