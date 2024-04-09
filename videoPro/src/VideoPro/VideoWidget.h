#pragma once
#include <QOpenGLWidget>
#include <opencv2/core.hpp>
#include <QPaintEvent>

class VideoWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	VideoWidget(QWidget *parent);
	~VideoWidget();
	void paintEvent(QPaintEvent *e);
	
public slots:
	void on_showMat(cv::Mat mat);
protected:
	QImage img;
};
