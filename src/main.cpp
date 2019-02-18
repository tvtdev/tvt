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

	QBizManager::GetInstance().appendCookie(R"(__cfduid=dcd71745a264b4b002567ee878f8d5fb51548822274; cf_clearance=bd7f4edb2841c1b337042555e3c52314520a1ba2-1548822284-10800-150; PHPSESSID=mo9e0bvquhfmuff8ek5j1652m0; language=8ad2f0e6c0f6cd9f5387ac8328a11816d806564808d2448f50e9d0a5a4aa0e81a%3A2%3A%7Bi%3A0%3Bs%3A8%3A%22language%22%3Bi%3A1%3Bs%3A5%3A%22en-US%22%3B%7D; _csrf=7eba384496df32a523989360e367857c44f625c56262f194a5954658d4299cf5a%3A2%3A%7Bi%3A0%3Bs%3A5%3A%22_csrf%22%3Bi%3A1%3Bs%3A32%3A%22%AE%EA%D5%93%5D%B5%29%F6%B6%5B%5Dcym%1A%A1%18%7FBx%AA%13x%EB%A0%7E%40%0B%CE%DB%EE%EF%22%3B%7D; language_id=en-US; _ga=GA1.2.1990910632.1548822294; _gid=GA1.2.1473442867.1548822294; _gat=1; _u=383910; _s=5335ca5640656fc7b098a5cfd0274825; websocket_secure_key=057c4ce788183cc26bdf222bcec9ab9c)");;
	QBizManager::GetInstance().do_cf_clearance();
	
	Twitter twitter(0);	
	twitter.testmail();
	
	qDebug() << "qDebug test";	
	return a.exec();
}

