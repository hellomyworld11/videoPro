#include "Filter.h"
#include "ImageInter.h"


Filter::Filter()
{
}

Filter::~Filter()
{

}

Filter * Filter::getinstance()
{
	static XFilter  filter;
	return &filter;
}

cv::Mat XFilter::filter(cv::Mat mat1, cv::Mat mat2)
{
	mutex.lock();
	ImageInter p;
	p.set(mat1, mat2);
	for (int i = 0; i < tasks.size(); i++)
	{
		switch (tasks[i].type)
		{
		case taskType::GAIN:
			p.gain(tasks[i].paras[0], tasks[i].paras[1]);
			break;
		case taskType::ROTATE:
			p.rotate_(tasks[i].paras[0]);
			break;
		case taskType::FLIPX:
			p.flipX();
			break;
		case taskType::FLIPY:
			p.flipY();
			break;
		case taskType::FLIPXY:
			p.flipXY();
			break;
		case taskType::RSIZE:
			p.resize_(tasks[i].paras[0], tasks[i].paras[1]);
			break;
		case taskType::PYDOWN:
 			p.pydown(tasks[i].paras[0]);
			break;
		case taskType::PYUP:
			p.pyup(tasks[i].paras[0]);
			break;
		case taskType::MARK:
			p.mark(tasks[i].paras[0], tasks[i].paras[1], tasks[i].paras[2]);
			break;
		default:
			break;
		}
	}
	cv::Mat re = p.get();
	mutex.unlock();
	return re;
}

void XFilter::add(frametask task)
{
	mutex.lock();
	tasks.push_back(task);
	mutex.unlock();
}

void XFilter::clear()
{
	mutex.lock();
	tasks.clear();
	mutex.unlock();
}
