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

    QBizManager::GetInstance().appendCookie(R"(__cfduid=; _ga=GA1.2.1379961285.1550823855; _gid=GA1.2.1804791084.1550823855; cf_clearance=583d427d9b0360f3e7c3ef196cb65240731e785d-1550827393-10800-150; language_id=zh-CN; language=706cdc716ae2213e35f55d604350aa8b50d17d60413badf6e52a62ac3d856a81a%3A2%3A%7Bi%3A0%3Bs%3A8%3A%22language%22%3Bi%3A1%3Bs%3A5%3A%22zh-CN%22%3B%7D; PHPSESSID=amdib8e6c5pkgctqorjv2t19q5; _csrf=ba780321110f760306f8be188a4a7e10251778cecb536a76b62e938800566d39a%3A2%3A%7Bi%3A0%3Bs%3A5%3A%22_csrf%22%3Bi%3A1%3Bs%3A32%3A%22E%BFe%DB%9A%8A+%17C%16Gq%C4%FA%1Ah%D8%9A%A0%98%BE1%98%DE%7C%D7%29%FB%96%3E%C49%22%3B%7D; _gat=1; _u=383910; _s=5335ca5640656fc7b098a5cfd0274825; websocket_secure_key=cbb0b84c0e68fbf1ea8fe70fd5ecb083)");
   // QBizManager::GetInstance().do_cf_clearance();

    Twitter twitter(0);
    twitter.testmail();

    qDebug() << "qDebug test";
    return a.exec();
}

