#include "ImageInter.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

ImageInter::ImageInter()
{
}


ImageInter::~ImageInter()
{
}

void ImageInter::set(cv::Mat mat1, cv::Mat mat2)
{
	if (mat1.empty())
	{
		return;
	}
	src1 = mat1;
	src2 = mat2;
	src1.copyTo(des);
}

void ImageInter::gain(double bright, double contrast)
{
	if (des.empty())
	{
		return;
	}
	des.convertTo(des, -1, contrast, bright);
}

void ImageInter::rotate_(double angle)
{
	if (des.empty()) return;

	int rotatecode = cv::ROTATE_90_CLOCKWISE;
	if (angle == 90)
	{
		//rotatecode = cv::ROTATE_90_CLOCKWISE;
	}else if (angle == 180)
	{
		rotatecode = cv::ROTATE_180;
	}else if (angle == 270)
	{
		rotatecode = cv::ROTATE_90_COUNTERCLOCKWISE;
	}
	rotate(des, des, rotatecode);
}

void ImageInter::flipX()
{
	if (des.empty()) return;
	flip(des, des, 0);
}

void ImageInter::flipY()
{
	if (des.empty()) return;
	flip(des, des, 1);
}

void ImageInter::flipXY()
{
	if (des.empty()) return;
	flip(des, des, -1);
}

void ImageInter::resize_(int width, int height)
{
	if (des.empty()) return;
	cv::resize(des, des, cv::Size(width, height));
}

void ImageInter::pydown(int count)
{
	if (des.empty()) return;
	for (int i = 0; i < count; i++)
	{
		cv::pyrDown(des, des);
	}
	
}

void ImageInter::pyup(int count)
{
	if (des.empty()) return;
	for (int i = 0; i < count; i++)
	{
		cv::pyrUp(des, des);
	}
}

void ImageInter::mark(int x, int y, double a)
{
	if (des.empty()) return;
	if (src2.empty())return;
	
	cv::Mat rol = des(cv::Rect(x, y, src2.cols, src2.rows));
	addWeighted(src2, a, rol, 1 - a, 0, rol);
}
