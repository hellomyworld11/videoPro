#include "VideoUI.h"
#include <QtWidgets/QApplication>
#include "Audio.h"
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	//加载样式表
	QFile file(":/qss/VideoUI.css");
	if (file.open(QFile::ReadOnly)) {
		QString qss = QLatin1String(file.readAll());
		QString paletteColor = qss.mid(20, 7);
	//	qApp->setPalette(QPalette(QColor(paletteColor)));
	//	qApp->setStyleSheet(qss);
		file.close();
	}
	VideoUI w;
	w.show();
	return a.exec();
}
