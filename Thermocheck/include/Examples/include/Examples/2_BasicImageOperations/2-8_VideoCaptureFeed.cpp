#include "tcpch.h"
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
	Logger::init();

	auto cap = cv::VideoCapture(0);
	while(true) {

		cv::Mat frame;
		cap.read(frame);

		cv::resize(frame, frame, { 0, 0 }, 0.5f, 0.5f);
		cv::imshow("Frame", frame);

		const int ch = cv::waitKey(1);
		if(ch == 27 || !static_cast<bool>(cv::getWindowProperty("Frame", cv::WND_PROP_VISIBLE))) {
			break;
		}
	}

	cap.release();
	cv::destroyAllWindows();

	Logger::shutdown();
	return 0;
}