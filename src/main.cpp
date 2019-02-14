#include <QtCore/QCoreApplication>
#include <QTextStream>
#include "qbizmanager.h"
#include <QtDebug>
#include <QFile>

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


	}
	return;
}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	qInstallMessageHandler(myMessageOutput);
	QBizManager _webSocket(0);
	_webSocket.appendCookie(R"(moments_profile_moments_nav_tooltip_self=true; moments_user_moment_profile_create_moment_tooltip=true; moments_moment_maker_more_menu_tooltip=true; netpu="Fp77o9bDVgA="; twitter_ads_id=v1_88121527954130688; moments_moment_guide_create_moment_tooltip=true; moments_profile_moments_nav_tooltip_other=true; co=us; kdt=JBWNOKMHO5zm4rMdk4i5YArtJzdEb42uKPuA5ytq; remember_checked_on=1; csrf_same_site_set=1; csrf_same_site=1; _ga=GA1.2.1460846950.1477574521; __utma=43838368.1460846950.1477574521.1515126634.1537860180.10; __utmz=43838368.1537860180.10.1.utmcsr=(direct)|utmccn=(direct)|utmcmd=(none); personalization_id="v1_tLF/Fm3UTWJHErK7amXI0w=="; guest_id=v1%3A154329995373823988; ads_prefs="HBERAAA="; twid="u=817737154482708480"; lang=zh-cn; ct0=db7d465915c65ee1552a29dbddd230de; _gid=GA1.2.709453843.1548736181; external_referer=padhuUp37zietkZA1RAYtSsa%2FW6K9sB713mUQdWBp4E%3D|0|8e8t2xd8A2w%3D; _twitter_sess=BAh7CiIKZmxhc2hJQzonQWN0aW9uQ29udHJvbGxlcjo6Rmxhc2g6OkZsYXNo%250ASGFzaHsABjoKQHVzZWR7ADoPY3JlYXRlZF9hdGwrCLfLkZJoAToHaWQiJTQw%250AYWQ2OGU5NDdhZjM4ZGEzOWIyZWU5YTRmYjI1YmQ2Ogxjc3JmX2lkIiU2ZjBj%250AZDkwNmUxNzY3NDJjZTYxNDY5NzllYjZlYWVkMjoJdXNlcmwrCQCglAScL1kL--8e0fc3712f30b2e59636c4269f9af6b1e6864ce0; auth_token=904c52cb34514ea230d544efb016539b169a7384; _gat=1)");
	
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
				_webSocket.appendCookie(line);
			}
		
		}
	}
        file.close();

	if (bbb == 2)
	{
		copeok();
	}	
	qDebug() << "main: ------do_cf_clearance" << endl;




	
	
	_webSocket.sendmx();
	qDebug() << "qDebug test";
	
	QEventLoop loop;
	QTimer::singleShot(6000, &loop, SLOT(quit()));
	loop.exec();
	return a.exec();
}

