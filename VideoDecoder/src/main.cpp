#include "vdpch.h"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#define CH3_3 1

int main(int argc, char** argv) {
	Logger::init();
	using namespace cv;

#if CH2_3
	Mat black = Mat(150, 200, CV_8UC1, Scalar::all(0));
	cv::imshow("Black", black);
	LOG_TRACE("{}", black.at<uchar>(Point(0, 0)));

	Mat ones = Mat(150, 200, CV_8UC3, Scalar::all(1));
	cv::imshow("Ones", ones);
	LOG_TRACE("{}", ones.at<Vec3b>(Point(0, 0)));

	Mat white = Mat(150, 200, CV_16UC3, Scalar::all(std::pow(2, 16) - 1));
	cv::imshow("White", white);
	LOG_TRACE("{}", white.at<Vec3w>(Point(0, 0)));

	Mat color = ones.clone();
	color = Scalar(255, 0, 0, 0);
	imshow("Blue", color);
	LOG_TRACE("{}", color.at<Vec3b>(Point(0, 0)));

	cv::waitKey(0);
	cv::destroyAllWindows();
	
#endif

#if CH2_4

	Mat color = imread("assets/starry_night.png", IMREAD_COLOR);
	cv::imshow("Image", color);
	cv::moveWindow("Image", 0, 0);

	auto shape = Vec3i(color.rows, color.cols, color.channels());
	LOG_TRACE("Shape: {}", shape);

	Mat bgrChannels[3];
	cv::split(color, bgrChannels);

	Mat rgbSplit = Mat(color.rows * 3, color.cols, CV_8UC3);
	Mat blueMerge[3] = { bgrChannels[0], bgrChannels[0], bgrChannels[0] };
	Mat greenMerge[3] = { bgrChannels[1], bgrChannels[1], bgrChannels[1] };
	Mat redMerge[3] = { bgrChannels[2], bgrChannels[2], bgrChannels[2] };
	
	Mat b, g, r;
	cv::merge(blueMerge, 3, b);
	cv::merge(greenMerge, 3, g);
	cv::merge(redMerge, 3, r);

	Mat last[3] = { b, g, r };
	cv::hconcat(last, 3, rgbSplit);	

	cv::imshow("Channels", rgbSplit);
	cv::moveWindow("Channels", 0, color.rows);

	Mat hsv, hsvChannels[3], hsvSplit;

	cv::cvtColor(color, hsv, COLOR_BGR2HSV);
	cv::split(hsv, hsvChannels);
	cv::hconcat(hsvChannels, 3, hsvSplit);

	cv::imshow("Split HSV", hsvSplit);

	cv::waitKey(0);
	cv::destroyAllWindows();
	
#endif

#if CH2_5
	Mat color = cv::imread("assets/starry_night.png", cv::IMREAD_COLOR);
	Mat gray;
	cv::cvtColor(color, gray, cv::COLOR_RGB2GRAY);
	cv::imwrite("outputs/gray.jpg", gray);

	Mat channels[3];
	cv::split(color, channels);
	
	Mat rgba, rgbaChannels[4] = { channels[0], channels[1], channels[2], channels[1] };
	cv::merge(rgbaChannels, 4, rgba);
	cv::imwrite("outputs/rgba.png", rgba);
	
#endif

#if CH2_6

	Mat image = cv::imread("assets/starry_night.png");
	LOG_ASSERT(!image.empty(), "Could not load image!");
	cv::imshow("Original", image);

	Mat blur;
	cv::GaussianBlur(image, blur, { 5, 55 }, 0);
	cv::imshow("Blur", blur);

	Mat kernel = Mat::ones({ 5, 5 }, CV_8U);
	Mat dilate, erode;
	cv::dilate(image, dilate, kernel, { -1, -1 }, 1);
	cv::erode(image, erode, kernel, { -1, -1 }, 1);

	cv::imshow("Dilate", dilate);
	cv::imshow("Erode", erode);

	cv::waitKey(0);
	cv::destroyAllWindows();
	
#endif

#if CH2_7

	Mat image = cv::imread("assets/starry_night.png", IMREAD_COLOR);
	LOG_ASSERT(!image.empty(), "Could not load image!");

	// Scaling
	Mat halfImage, stretchImage, stretchNear;
	cv::resize(image, halfImage, { 0, 0 }, 0.5, 0.5);
	cv::resize(image, stretchImage, { 600, 600 });
	cv::resize(image, stretchNear, { 600, 600 }, 0, 0, INTER_NEAREST);

	cv::imshow("Half", halfImage);
	cv::imshow("Stretch", stretchImage);
	cv::imshow("Stretch Nearest", stretchNear);

	// Rotation
	Mat rotationMat = cv::getRotationMatrix2D({ 0, 0 }, -30, 1);
	Mat rotated;
	cv::warpAffine(image, rotated, rotationMat, rotated.size());

	cv::imshow("Rotated", rotated);
	
	cv::waitKey(0);
	cv::destroyAllWindows();
	
#endif

#if CH2_8 && VIDEO_FEED

	auto cap = VideoCapture(0);
	while(true) {

		Mat frame;
		cap.read(frame);

		cv::resize(frame, frame, { 0, 0 }, 0.5f, 0.5f);
		cv::imshow("Frame", frame);
		
		auto ch = cv::waitKey(1);
		if(ch > 0) {
			break;
		}
	}

	cap.release();
	cv::destroyAllWindows();
	
#endif

#if CH2_9 && VIDEO_FEED

	auto cap = VideoCapture(1);
	const Scalar color = { 0, 255, 0 };
	const int lineWidth = 3;
	const int radius = 100;
	Vec2i point = { 0, 0 };
	
	
	cv::namedWindow("Frame");
	cv::setMouseCallback("Frame", [](const int e, const int x, const int y, const int flags, void* userData) {
		if(e == cv::EVENT_LBUTTONDOWN) {
			LOG_INFO("Left button pressed: ({0}, {1})", x, y);
			*static_cast<Vec2i*>(userData) = { x, y };
		}

		if(e == cv::EVENT_MOUSEMOVE) {
			if(flags == cv::EVENT_FLAG_LBUTTON) {
				LOG_INFO("Mouse moved while pressed: ({0}, {1})", x, y);
				*static_cast<Vec2i*>(userData) = { x, y };
			}
		}
		
	}, &point);
	
	while(true) {

		Mat frame;
		cap.read(frame);

		cv::resize(frame, frame, { 0, 0 }, 0.5f, 0.5f);
		cv::circle(frame, point, radius, color, lineWidth);
		cv::imshow("Frame", frame);

		const auto ch = cv::waitKey(1);
		if(ch > 0) {
			break;
		}
	}

	cap.release();
	cv::destroyAllWindows();

#endif

#if CH2_DRAWING_APP

	struct CallbackData {
		Mat canvas = Mat::ones({ 500, 500 }, CV_8UC3);
		int radius = 3;
		Scalar color = { 0.0, 255.0, 0.0, 255.0 };
	} callbackData;


	callbackData.canvas = Scalar(255, 255, 255, 255);
	
	cv::namedWindow("Canvas");
	cv::setMouseCallback("Canvas", [](const int e, const int x, const int y, const int flags, void* userData) {
		CallbackData* params = static_cast<CallbackData*>(userData);
		
		switch(e) {
			case cv::EVENT_LBUTTONDOWN: {
				LOG_INFO("Left button pressed: ({0}, {1})", x, y);
				cv::circle(params->canvas, { x, y }, params->radius, params->color, FILLED);
				break;
			}
			case cv::EVENT_MOUSEMOVE: {
				if(flags == EVENT_FLAG_LBUTTON) {
					LOG_INFO("Mouse moved while pressed: ({0}, {1})", x, y);
					cv::circle(params->canvas, { x, y }, params->radius, params->color, FILLED);
				}
				break;
			}
			case cv::EVENT_LBUTTONUP: {
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
			case 'b': {
				callbackData.color = { 255.0, 0.0, 0.0 };
				break;
			}
			case 'g': {
				callbackData.color = { 0.0, 255.0, 0.0 };
				break;
			}
			case 'r': {
				callbackData.color = { 0.0, 0.0, 255.0 };
				break;
			}
			
			default: break;
			
		}
		
	}

	cv::destroyAllWindows();
	
#endif

#if CH3_2
	Mat bw = cv::imread("assets/detect_blob.png", IMREAD_GRAYSCALE);
	cv::imshow("Original B&W", bw);

	Mat binary = Mat::zeros(bw.rows, bw.cols, CV_8U);
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
		cv::imshow("Slow Binary", binary);
	}

	{
		Timer timer("CV threshold func", true);
		
		cv::threshold(bw, binary, threshold, 255, cv::THRESH_BINARY);
		cv::imshow("CV Threshold Func", binary);
	}
	
	const int ch = cv::waitKey(0);
	if(ch == 27) {
		cv::destroyAllWindows();
	}
	
#endif

#if CH3_3


	
#endif
	
	Logger::shutdown();
	
	return 0;
}