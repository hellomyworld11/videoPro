#pragma once

#include <QWidget>
#include "ui_RecorderUI.h"

class RecorderUI : public QWidget
{
	Q_OBJECT

public:
	RecorderUI(QWidget *parent = Q_NULLPTR);
	~RecorderUI();

private:
	Ui::RecorderUI ui;
};
