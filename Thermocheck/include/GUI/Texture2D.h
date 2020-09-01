#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include "Utils/Math.h"
#include <opencv2/opencv.hpp>

class Texture2D {
public:

	Texture2D() = default;
	~Texture2D();

	void setData(cv::UMat arr);

	
	intptr_t getRendererId() const { return static_cast<intptr_t>(_id); }

	cv::UMat getUMat()       const { return mat; }
	bool     empty()         const { return _empty; }
	
	float    getWidth()      const { return static_cast<float>(mat.cols); }
	float    getHeight()     const { return static_cast<float>(mat.rows); }
	Vector2  getSize()       const { return { static_cast<float>(mat.cols), static_cast<float>(mat.rows) }; }

private:

	void initTexture2D(cv::UMat arr);
	
	cv::UMat mat;
	bool _empty = true;
	uint32_t _id = 0;
	
};

#endif