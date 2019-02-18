﻿#include <QtCore/QCoreApplication>
#include <QTextStream>
#include <QtDebug>
#include <QFile>

#include "qbizmanager.h"
#include "twitter.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QString txt;
	switch (type)
	{
	case QtDebugMsg:     //调试信息提示
		txt = QString("%1 Debug: %2\r\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(msg);
		break;
	case QtWarningMsg:  //一般的warning提示
		//txt = QString("%1 Warning: %2\r\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(msg);
		break;
	case QtCriticalMsg:    //严重错误提示
		txt = QString("Critical: %1\r\n").arg(msg);
		break;
	case QtFatalMsg:      //致命错误提示
		txt = QString("Fatal: %1\r\n").arg(msg);
		abort();
	default:
		break;
	}

	QFile outFile(qApp->applicationDirPath() + "/" + QDateTime::currentDateTime().toString("yyyy-MM-dd") + ".txt");
	outFile.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream ts(&outFile);
	ts << txt << endl;
	outFile.close();
}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	qInstallMessageHandler(myMessageOutput);

	QBizManager::GetInstance().appendCookie(R"(__cfduid=d32fe440b549b863c04bfb8cb171d14311548517673; _ga=GA1.2.76270165.1548517752; language=8ad2f0e6c0f6cd9f5387ac8328a11816d806564808d2448f50e9d0a5a4aa0e81a%3A2%3A%7Bi%3A0%3Bs%3A8%3A%22language%22%3Bi%3A1%3Bs%3A5%3A%22en-US%22%3B%7D; language_id=en-US; cf_clearance=47cef5f8f8c215397644aff489c2b0056e0a0e54-1550496754-10800-150; PHPSESSID=bt9fektpetabngqdqjate9imk5; _csrf=ba9f361cf53563aa270a8c0533cd742223f0218d259852d57c741d10cffd2597a%3A2%3A%7Bi%3A0%3Bs%3A5%3A%22_csrf%22%3Bi%3A1%3Bs%3A32%3A%22%60%FA%D7%D3%5Dv%C0%CB%01c+%2F%C9%17%FFW%C9%28%16u%C6%81a%B6%87%28%F2%0B%03%8A%A4%88%22%3B%7D; _gid=GA1.2.263861107.1550496775; _u=383910; _s=5335ca5640656fc7b098a5cfd0274825; websocket_secure_key=4a30456e65afa4b42e701e7357e75d64; _gat=1)");;
	QBizManager::GetInstance().do_cf_clearance();
	
	Twitter twitter(0);	
	twitter.testmail();
	
	qDebug() << "qDebug test";	
	return a.exec();
}

