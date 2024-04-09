#include "Audio.h"
#include <iostream>
#include <QMutex>
using namespace std;

struct AudioTime {
	AudioTime(int ms)
	{
		h = ((ms) / 1000) / 3600;
		m = ((ms / 1000) % 3600) / 60;
		s = (ms / 1000) % (3600 *60);
		ms = ms % 1000;
	}
	std::string tostring()
	{
		char buf[16] = { 0 };
		sprintf(buf, "%d:%d:%d.%d", h , m, s, ms);
		return buf;
	}
	int h = 0;
	int m = 0;
	int s = 0;
	int ms = 0;
};

class CAutdio : public Audio
{
public:
	QMutex mutex;
	bool exportA(std::string src, std::string out, int beginms = 0, int endms = 0)
	{
		
		//ffmpeg -i xxx.mp4 -ss 0:1:10.112 -t 0:1:10.100  -vn -y out.mp3
		string cmd = "ffmpeg ";
		cmd += "-i ";
		cmd += src;
		if (beginms > 0)
		{
			cmd += " -ss ";
			AudioTime t(beginms);
			cmd += t.tostring();
		}
		if (endms > 0)
		{
			cmd += " -t ";
			AudioTime t(endms);
			cmd += t.tostring();
		}
		cmd += " -vn -y ";
		cmd += out;
		cout <<"cmd :" <<cmd << endl;
		mutex.lock();
		system(cmd.c_str());
		mutex.unlock();
		return true;
	}
	bool merge(std::string v, std::string a, std::string out)
	{
		//ffmpeg -i test.avi  -i test.mp3 -c copy out.avi
		string cmd = "ffmpeg -i ";
		cmd += v;
		cmd += " -i ";
		cmd += a;
		cmd += " -c copy ";
		cmd += out;
		 
		cout << "cmd :" << cmd << endl;
		mutex.lock();
		system(cmd.c_str());
		mutex.unlock();
		return true;
	}
};



Audio * Audio::getinstance()
{
	static CAutdio ca;
	return &ca;
}

Audio::Audio()
{
}


Audio::~Audio()
{
}
