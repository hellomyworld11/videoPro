#pragma once

#include <QThread>
#include <QMutex>
#include <opencv2/core.hpp>

class VideoThread : public QThread
{

	Q_OBJECT

public:
	int totalms;

	static VideoThread* getintance() {
		static VideoThread gthread;
		return &gthread;
	}

	~VideoThread();


	double getPos();
	int getwidth() { return width; }
	int getheigh() { return heigh; }
	std::string getsrc() { return src1file; }
	std::string getsave() { return savefile; }

	//��ת��Ƶ
	bool seek(int frame);
	bool seek(double pos);
	void setbegin(double p);
	void setend(double p);

	//��ʼ������Ƶ
	bool startSave(std::string filename,int width = 0, int height = 0);
	//ֹͣ������Ƶ�� д����Ƶ֡������
	bool stopSave();

	void setMark(cv::Mat mark);

	void run();

	bool open(const std::string file);
	void play();
	void pause();
signals:
	void showMat1(cv::Mat img);
	void showDes(cv::Mat img);
	void saveEnd();
protected:
	VideoThread();

	QMutex mutex;
	int fps;
	int allframes; //��֡��
	
	int width;
	int heigh;
	bool isWrite = false;
	bool isPlay = false;

	cv::Mat mark;
	std::string src1file;
	std::string savefile;

	int begin = 0;
	int end = 0;
};
