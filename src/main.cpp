#include <QtCore/QCoreApplication>
#include <QTextStream>
#include "qbizmanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
	QBizManager _webSocket;

	QSettings m_settingsa(qApp->applicationDirPath() + "/bitcoin.ini", QSettings::IniFormat);
	QString cookie = m_settingsa.value("bitcoin/cookie").toString();
	_webSocket.bttname = m_settingsa.value("bitcoin/name").toString();

	//cookie = cookie.mid(0, cookie.length() - 1);// .replace("\"", "");
	//cookie = cookie.mid(1);
	//cookie = cookie.mid(0, cookie.length()-1);// .replace("\"", "");
	//m_settingsa.setValue("yobit/cookie", "a");
	//m_settingsa.setValue("yobit/name", "a");
	qDebug() << "main 1" << cookie;
	_webSocket.appendCookie(cookie);
	
	while (1)
	{
		_webSocket.doPost();
		
		QEventLoop loop;
		QTimer::singleShot(30000, &loop, SLOT(quit()));
		loop.exec();
	}
	
	QEventLoop loop;
	QTimer::singleShot(6000, &loop, SLOT(quit()));
	loop.exec();
	return a.exec();
}