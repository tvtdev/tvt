#include <QtCore/QCoreApplication>
#include <QTextStream>
#include "qbizmanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
	QBizManager _webSocket;

	_webSocket.appendCookie("__cfduid=d837358af81e46c99b44800d63f03af841555578824; PHPSESSID=ibkbotjaj8n3l0t6n3ovku36qskrkg7d; SMFCookie129=a%3A4%3A%7Bi%3A0%3Bs%3A7%3A%221168604%22%3Bi%3A1%3Bs%3A40%3A%227313b540eb3cc067e88201590f09b3e0b656e79a%22%3Bi%3A2%3Bi%3A1756022114%3Bi%3A3%3Bi%3A0%3B%7D; sessionid=8qmqvi2v2ljeaqlvnej1iuvffpdp50po");
	
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