#include "tcpch.h"
#include "GUI/Texture2D.h"

#include <glad/glad.h>
#include <opencv2/opencv.hpp>

Texture2D::~Texture2D() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &_id);
}

void Texture2D::setData(cv::InputArray arr) {

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

	Logger::logAssert(!arr.empty(), "No data in given UMat argument!");
	glTextureSubImage2D(_id, 0, 0, 0, mat.cols, mat.rows, GL_BGR, GL_UNSIGNED_BYTE, arr.getMat().data);
}

void Texture2D::initTexture2D(cv::InputArray arr) {
	Logger::logAssert(!arr.empty(), "No data in given image!");
	mat = arr.getUMat();

	glCreateTextures(GL_TEXTURE_2D, 1, &_id);
	glTextureParameteri(_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	// glPixelStorei(GL_PACK_ALIGNMENT, 1);
	
	glTextureStorage2D(_id, 1, GL_RGB8, mat.cols, mat.rows);
	glTextureSubImage2D(_id, 0, 0, 0, mat.cols, mat.rows, GL_BGR, GL_UNSIGNED_BYTE, arr.getMat().data);
}
