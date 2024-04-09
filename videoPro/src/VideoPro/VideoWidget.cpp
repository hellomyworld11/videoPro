#include "VideoWidget.h"
#include <QPainter>
#include <opencv2/imgproc.hpp>
using namespace cv;

VideoWidget::VideoWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{
	

}

VideoWidget::~VideoWidget()
{
}

void VideoWidget::paintEvent(QPaintEvent *e)
{
	QPainter painter;
	painter.begin(this);
	painter.drawImage(QPoint(0, 0), img);
	painter.end();
}

void VideoWidget::on_showMat(cv::Mat mat)
{
	if (img.isNull())
	{
		uchar *buf = new uchar[width()*height() * 3];
		img = QImage(buf, width(), height(), QImage::Format_RGB888);

	}

	Mat des;
	cv::resize(mat, des, Size(img.size().width(), img.size().height()));
	cv::cvtColor(des, des, COLOR_BGR2RGB);

	memcpy(img.bits(), des.data, des.cols*des.rows*des.elemSize());
	update();
}

