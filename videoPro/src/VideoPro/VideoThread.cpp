#include "VideoThread.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "Filter.h"
#include <iostream>
#include <QDateTime>
using namespace cv;

static VideoWriter vwriter;
static VideoCapture vcap1;
static bool bExit = false;

VideoThread::VideoThread()
{
	start();
}

VideoThread::~VideoThread()
{
	mutex.lock();
	bExit = true;
	mutex.unlock();
	wait();
}

double VideoThread::getPos()
{
	double pos = 0;
//	mutex.lock();
	if (!vcap1.isOpened())
	{
//		mutex.unlock();
		return pos;
	}
	
	double cur  = vcap1.get(CAP_PROP_POS_FRAMES);

	if (allframes > 0.001)
	{
		pos = cur / allframes;
	}
	
//	mutex.unlock();
	return pos;
	 
}

bool VideoThread::seek(int frame)
{
	mutex.lock();
	if (!vcap1.isOpened())
	{
		mutex.unlock();
		return false;
	}
	int re = vcap1.set(CAP_PROP_POS_FRAMES, frame);

	mutex.unlock();
	return re;
}

bool VideoThread::seek(double pos)
{
	double count = vcap1.get(CAP_PROP_FRAME_COUNT);
	int frame = pos*count;
	return seek(frame);
}

void VideoThread::setbegin(double p)
{
	mutex.lock();
	double count = vcap1.get(CAP_PROP_FRAME_COUNT);
	int frame = p * count;
	begin = frame;
	mutex.unlock();
}

void VideoThread::setend(double p)
{
	mutex.lock();
	double count = vcap1.get(CAP_PROP_FRAME_COUNT);
	int frame = p * count;
	end = frame;
	mutex.unlock();
}

bool VideoThread::startSave(std::string filename, int width /*= 0*/, int height /*= 0*/)
{
	mutex.lock();
	
	if (!vcap1.isOpened())
	{
		mutex.unlock();
		return false;
	}
	mutex.unlock();
	seek(begin);

	mutex.lock();
	if (width <= 0)
	{
		width = vcap1.get(CAP_PROP_FRAME_WIDTH);
	}
	if (height <= 0)
	{
		height = vcap1.get(CAP_PROP_FRAME_HEIGHT);
	}
	vwriter.open(filename,
		VideoWriter::fourcc('X', '2', '6', '4'),
		this->fps,
		Size(width, height));

	if (!vwriter.isOpened())
	{
		mutex.unlock();
		return false;
	}

	isWrite = true;
	savefile = filename;
	mutex.unlock();
	return true;
}

bool VideoThread::stopSave()
{
	mutex.lock();
	vwriter.release();
	isWrite = false;
	mutex.unlock();
	return true;
}

void VideoThread::setMark(Mat mark)
{
	mutex.lock();
	this->mark = mark;
	mutex.unlock();
}

void VideoThread::run()
{
	Mat mat1;
	int s = 0;
	
	if (fps <= 0)
	{
		std::cout << "fps maybe error" << std::endl;
		fps = 25;
	}
	
	

	for (;;)
	{
//		uint start_time = QDateTime::currentDateTime().time().msec();
		mutex.lock();
		if (bExit)
		{
			mutex.unlock();
			break;
		}
		if (!vcap1.isOpened())
		{
			mutex.unlock();
			msleep(5);
			continue;
		}
		if (!isPlay)
		{
			mutex.unlock();
			msleep(5);
			continue;
		}
		double cur = vcap1.get(CAP_PROP_POS_FRAMES);
		//读取一帧数据， 解码并颜色转换
		if ( (end >0 && cur >= end) || !vcap1.read(mat1) || mat1.empty())
		{		
			mutex.unlock();
			if (isWrite)
			{
				stopSave();
				emit saveEnd();
			}
			msleep(5);
			continue;
		}

		if (!isWrite)
		{
			emit showMat1(mat1);
		}
		
		Mat mat2 = mark;
		Mat des =  Filter::getinstance()->filter(mat1, mat2);

		emit showDes(des);
//		uint ed_time = QDateTime::currentDateTime().time().msec();
//		int waittime = ed_time - start_time;
		//每帧的时间
		//long d = s - waittime;

		if (isWrite)
		{
			s = 1;
			vwriter.write(des);
		}
		else {
			s = 1000 / fps;
		}

		msleep(s);

		mutex.unlock();
		
	}
}

bool VideoThread::open(const std::string file)
{
	mutex.lock();
	bool ret = vcap1.open(file);
	mutex.unlock();
	if (!ret)
	{
		std::cout << "cap1 open err" << std::endl;
		return false;
	}

	fps = vcap1.get(CAP_PROP_FPS);
	width = vcap1.get(CAP_PROP_FRAME_WIDTH);
	heigh = vcap1.get(CAP_PROP_FRAME_HEIGHT);
	if (fps <= 0)
	{
		std::cout << "fps maybe error" << std::endl;
		fps = 25;
	}
	src1file = file;
	allframes = vcap1.get(CAP_PROP_FRAME_COUNT);
	totalms = (allframes / (double)fps) * 1000;
	return true;
}

void VideoThread::play()
{
	mutex.lock();
	isPlay = true;
	mutex.unlock();
}

void VideoThread::pause()
{
	mutex.lock();
	isPlay = false;
	mutex.unlock();
}
