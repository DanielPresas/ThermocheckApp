#include "tcpch.h"
#include "GUI/Texture2D.h"

#include <glad/glad.h>
#include <opencv2/opencv.hpp>

Texture2D::~Texture2D() {
	glDeleteTextures(1, &_id);
}

void Texture2D::setData(const cv::UMat arr) {
	_empty = arr.empty();

	if(!_empty) {
		if(_id <= 0) {
			initTexture2D(arr);
			return;
		}

		if(arr.size() != mat.size()) {
			glDeleteTextures(1, &_id);
			mat.release();
			initTexture2D(arr);
			return;
		}

		glBindTexture(GL_TEXTURE_2D, _id);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mat.cols, mat.rows, GL_BGR, GL_UNSIGNED_BYTE, arr.getMat(cv::ACCESS_READ).data);
	}
}

void Texture2D::initTexture2D(const cv::UMat arr) {
	mat = arr;
	
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, mat.cols, mat.rows);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mat.cols, mat.rows, GL_BGR, GL_UNSIGNED_BYTE, mat.getMat(cv::ACCESS_READ).data);
}
