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

    QBizManager::GetInstance().appendCookie(R"(language_id=en-US; language=8ad2f0e6c0f6cd9f5387ac8328a11816d806564808d2448f50e9d0a5a4aa0e81a%3A2%3A%7Bi%3A0%3Bs%3A8%3A%22language%22%3Bi%3A1%3Bs%3A5%3A%22en-US%22%3B%7D; language_id=en-US; _ga=GA1.2.1990910632.1548822294; _gid=GA1.2.172559651.1550481707; __cfduid=; PHPSESSID=gptsvv6s09s7pjs06nbv43urc2; _csrf=0de5802a2153184f1b73a6c740dd1d4fea28aa9cfdcc34add7f2933b77cae536a%3A2%3A%7Bi%3A0%3Bs%3A5%3A%22_csrf%22%3Bi%3A1%3Bs%3A32%3A%22%07%1F%E1On%9B%EF%130Um%92%03%7D%C4%C0%95%F0%0D%CF%FB%BDr%C7%5Cax%3D%F9i%FA%D8%22%3B%7D; cf_clearance=fd96717bb60314e2a245292a54572629526c29c3-1550591214-10800-150; _u=383910; _s=5335ca5640656fc7b098a5cfd0274825; websocket_secure_key=c7995ae74273cfb00d704a46c7ee4eb8; _gat=1)");
    QBizManager::GetInstance().do_cf_clearance();

    Twitter twitter(0);
    twitter.testmail();

    qDebug() << "qDebug test";
    return a.exec();
}

