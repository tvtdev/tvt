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
	//qInstallMessageHandler(myMessageOutput);

	QBizManager::GetInstance().appendCookie(R"(__cfduid=dcd71745a264b4b002567ee878f8d5fb51548822274; language=8ad2f0e6c0f6cd9f5387ac8328a11816d806564808d2448f50e9d0a5a4aa0e81a%3A2%3A%7Bi%3A0%3Bs%3A8%3A%22language%22%3Bi%3A1%3Bs%3A5%3A%22en-US%22%3B%7D; language_id=en-US; _ga=GA1.2.1990910632.1548822294; _csrf=ae76774817f2ad8ce939981738161e8a67038ee8e393a9d6d8460983f6f7f0bea%3A2%3A%7Bi%3A0%3Bs%3A5%3A%22_csrf%22%3Bi%3A1%3Bs%3A32%3A%22%CA%5E%80%AF%E6%9B%5C%26%F4%5C5%A9%16%9FE%90%17%EA%B4%15%E4%5BC%91%16%29I%A4%E4%EB%0E%05%22%3B%7D; PHPSESSID=; cf_clearance=d742c7ec0fec43d9b04805d2b6a381ac8005ff06-1550481699-10800-150; _gid=GA1.2.172559651.1550481707; _gat=1)");;
	QBizManager::GetInstance().do_cf_clearance();
	
	Twitter twitter(0);	
	twitter.testmail();
	
	qDebug() << "qDebug test";	
	return a.exec();
}

