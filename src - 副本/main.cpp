

#include <QtCore/QCoreApplication>
#include <QTextStream>
#include "qbizmanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


	QBizManager _webSocket;

	_webSocket.appendCookie("__cfduid=d5381fbd5821b76f56265c4e036eb00231528861346; SMFCookie129=a%3A4%3A%7Bi%3A0%3Bs%3A7%3A%221169050%22%3Bi%3A1%3Bs%3A40%3A%2260bd1c0fe9de78d493d813a24d95316d028485df%22%3Bi%3A2%3Bi%3A1718077687%3Bi%3A3%3Bi%3A0%3B%7D; PHPSESSID=ea904vp6a70sf3vjsns1uu8ll0");
	_webSocket.appendCookie("__cfduid=dffd55961d70e3686fbaae9b0c6f359921512108285; SMFCookie129=a%3A4%3A%7Bi%3A0%3Bs%3A7%3A%221145707%22%3Bi%3A1%3Bs%3A40%3A%227c876b6ef4adb674df22cb245b0687d3c5a3e83e%22%3Bi%3A2%3Bi%3A1718076991%3Bi%3A3%3Bi%3A0%3B%7D; PHPSESSID=676te419num0unjb037vdkpil4");

	_webSocket.doPost();
	
	QEventLoop loop;
	QTimer::singleShot(6000, &loop, SLOT(quit()));
	loop.exec();


	


	return a.exec();
}