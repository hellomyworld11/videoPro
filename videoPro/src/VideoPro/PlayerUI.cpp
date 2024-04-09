#include "PlayerUI.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include "VideoThread.h"
#include <iostream>
#include <QSlider>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "Filter.h"
#include "Audio.h"
static bool pressSlider = false;
static bool isExport = false;
static bool isMark = false;
PlayerUI::PlayerUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
	qRegisterMetaType<cv::Mat>("cv::Mat");

	connect(ui.btn_openvideo, &QPushButton::clicked,
		this, &PlayerUI::on_btn_openvideo);

	
	connect(VideoThread::getintance(),
		&VideoThread::showMat1,
		ui.srcwidget1, &VideoWidget::on_showMat);

	connect(VideoThread::getintance(),
		&VideoThread::showDes,
		ui.deswidget, &VideoWidget::on_showMat);

	connect(ui.playerslider, &QSlider::sliderPressed,
		this, &PlayerUI::on_slider_press);

	connect(ui.playerslider, &QSlider::sliderReleased,
		this, &PlayerUI::on_slider_release);

	connect(ui.playerslider, &QSlider::sliderMoved,
		this, &PlayerUI::on_slider_move);

	connect(ui.btn_set, &QPushButton::clicked,
		this, &PlayerUI::on_btn_set);

	connect(ui.btn_export, &QPushButton::clicked,
		this, &PlayerUI::on_btn_export);

	connect(VideoThread::getintance(),
		&VideoThread::saveEnd,
		this, &PlayerUI::on_exportend);

	connect(ui.btn_stop, &QPushButton::clicked,
		this, &PlayerUI::on_playpause);

	connect(ui.btn_word, &QPushButton::clicked,
		this, &PlayerUI::on_btn_word);

	connect(ui.playerslider_begin, &QSlider::sliderMoved,
		this, &PlayerUI::on_slider_left);
	connect(ui.playerslider_end, &QSlider::sliderMoved,
		this, &PlayerUI::on_slider_right);

	ui.btn_stop->setText("play");

	startTimer(40);
}

PlayerUI::~PlayerUI()
{ 
}

void PlayerUI::timerEvent(QTimerEvent *e)
{
	if (pressSlider) return;
	double pos = VideoThread::getintance()->getPos();
	ui.playerslider->setValue(pos * 1000);
}

void PlayerUI::on_btn_openvideo()
{
	//QMessageBox::warning(nullptr, "ceshi", "ces");
	QString strname =  QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("select video"),
		"../test/");

	if (strname.isEmpty())
	{
		QMessageBox::warning(nullptr, "openFile", "failed");
		return;
	}

	std::string file = strname.toLocal8Bit().data();

	bool ret =  VideoThread::getintance()->open(file);

	std::cout << "ret:" << std::endl;
	
	ui.lineEdit_w->setText(QString("%1").arg(VideoThread::getintance()->getwidth()) );
	ui.lineEdit_h->setText(QString("%1").arg(VideoThread::getintance()->getheigh()));
	on_playpause();
}

void PlayerUI::on_slider_press()
{
	pressSlider = true;
}

void PlayerUI::on_slider_release()
{
	pressSlider = false;
}

void PlayerUI::on_slider_move(int val)
{
	VideoThread::getintance()->seek((double)val / 1000.0);
}

void PlayerUI::on_btn_set()
{
	Filter::getinstance()->clear();

	double w = ui.lineEdit_w->text().toDouble();
	double h = ui.lineEdit_h->text().toDouble();

	int countup = ui.lineEdit_gaosi->text().toInt();
	int countdn = ui.lineEdit_laplase->text().toInt();

	bool bpy = false;
	int srcw = VideoThread::getintance()->getwidth();
	int srch = VideoThread::getintance()->getheigh();
	if (countup > 0)
	{
		Filter::getinstance()->add(frametask{ taskType::PYUP,{ (double)countup } });
		for (int i = 0; i < countup; i++)
		{
			srcw = srcw * 2;
			srch = srch * 2;
		}
		bpy = true;
		
	}
	if (countdn > 0)
	{
		Filter::getinstance()->add(frametask{ taskType::PYDOWN,{ (double)countdn } });
		for (int i = 0; i < countdn; i++)
		{
			srcw = srcw / 2;
			srch = srch / 2;
		}
		bpy = true;
	}

	ui.lineEdit_w->setText(QString("%1").arg(srcw));
	ui.lineEdit_h->setText(QString("%1").arg(srch));

	if (!bpy &&  w > 0 && h > 0)
	{
		Filter::getinstance()->add(frametask{ taskType::RSIZE,{ w, h } });
	}

	double bright = ui.spinbox_bright->value();
	double contrast = ui.spinbox_contrast->value();
	if (bright > 0
		|| contrast > 1)
	{
		Filter::getinstance()->add(frametask{ taskType::GAIN,
		{(double)bright, contrast } });
	}

	double rotateV = ui.combo_rotate->currentText().toDouble();
	if (rotateV != 0)
	{
		Filter::getinstance()->add(frametask{ taskType::ROTATE,
		{ (double)rotateV} });
	}

	int index = ui.combo_flip->currentIndex();
	QString strFlip = ui.combo_flip->itemText(index);
	if (strFlip.toLocal8Bit() == "上下")
	{
		Filter::getinstance()->add(frametask{ taskType::FLIPX});
	}
	else if (strFlip.toLocal8Bit() == "左右")
	{
		Filter::getinstance()->add(frametask{ taskType::FLIPY });
	}
	else if (strFlip.toLocal8Bit() == "上下左右")
	{
		Filter::getinstance()->add(frametask{ taskType::FLIPXY });
	}

	if (isMark)
	{
		double x = ui.lineEdit_wordw->text().toDouble();
		double y = ui.lineEdit_wordh->text().toDouble();
		double a = ui.lineEdit_worda->text().toDouble();
		Filter::getinstance()->add(frametask{ taskType::MARK, {x, y, a} });
	}


}

void PlayerUI::on_btn_export()
{
	
	if (isExport)
	{
		VideoThread::getintance()->stopSave();
		isExport = false;
		ui.btn_export->setText("start export");
		return;
	}
	QString name = QFileDialog::getSaveFileName(this, "save ", "out.avi");
	if (name.isEmpty())
	{
		return;
	}
	std::string filename = name.toLocal8Bit().data();

	double w = ui.lineEdit_w->text().toDouble();
	double h = ui.lineEdit_h->text().toDouble();

	if (VideoThread::getintance()->startSave(filename, w, h))
	{
		ui.btn_export->setText("stop export");
		isExport = true;
	}

}

void PlayerUI::on_exportend()
{
	using namespace std;
	isExport = false;
	ui.btn_export->setText("Start export");

	std::string src = VideoThread::getintance()->getsrc();
	std::string des = VideoThread::getintance()->getsave();

	int ss = 0;
	int t = 0;
	ss = VideoThread::getintance()->totalms * ((double)ui.playerslider_begin->value()/1000);
	int end = VideoThread::getintance()->totalms * ((double)ui.playerslider_end->value() / 1000);
	t = end - ss;
	ui.playerslider_end->value();

	Audio::getinstance()->exportA(src, src + ".mp3", ss, t);
	string tmp = des + ".avi";
	QFile::remove(tmp.c_str());
	QFile::rename(des.c_str(), tmp.c_str());
	Audio::getinstance()->merge(tmp, src + ".mp3", des);
}

void PlayerUI::on_playpause()
{
	bplay = !bplay;
	if (bplay)
	{
		ui.btn_stop->setText("pause");
		VideoThread::getintance()->play();
	}
	else {
		ui.btn_stop->setText("play");
		VideoThread::getintance()->pause();
	}
	
}

void PlayerUI::on_btn_word()
{
	QString name = QFileDialog::getOpenFileName(this,
		"select image:");

	isMark = false;
	if (name.isEmpty())
	{	
		return;
	}

	std::string file = name.toLocal8Bit().data();
	cv::Mat mark = cv::imread(file);
	if (mark.empty())
	{
		return;
	}
	VideoThread::getintance()->setMark(mark);
	isMark = true;
}

void PlayerUI::on_slider_left(int val)
{
	VideoThread::getintance()->setbegin((double)val / 1000.0);
	on_slider_move(val);
	
}

void PlayerUI::on_slider_right(int val)
{
	VideoThread::getintance()->setend((double)val / 1000.0);
}
