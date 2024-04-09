#include "VideoUI.h"

VideoUI::VideoUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initui();
	iniconnect();
}

void VideoUI::initui()
{
	setWindowFlags(Qt::FramelessWindowHint);
	//PlayerUI *playui = new PlayerUI;
	//int index = this->ui.stackedWidget->addWidget(playui);
	ui.stackedWidget->setCurrentIndex(0);

	QPixmap maxPix = style()->standardPixmap(QStyle::SP_TitleBarMaxButton);
	ui.btnmenu_max->setIcon(maxPix);

	QPixmap minPix = style()->standardPixmap(QStyle::SP_TitleBarMinButton);
	ui.btnmenu_min->setIcon(minPix);

	QPixmap closePix = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
	ui.btnmenu_close->setIcon(closePix);
}

void VideoUI::iniconnect()
{
	connect(ui.btnPlayer, &QToolButton::clicked,
		this, &VideoUI::on_player_btn);
	connect(ui.btnRecord, &QToolButton::clicked,
		this, &VideoUI::on_record_btn);
}

void VideoUI::on_record_btn()
{
	ui.stackedWidget->setCurrentIndex(2);
}

void VideoUI::on_player_btn()
{
	ui.stackedWidget->setCurrentIndex(0);
}

