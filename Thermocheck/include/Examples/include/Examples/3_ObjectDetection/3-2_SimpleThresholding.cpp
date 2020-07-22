#include "tcpch.h"
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
	Logger::init();

	cv::Mat bw = cv::imread("assets/detect_blob.png", cv::IMREAD_GRAYSCALE);
	cv::imshow("Original B&W", bw);

	cv::Mat binary = cv::Mat::zeros(bw.rows, bw.cols, CV_8U);
	const int threshold = 85;

	{
		Timer timer("Slow binary", true);

		for(int row = 0; row < bw.rows; ++row) {
			for(int col = 0; col < bw.cols; ++col) {
				if(bw.at<uchar>(row, col) > threshold) {
					binary.at<uchar>(row, col) = static_cast<uint8_t>(255);
				}
			}
		}
	}

	cv::imshow("Slow Binary", binary);

	{
		Timer timer("CV threshold func", true);
		cv::threshold(bw, binary, threshold, 255, cv::THRESH_BINARY);
	}

	cv::imshow("CV Threshold Func", binary);

	const int ch = cv::waitKey(0);
	if(ch == 27) {
		cv::destroyAllWindows();
	}

	Logger::shutdown();
	return 0;
}