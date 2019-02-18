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

	QBizManager::GetInstance().appendCookie(R"(__cfduid=dcd71745a264b4b002567ee878f8d5fb51548822274; language=8ad2f0e6c0f6cd9f5387ac8328a11816d806564808d2448f50e9d0a5a4aa0e81a%3A2%3A%7Bi%3A0%3Bs%3A8%3A%22language%22%3Bi%3A1%3Bs%3A5%3A%22en-US%22%3B%7D; language_id=en-US; _ga=GA1.2.1990910632.1548822294; cf_clearance=d742c7ec0fec43d9b04805d2b6a381ac8005ff06-1550481699-10800-150; _gid=GA1.2.172559651.1550481707; PHPSESSID=rukg7amhet0n2kbc8kohi6d5q5; _csrf=57a222e2182ac168889231291ecc23ba2f0063b9e4ada986428d6462568426c8a%3A2%3A%7Bi%3A0%3Bs%3A5%3A%22_csrf%22%3Bi%3A1%3Bs%3A32%3A%22%22%CA%AA%8B%06N8i%86%24%B0%BD%2AC%83%C6%A2%0D%90%B3%5D%D5I%D5%DB%81%A4%26%C76%AB_%22%3B%7D; _u=383910; _s=5335ca5640656fc7b098a5cfd0274825; websocket_secure_key=26ee868e56641751fafc9e61cbf9af31; _gat=1)");;
	QBizManager::GetInstance().do_cf_clearance();
	
	Twitter twitter(0);	
	twitter.testmail();
	
	qDebug() << "qDebug test";	
	return a.exec();
}

