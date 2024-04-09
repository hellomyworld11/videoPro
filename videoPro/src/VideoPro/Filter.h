#pragma once
#include <opencv2/core.hpp>
#include <vector>
#include <QThread>
enum class taskType
{
	NONE,
	GAIN,
	ROTATE,
	FLIPX,
	FLIPY,
	FLIPXY,
	RSIZE,
	PYDOWN,
	PYUP,
	MARK
	//...
};


struct frametask {
	taskType type;
	std::vector<double> paras;
};

class Filter
{
public:
	static Filter *getinstance();

	virtual ~Filter();

	virtual cv::Mat filter(cv::Mat mat1, cv::Mat mat2) = 0;

	virtual void add(frametask task) = 0;
	virtual void clear() = 0;
protected:
	Filter();
};

class XFilter : public Filter
{
public:
	virtual cv::Mat filter(cv::Mat mat1, cv::Mat mat2);

	virtual void add(frametask task) ;
	virtual void clear() ;

	std::vector<frametask> tasks;
	QMutex mutex;
};