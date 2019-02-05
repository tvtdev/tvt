

#include <QtCore/QCoreApplication>
#include <QTextStream>
#include "qbizmanager.h"




void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
        QString txt;
        switch (type)
        {
        case QtDebugMsg:     //调试信息提示
                txt = QString("%1 Debug: %2\r\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(msg);
                break;
        case QtWarningMsg:  //一般的warning提示
                txt = QString("%1 Warning: %2\r\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(msg);
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

        QFile outFile(qApp->applicationDirPath()+"/"+QDateTime::currentDateTime().toString("yyyy-MM-dd") + ".txt");
        outFile.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream ts(&outFile);
        ts << txt << endl;
        outFile.close();
}







int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
qInstallMessageHandler(myMessageOutput);




	QBizManager _webSocket;

//cookie: __cfduid=da81496f63227c5d141955fcc481427b91548822121; SMFCookie129=a%3A4%3A%7Bi%3A0%3Bs%3A7%3A%221095172%22%3Bi%3A1%3Bs%3A40%3A%22f0e52a6ec96085f6d3d22698aa269324d1f8d891%22%3Bi%3A2%3Bi%3A1738222454%3Bi%3A3%3Bi%3A0%3B%7D; PHPSESSID=4kfodd443lc872alf3p9g25u56

_webSocket.appendCookie("__cfduid=da81496f63227c5d141955fcc481427b91548822121; SMFCookie129=a%3A4%3A%7Bi%3A0%3Bs%3A7%3A%221095172%22%3Bi%3A1%3Bs%3A40%3A%22f0e52a6ec96085f6d3d22698aa269324d1f8d891%22%3Bi%3A2%3Bi%3A1738222454%3Bi%3A3%3Bi%3A0%3B%7D; PHPSESSID=4kfodd443lc872alf3p9g25u56");
	
	//_webSocket.appendCookie("__cfduid=da81496f63227c5d141955fcc481427b91548822121; SMFCookie129=a%3A4%3A%7Bi%3A0%3Bs%3A7%3A%221095172%22%3Bi%3A1%3Bs%3A40%3A%22f0e52a6ec96085f6d3d22698aa269324d1f8d891%22%3Bi%3A2%3Bi%3A1738222454%3Bi%3A3%3Bi%3A0%3B%7D; PHPSESSID=4kfodd443lc872alf3//p9g25u56");
	
	_webSocket.doPost();
	


	
	QEventLoop loop;
	QTimer::singleShot(6000, &loop, SLOT(quit()));
	loop.exec();


	


	return a.exec();
}
