#include <QtCore/QCoreApplication>
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

	QBizManager::GetInstance().appendCookie(R"(language_id=en-US; __cfduid=dcd71745a264b4b002567ee878f8d5fb51548822274; language=8ad2f0e6c0f6cd9f5387ac8328a11816d806564808d2448f50e9d0a5a4aa0e81a%3A2%3A%7Bi%3A0%3Bs%3A8%3A%22language%22%3Bi%3A1%3Bs%3A5%3A%22en-US%22%3B%7D; language_id=en-US; _ga=GA1.2.1990910632.1548822294; _gid=GA1.2.172559651.1550481707; cf_clearance=73a14ff5412649380811105704f2d58b7e5df568-1550548980-10800-150; PHPSESSID=7udh0ujpbleb6lrj5s2pe5mhp2; _csrf=4122a413fe921fdd3b85756978b9bfda5af33720f45ae134be2c60aa2132e2e0a%3A2%3A%7Bi%3A0%3Bs%3A5%3A%22_csrf%22%3Bi%3A1%3Bs%3A32%3A%22%1F-%3A%08%01%B7%B4%C5%EB%C0%84%D9%28%89%A7%9B%B8%F2a%5Ec%9C%1A%AFh%EB0.%9FEk%0E%22%3B%7D; _u=383910; _s=5335ca5640656fc7b098a5cfd0274825; websocket_secure_key=b73b8dc95aa28f00a0f2b3ad655d848d
)");;
	QBizManager::GetInstance().do_cf_clearance();
	
	Twitter twitter(0);	
	twitter.testmail();
	
	qDebug() << "qDebug test";	
	return a.exec();
}

