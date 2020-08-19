#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include <cstdint>
#include <imgui/imgui.h>
#include <opencv2/opencv.hpp>


class Texture2D {
public:

	Texture2D() = default;
	~Texture2D();

	void setData(cv::InputArray arr);
	
	intptr_t getRendererId() const { return static_cast<intptr_t>(_id); }
	
	int32_t getWidth()      const { return mat.cols; }
	int32_t getHeight()     const { return mat.rows; }
	ImVec2  getSize()       const { return { static_cast<float>(mat.cols), static_cast<float>(mat.rows) }; }

	
private:

	void initTexture2D(cv::InputArray arr);
	
	cv::UMat mat;
	uint32_t _id = 0;
	
};

#endif