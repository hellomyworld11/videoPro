#pragma once
#include <string>

class Audio
{
public:
	static Audio *getinstance();
	Audio();
	virtual ~Audio();

	virtual bool exportA(std::string src, std::string out,
		int beginms = 0, int endms = 0) = 0;
	virtual bool merge(std::string v, std::string a, std::string out)=0;

};

