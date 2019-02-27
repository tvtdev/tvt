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

void copeok()
{
	qDebug() << "main: ------copy" << endl;

	for (size_t i = 0; i < 3; i++)
	{	
		QFile outFile(qApp->applicationDirPath() + "/cok");
		outFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
		QTextStream ts(&outFile);

		QFile file(qApp->applicationDirPath() + "/cok2");
		if (file.open(QIODevice::ReadOnly)) {
			QTextStream in(&file);
			while (!in.atEnd())
			{
				QString line = in.readLine();
				ts << line << endl;
			}
		}
		outFile.close();
		file.close();

		QEventLoop loop;
		QTimer::singleShot(3000, &loop, SLOT(quit()));
		loop.exec();

	}
	return;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
	qInstallMessageHandler(myMessageOutput);
	
	int bbb = 2;
	QFile file(qApp->applicationDirPath() + "/cok");
	if (file.open(QIODevice::ReadOnly)) {
		QTextStream in(&file);
		while (!in.atEnd()) {
			QString line = in.readLine();
			if (line.length() > 50)
			{
				qDebug() << "main: ------appendCookie" << endl;
				bbb = 1;
				QBizManager::GetInstance().appendCookie(line);
			}
		}
	}
	file.close();

	qDebug() << "qDebuga test";
	if (bbb == 2)
	{	copeok();
	}
	
	Twitter twitter(0);
	QString out;
	for (size_t i = 0; i < 3; i++)
	{
		QBizManager::GetInstance().do_cf_clearance();
		int ret = twitter.DoTestMail(out);
		if (ret == 5)
		{
			qDebug() << "DoTestMail test error" << out;
			return 0;
		}

	    if (ret ==1)
			break;
	}
	
	if (out.isEmpty())
	{
		qDebug() << "DoTestMail test error";
		return 0;
	}

    twitter.testmail();

    qDebug() << "qDebug test";
    return a.exec();
}

