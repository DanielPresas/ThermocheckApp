#include "vdpch.h"

#include <opencv2/opencv.hpp>

#define CH4_EYE_DETECT 0

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
					LOG_TRACE("Mouse moved while pressed: ({0}, {1})", x, y);
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
	
#endif

#if CH3_3

	Mat img = cv::imread("assets/sudoku.png", IMREAD_GRAYSCALE);
	cv::imshow("Original", img);

	Mat threshBasic;
	{
		Timer timer("Threshold basic", true);
		cv::threshold(img, threshBasic, 70, 255, THRESH_BINARY);
	}
	cv::imshow("Basic Binary", threshBasic);
	
	Mat adaptiveThresh;
	{
		Timer timer("Adaptive threshold", true);
		cv::adaptiveThreshold(img, adaptiveThresh, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 115, 1);
	}
	cv::imshow("Adaptive Binary", adaptiveThresh);
	
	const int ch = cv::waitKey(0);
	if(ch == 27) {
		cv::destroyAllWindows();
	}
	
#endif

#if CH3_4

	Mat img = cv::imread("assets/faces.jpeg", IMREAD_COLOR);
	LOG_ASSERT(!img.empty(), "Failed to load image!");
	cv::resize(img, img, { 0, 0 }, 0.15, 0.15);
	
	Mat hsv;
	cv::cvtColor(img, hsv, COLOR_BGR2HSV);

	Mat hsvChannels[3], hsvSplit;
	cv::split(hsv, hsvChannels);
	cv::hconcat(hsvChannels, 3, hsvSplit);
	cv::imshow("Split HSV", hsvSplit);

	Mat minSaturation;
	cv::threshold(hsvChannels[1], minSaturation, 40, 255, THRESH_BINARY);
	cv::imshow("Saturation Filter", minSaturation);
	
	Mat maxHue;
	cv::threshold(hsvChannels[0], maxHue, 15, 255, THRESH_BINARY_INV);
	cv::imshow("Hue Filter", maxHue);

	Mat finalMat;
	cv::bitwise_and(minSaturation, maxHue, finalMat);
	cv::imshow("Final", finalMat);
	cv::imshow("Original", img);
	
	const int ch = cv::waitKey(0);
	if(ch == 27) {
		cv::destroyAllWindows();
	}
	
#endif

#if CH3_5
	
	Mat img = cv::imread("assets/detect_blob.png", IMREAD_COLOR);
	LOG_ASSERT(!img.empty(), "Failed to load image!");
	Mat gray; cv::cvtColor(img, gray, COLOR_RGB2GRAY);
	Mat thresh; cv::adaptiveThreshold(gray, thresh, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 115, 1);
	cv::imshow("Binary", thresh);

	std::vector<Mat> contours; Mat hierarchy; cv::findContours(thresh, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
	Mat img2 = img.clone();

	cv::drawContours(img2, contours, -1, { 255, 0, 255 }, 4);
	cv::imshow("Contours", img2);
	
	const int ch = cv::waitKey(0);
	if(ch == 27) {
		cv::destroyAllWindows();
	}
	
#endif
	
#if CH3_6
	
	Mat img = cv::imread("assets/detect_blob.png", IMREAD_COLOR);
	LOG_ASSERT(!img.empty(), "Failed to load image!");
	Mat gray; cv::cvtColor(img, gray, COLOR_RGB2GRAY);
	Mat thresh; cv::adaptiveThreshold(gray, thresh, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 115, 1);
	cv::imshow("Binary", thresh);

	std::vector<Mat> contours; Mat hierarchy; cv::findContours(thresh, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	Mat objects = Mat::zeros(img.rows, img.cols, CV_8UC3);
	uint32_t i = 0;
	for(const auto& contour : contours) {
		cv::drawContours(objects, std::array<Mat, 1>{ contour }, -1, { 255, 0, 255 }, FILLED);

		double area = cv::contourArea(contour);
		double perimeter = cv::arcLength(contour, true);

		Moments moment = cv::moments(contour);
		int centroidX = static_cast<int>(moment.m10 / moment.m00);
		int centroidY = static_cast<int>(moment.m01 / moment.m00);
		cv::circle(objects, { centroidX, centroidY }, 4, { 0, 0, 255 }, FILLED);

		// https://fmt.dev/latest/syntax.html to read up on what the heck this all means lol
		LOG_TRACE("Contour {0:02} [ Area: {1:>05.2F}, Perimeter: {2:>05.2F} ]", ++i, area, perimeter);
	}
	
	cv::imshow("Contours", objects);
	
	const int ch = cv::waitKey(0);
	if(ch == 27) {
		cv::destroyAllWindows();
	}
	
#endif

#if CH3_7

	Mat img = cv::imread("assets/tomatoes.jpg", IMREAD_COLOR);
	LOG_ASSERT(!img.empty(), "Failed to load image!");
	Mat hsv; cv::cvtColor(img, hsv, COLOR_BGR2HSV);
	std::array<Mat, 3> hsvChannels; cv::split(hsv, hsvChannels);
	Mat thresh; cv::threshold(hsvChannels.at(0), thresh, 25, 255, THRESH_BINARY_INV);
	cv::imshow("Threshold", thresh);

	Mat edges; cv::Canny(img, edges, 100, 70);
	cv::imshow("Canny edges", edges);
	
	const int ch = cv::waitKey(0);
	if(ch == 27) {
		cv::destroyAllWindows();
	}

	
	
#endif

#if CH3_OBJECT_ID

	Mat img = cv::imread("assets/fuzzy.png", IMREAD_COLOR);
	LOG_ASSERT(!img.empty(), "Failed to load image!");
	cv::imshow("Original", img);

	Mat thresh;
	cv::cvtColor(img, thresh, COLOR_BGR2GRAY);
	cv::GaussianBlur(thresh, thresh, { 3, 3 }, 0);
	cv::adaptiveThreshold(thresh, thresh, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 205, 1);
	cv::imshow("Binary", thresh);

	std::vector<Mat> contours; cv::findContours(thresh, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
	LOG_INFO("Number of contours: {}", contours.size());

	std::vector<Mat> filtered;
	for(const auto& contour : contours) {
		if(cv::contourArea(contour) < 1000.0) continue;
		filtered.push_back(contour);
	}
	LOG_INFO("Number of contours with area >1000: {}", filtered.size());
	
	Mat objects = Mat::zeros(img.rows, img.cols, CV_8UC3);
	uint32_t i = 0;
	for(const auto& c : filtered) {
		Scalar col = { static_cast<double>(Random::UInt(0, 255)), static_cast<double>(Random::UInt(0, 255)), static_cast<double>(Random::UInt(0, 255)) };
		cv::drawContours(objects, std::vector<Mat>{ c }, -1, col, FILLED);

		double area = cv::contourArea(c);
		double perimeter = cv::arcLength(c, true);
		LOG_TRACE("Contour {0:02} [ Area: {1:>05.2F}, Perimeter: {2:>05.2F} ]", ++i, area, perimeter);
	}

	cv::imshow("Contours", objects);
	
	const int ch = cv::waitKey(0);
	if(ch == 27) {
		cv::destroyAllWindows();
	}
	
#endif

#if CH4_3

	Mat tmpl = cv::imread("assets/template.jpg", IMREAD_GRAYSCALE);
	Mat frame = cv::imread("assets/players.jpg", IMREAD_GRAYSCALE);

	cv::imshow("Frame", frame);
	cv::imshow("Template", tmpl);

	Mat result; cv::matchTemplate(frame, tmpl, result, TM_CCOEFF_NORMED);

	double minVal, maxVal;
	Point minLoc, maxLoc;

	cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
	LOG_TRACE("Max value: {} at ({}, {})", maxVal, maxLoc.x, maxLoc.y);
	LOG_TRACE("Min value: {} at ({}, {})", minVal, minLoc.x, minLoc.y);

	cv::circle(result, minLoc, 15, 100 / 255.0f, 2);
	cv::circle(result, maxLoc, 15, 255 / 255.0f, 2);
	
	cv::imshow("Matching", result);

	const int ch = cv::waitKey(0);
	if(ch == 27) {
		cv::destroyAllWindows();
	}
	
	
#endif

#if CH4_5

	UMat img; cv::imread("assets/faces.jpeg", IMREAD_COLOR).copyTo(img);
	LOG_ASSERT(!img.empty(), "Failed to load image!");
	
	UMat gray; cv::cvtColor(img, gray, COLOR_BGR2GRAY);

	auto faceCascade = CascadeClassifier("assets/haarcascade_frontalface_default.xml");
	LOG_ASSERT(!faceCascade.empty(), "Failed to load cascade classifier!");
	std::vector<Rect> faces;
	
	{
		Timer timer("Face cascade", true);
		faceCascade.detectMultiScale(gray, faces, 1.1, 5, 0, { 40, 40 });
	}
	LOG_TRACE("Number of faces detected: {}", faces.size());

	for(const auto& rect : faces) {
		cv::rectangle(img, { rect.x, rect.y }, { rect.x + rect.width, rect.y + rect.height }, { 0, 255, 0 }, 4);
	}

	cv::resize(img, img, { 0,0 }, 0.30, 0.30);
	cv::imshow("Image", img);

	const int ch = cv::waitKey(0);
	if(ch == 27) {
		cv::destroyAllWindows();
	}
	
#endif

#if CH4_EYE_DETECT

	UMat img; cv::imread("assets/faces.jpeg", IMREAD_COLOR).copyTo(img);
	LOG_ASSERT(!img.empty(), "Failed to load image!");
	UMat gray; cv::cvtColor(img, gray, COLOR_BGR2GRAY);

	auto eyeCascade = CascadeClassifier("assets/haarcascade_eye.xml");
	LOG_ASSERT(!eyeCascade.empty(), "Failed to load cascade classifier!");
	
	std::vector<Rect> eyes;
	{
		Timer timer("Eye detection", true);
		eyeCascade.detectMultiScale(gray, eyes, 1.02, 20, 0, { 10, 10 });
	}
	LOG_TRACE("Number of eyes detected: {}", eyes.size());

	for(const auto& eye : eyes) {
		const double xc = (eye.x + eye.x + eye.width) / 2.0;
		const double yc = (eye.y + eye.y + eye.height) / 2.0;
		const double radius = eye.width / 2.0;
		
		cv::circle(img, { static_cast<int>(xc), static_cast<int>(yc) }, static_cast<int>(radius), { 255, 0, 0 }, 4);
	}

	cv::resize(img, img, { 0,0 }, 0.30, 0.30);
	cv::imshow("Eyes", img);
	
	const int ch = cv::waitKey(0);
	if(ch == 27) {
		cv::destroyAllWindows();
	}
	
#endif


	auto cam = VideoCapture(1);

	cv::namedWindow("Capture");

	while(true) {
		UMat frame;
		LOG_ASSERT(cam.read(frame), "Failed to read frame from capture device!");
		cv::imshow("Capture", frame);

		UMat hsv; cv::cvtColor(frame, hsv, COLOR_BGR2HSV);
		std::array<Mat, 3> hsvChannels; cv::split(hsv, hsvChannels);

		UMat minSat, maxHue;
		cv::threshold(hsvChannels[0], maxHue, 15, 255, THRESH_BINARY_INV);
		cv::threshold(hsvChannels[1], minSat, 40, 255, THRESH_BINARY);
		cv::bitwise_and(minSat, maxHue, hsv);
		cv::imshow("HSV Skin Detect", hsv);
		
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