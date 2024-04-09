#pragma once
#include <opencv2/core.hpp>



class ImageInter
{
public:
	ImageInter();
	~ImageInter();

	cv::Mat get() { return des; }
	void set(cv::Mat mat1, cv::Mat mat2);

	// 亮度 对比度
	void gain(double bright, double contrast);

	void rotate_(double angle);

	void flipX();
	void flipY();
	void flipXY();
	void resize_(int width, int height);

	//图像金字塔
	void pydown(int count);
	void pyup(int count);

	void mark(int x, int y, double a);

private:
	cv::Mat src1;
	cv::Mat src2;

	cv::Mat des;
};

