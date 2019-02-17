#include <QtCore/QCoreApplication>
#include <QTextStream>
#include "qbizmanager.h"
#include <QtDebug>
#include <QFile>
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

	QBizManager _webSocket(0);
	//_webSocket.appendCookie(R"(moments_profile_moments_nav_tooltip_self=true; moments_user_moment_profile_create_moment_tooltip=true; moments_moment_maker_more_menu_tooltip=true; netpu="Fp77o9bDVgA="; twitter_ads_id=v1_88121527954130688; moments_moment_guide_create_moment_tooltip=true; moments_profile_moments_nav_tooltip_other=true; co=us; kdt=JBWNOKMHO5zm4rMdk4i5YArtJzdEb42uKPuA5ytq; remember_checked_on=1; csrf_same_site_set=1; csrf_same_site=1; _ga=GA1.2.1460846950.1477574521; __utma=43838368.1460846950.1477574521.1515126634.1537860180.10; __utmz=43838368.1537860180.10.1.utmcsr=(direct)|utmccn=(direct)|utmcmd=(none); personalization_id="v1_tLF/Fm3UTWJHErK7amXI0w=="; guest_id=v1%3A154329995373823988; ads_prefs="HBERAAA="; twid="u=817737154482708480"; lang=zh-cn; ct0=db7d465915c65ee1552a29dbddd230de; _gid=GA1.2.709453843.1548736181; external_referer=padhuUp37zietkZA1RAYtSsa%2FW6K9sB713mUQdWBp4E%3D|0|8e8t2xd8A2w%3D; _twitter_sess=BAh7CiIKZmxhc2hJQzonQWN0aW9uQ29udHJvbGxlcjo6Rmxhc2g6OkZsYXNo%250ASGFzaHsABjoKQHVzZWR7ADoPY3JlYXRlZF9hdGwrCLfLkZJoAToHaWQiJTQw%250AYWQ2OGU5NDdhZjM4ZGEzOWIyZWU5YTRmYjI1YmQ2Ogxjc3JmX2lkIiU2ZjBj%250AZDkwNmUxNzY3NDJjZTYxNDY5NzllYjZlYWVkMjoJdXNlcmwrCQCglAScL1kL--8e0fc3712f30b2e59636c4269f9af6b1e6864ce0; auth_token=904c52cb34514ea230d544efb016539b169a7384; _gat=1)");
	_webSocket.appendCookie(R"(__cfduid=dcd71745a264b4b002567ee878f8d5fb51548822274; cf_clearance=bd7f4edb2841c1b337042555e3c52314520a1ba2-1548822284-10800-150; PHPSESSID=mo9e0bvquhfmuff8ek5j1652m0; language=8ad2f0e6c0f6cd9f5387ac8328a11816d806564808d2448f50e9d0a5a4aa0e81a%3A2%3A%7Bi%3A0%3Bs%3A8%3A%22language%22%3Bi%3A1%3Bs%3A5%3A%22en-US%22%3B%7D; _csrf=7eba384496df32a523989360e367857c44f625c56262f194a5954658d4299cf5a%3A2%3A%7Bi%3A0%3Bs%3A5%3A%22_csrf%22%3Bi%3A1%3Bs%3A32%3A%22%AE%EA%D5%93%5D%B5%29%F6%B6%5B%5Dcym%1A%A1%18%7FBx%AA%13x%EB%A0%7E%40%0B%CE%DB%EE%EF%22%3B%7D; language_id=en-US; _ga=GA1.2.1990910632.1548822294; _gid=GA1.2.1473442867.1548822294; _gat=1; _u=383910; _s=5335ca5640656fc7b098a5cfd0274825; websocket_secure_key=057c4ce788183cc26bdf222bcec9ab9c)");;
	
	//_webSocket.testmail();

	
	qDebug() << "qDebug test";
	
	QEventLoop loop;
	QTimer::singleShot(6000, &loop, SLOT(quit()));
	loop.exec();
	return a.exec();
}

