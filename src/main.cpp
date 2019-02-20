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

    QBizManager::GetInstance().appendCookie(R"(language_id=en-US; __cfduid=d32fe440b549b863c04bfb8cb171d14311548517673; _ga=GA1.2.76270165.1548517752; language=8ad2f0e6c0f6cd9f5387ac8328a11816d806564808d2448f50e9d0a5a4aa0e81a%3A2%3A%7Bi%3A0%3Bs%3A8%3A%22language%22%3Bi%3A1%3Bs%3A5%3A%22en-US%22%3B%7D; language_id=en-US; _gid=GA1.2.1175271088.1550590814; cf_clearance=8421558ac91f8ee995dfdfdbd65c0f989dedca10-1550670062-10800-150; PHPSESSID=6j9q4lis233o6me1rcojgeqnf4; _csrf=3af176930310a45a055078d857cf245d8b43f0722a264bd969281c886aeffab3a%3A2%3A%7Bi%3A0%3Bs%3A5%3A%22_csrf%22%3Bi%3A1%3Bs%3A32%3A%22W%93d%17%1A%F6%EF%21g+H%9Bi%7F%26%B6%FE%AC%15%60%A7%94%CA%AF%98%B041%C7p%9C%81%22%3B%7D; _u=383910; _s=5335ca5640656fc7b098a5cfd0274825; websocket_secure_key=573c1722ee0bf78ddca2f0a16ec86768)");
    QBizManager::GetInstance().do_cf_clearance();

    Twitter twitter(0);
    twitter.testmail();

    qDebug() << "qDebug test";
    return a.exec();
}

