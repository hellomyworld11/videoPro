#pragma once

#include <QWidget>
#include "ui_PlayerUI.h"
#include <QTimerEvent>

class PlayerUI : public QWidget
{
	Q_OBJECT

public:
	PlayerUI(QWidget *parent = Q_NULLPTR);
	~PlayerUI();

	void timerEvent(QTimerEvent *e);


public slots:
	void on_btn_openvideo();
	void on_slider_press();
	void on_slider_release();
	void on_slider_move(int val);
	void on_btn_set();
	void on_btn_export();
	void on_exportend();
	void on_playpause();
	void on_btn_word();

	void on_slider_left(int val);
	void on_slider_right(int val);
private:
	Ui::PlayerUI ui;
	bool bplay = false;
};
