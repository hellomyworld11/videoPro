#pragma once

#include <QtWidgets/QWidget>
#include "ui_VideoUI.h"
#include "PlayerUI.h"

class VideoUI : public QWidget
{
	Q_OBJECT

public:
	VideoUI(QWidget *parent = Q_NULLPTR);

	void initui();
	void iniconnect();
private slots:
	void on_record_btn();
	void on_player_btn();
private:
	Ui::MainFrame ui;
};
