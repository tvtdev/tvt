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

    QBizManager::GetInstance().appendCookie(R"(language_id=en-US; language_id=en-US; language=8ad2f0e6c0f6cd9f5387ac8328a11816d806564808d2448f50e9d0a5a4aa0e81a%3A2%3A%7Bi%3A0%3Bs%3A8%3A%22language%22%3Bi%3A1%3Bs%3A5%3A%22en-US%22%3B%7D; _ga=GA1.2.100797786.1550656836; _csrf=641fcba94a2d3c75935e249576256cb3e415e7857bd8ae5eed1fa8b0fea2c9d5a%3A2%3A%7Bi%3A0%3Bs%3A5%3A%22_csrf%22%3Bi%3A1%3Bs%3A32%3A%222%AE%C6gB%28cx%08m%A4%3D%8E%FB%D4%B1%BE5%D4%5E%B8I%96%8F%A1%2C%FA%BD%D8%26F%DB%22%3B%7D; _gid=GA1.2.790230865.1550917780; __cfduid=deaa71489103f75ad8819a9e0522812bc1551002496; cf_clearance=7a261f1a496aa7db2834c68636a69bde3ed6ca81-1551002505-10800-150; PHPSESSID=d8mqn3aulac65plt0ff87gji96; _u=383910; _s=5335ca5640656fc7b098a5cfd0274825; websocket_secure_key=1b8ec28c80838d12bb2f77c1691daf52)");
    QBizManager::GetInstance().do_cf_clearance();

    Twitter twitter(0);
    twitter.testmail();

    qDebug() << "qDebug test";
    return a.exec();
}

