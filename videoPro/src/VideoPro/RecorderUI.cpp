#include "RecorderUI.h"

RecorderUI::RecorderUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
}

RecorderUI::~RecorderUI()
{
}
