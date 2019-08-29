#include "qbizmanager.h"
#include "qhttpmanager.h"
#include <queue>
#include <QObject>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <QVariant>
#include <QString>

QBizManager::QBizManager( )
{
    bok = true;
    bhuobi = true;
    initDb();

    tnum =0;

	m_StringList.append(R"(will 2050 year Grin Grin Grin Grin Grin )");
	m_StringList.append(R"(today  now )");
	m_StringList.append(R"( tvt-doge not value tvt-btc only valu this coin fake bot scam coin. )");
	m_cookienum = 0;
}


//! [0]
bool QBizManager::initDb()
{
  
    return true;
}


QBizManager::~QBizManager()
{
}

bool QBizManager::CheckTime()
{

	QHttpManager::GetInstance().setCookie(m_cookieList.at(0));

	QString url = QString("https://bitcointalk.org/index.php?topic=2690972.new#new");
	QString web;
	for (int i = 0; i <= 3; i++)
	{

		QHttpManager::GetInstance().HttpGet_bitcointalk(url, web);
		if (!web.isEmpty())
		{
			break;
		}
	}

	QString num;
	{
		QString strfind = "Reply with quote";
		int p = web.lastIndexOf(strfind);
		p = web.indexOf("#", p + strfind.length() + 2);
		p = web.indexOf("#", p + 2);
		int p1 = web.indexOf("<", p +1);
		num = web.mid(p + 1, p1 - p - 1);
	}
	QString numa = QString::number(num.toDouble() - 10);

	{
		QString url = QString("https://bitcointalk.org/index.php?topic=2690972.%1").arg(numa);
		QString web;
		for (int i = 0; i <= 3; i++)
		{

			QHttpManager::GetInstance().HttpGet_bitcointalk(url, web);
			if (!web.isEmpty())
			{
				break;
			}
		}
	}

	QStringList QStringLi = web.split("View the profile of ");// +bttname;

	for (int i = 10; i < QStringLi.size()&& QStringLi.size()>=11; i++)
	{
		QString str = QStringLi.at(i); 
		QString strfind = bttname;
		int p = str.lastIndexOf(bttname);

		if (p != -1)
			return 1;
	}
	
	return 0;
}

void QBizManager::doPost()
{
	qDebug() << "doPost1";
//	if (Checkboard())
	//	return;
	qDebug() << "doPost12";
	if (CheckTime())
		return;

	qDebug() << "doPost223232";

	QString posturl;
	{

		QHttpManager::GetInstance().setCookie(m_cookieList.at(0));
		QString url = QString("https://bitcointalk.org/index.php?topic=2690972.new#new");

		QString web;
		for (int i = 0; i <= 3; i++)
		{

			QHttpManager::GetInstance().HttpGet_bitcointalk(url, web);
			if (!web.isEmpty())
			{
				break;
			}
			QEventLoop eventloop;
			QTimer::singleShot(5500, &eventloop, SLOT(quit()));
			eventloop.exec();
		}


		
		int pss = web.lastIndexOf("post;quote");
		pss = web.lastIndexOf("post;quote", pss - 10);
		int pfdsf = web.indexOf("><", pss);
		int pdaaad1 = web.lastIndexOf("https", pss);
		posturl = web.mid(pdaaad1, pfdsf - pdaaad1 - 1);

	}


		QHttpManager::GetInstance().setCookie(m_cookieList.at(0));

		QString url = posturl;// QString("https://bitcointalk.org/index.php?topic=2690972.new#new").arg(start);


		QString web;
		for (int i = 0; i <= 3; i++)
		{

			QHttpManager::GetInstance().HttpGet_bitcointalk(url, web);
			if (!web.isEmpty())
			{
				break;
			}
			QEventLoop eventloop;
			QTimer::singleShot(5500, &eventloop, SLOT(quit()));
			eventloop.exec();
		}


		if (web.indexOf(" new reply has been posted") != -1)
			return;

		if (web.indexOf("new replies have been posted") != -1)
			return;

		if (web.indexOf("This topic is locked") != -1)
			return;

		if (web.indexOf("he topic or board you are looking for appears to be either missing or off limits to you") != -1)
			return;

		if (web.indexOf("his topic has been moved to Trashcan") != -1)
			return;
	
		if (web.indexOf("normal_post_locked") != -1)
			return;


		QString strfind = "<input type=\"text\" name=\"subject\" value=";
		int p = web.indexOf(strfind);
		int p1 = web.indexOf("\"",p+strfind.length()+2);
		QString subject = web.mid(p + strfind.length()+1, p1 - p - strfind.length()-1);


		strfind = "<input type=\"hidden\" name=\"seqnum\" value=";
		p = web.indexOf(strfind);
		p1 = web.indexOf("\"", p + strfind.length() + 2);
		QString seqnum = web.mid(p + strfind.length()+1, p1 - p - strfind.length()-1);

		strfind = "<input type=\"hidden\" name=\"sc\" value=";
		p = web.indexOf(strfind);
		p1 = web.indexOf("\"", p + strfind.length() + 2);
		QString sc = web.mid(p + strfind.length() + 1, p1 - p - strfind.length() - 1);


		QString num_replies="0";
		strfind = "<input type=\"hidden\" name=\"num_replies\" value=";
		p = web.indexOf(strfind);
		if (p != -1)
		{

			p1 = web.indexOf("\"", p + strfind.length() + 2);
			num_replies = web.mid(p + strfind.length() + 1, p1 - p - strfind.length() - 1);

		}

		strfind = "X-UA-Compatible";
		p = web.indexOf(strfind);
		p1 = web.indexOf("board", p - strfind.length() - 102);
		QString board = web.mid(p1 + 6, p - p1 - 29);


		{
			strfind = "textarea class=\"editor\" name=\"message";
			p = web.indexOf(strfind);
			p = web.indexOf(">", p + strfind.length() + 2);
			p1 = web.indexOf("</textarea>", p + strfind.length() + 2);
			QString message = web.mid(p + 1, p1 - p - 1);
			m_StringList[0] = message;

		}
	

		QEventLoop eventloop;
		QTimer::singleShot(1000 * 6, &eventloop, SLOT(quit()));
		eventloop.exec();

		for (int i = 0; i <= 5; i++)
		{
			if (!bitcointalkPosttest_seqnum("2690972",subject,sc, num_replies,seqnum, board))
				break;
		}
		
		eventloop;
		QTimer::singleShot(1000*60*2, &eventloop, SLOT(quit()));
		eventloop.exec();

	
}



bool QBizManager::bitcointalkPosttest_seqnum(const QString& topic, const QString& subject, const QString& sc, const QString& num_replies, const QString& seqnum, const QString& board)
{
	QString bstr = QString::number(QDateTime::currentMSecsSinceEpoch()).toUtf8().toBase64();
	QString str = "------WebKitFormBoundary" + bstr.mid(1,16) + "\r\n";;//

    str = "------WebKitFormBoundaryYUwiL7xkzJ0CkiyA\r\n";
	QString post_data = "";
	post_data.append(str);
	post_data.append("Content-Disposition: form-data; name=\"topic\"\r\n\r\n");
	post_data.append(topic + "\r\n");

	post_data.append(str);
	post_data.append("Content-Disposition: form-data; name=\"subject\"\r\n\r\n");
	post_data.append(subject +"\r\n");

	post_data.append(str);
	post_data.append("Content-Disposition: form-data; name=\"icon\"\r\n\r\n");
	post_data.append("xx\r\n");

	int i = rand();
	QString msg = m_StringList.at(0) + "tvt is good ";
	post_data.append(str);
	post_data.append("Content-Disposition: form-data; name=\"message\"\r\n\r\n");
	post_data.append(msg + "\r\n");
	
	post_data.append(str);
	post_data.append("Content-Disposition: form-data; name=\"notify\"\r\n\r\n");
	post_data.append("0\r\n");

	post_data.append(str);
	post_data.append("Content-Disposition: form-data; name=\"do_watch\"\r\n\r\n");
	post_data.append("0\r\n");

	post_data.append(str);
	post_data.append("Content-Disposition: form-data; name=\"do_watch\"\r\n\r\n");
	post_data.append("1\r\n");

	post_data.append(str);
	post_data.append("Content-Disposition: form-data; name=\"goback\"\r\n\r\n");
	post_data.append("1\r\n");

	post_data.append(str);
	post_data.append("Content-Disposition: form-data; name=\"post\"\r\n\r\n");
	post_data.append("Post\r\n");

	post_data.append(str);
	post_data.append("Content-Disposition: form-data; name=\"num_replies\"\r\n\r\n");
	post_data.append(num_replies +"\r\n");

	post_data.append(str);
	post_data.append("Content-Disposition: form-data; name=\"additional_options\"\r\n\r\n");
	post_data.append("0\r\n");

	post_data.append(str);
	post_data.append("Content-Disposition: form-data; name=\"sc\"\r\n\r\n");
	post_data.append(sc + "\r\n");


	post_data.append(str);
	post_data.append("Content-Disposition: form-data; name=\"seqnum\"\r\n\r\n");
	post_data.append(seqnum + "\r\n");

	QString end = str;
	end = end.insert(str.length() - 2, "--");
	post_data.append(end);

	QString url = QString("https://bitcointalk.org/index.php?action=post2;start=0;board=%1").arg(board);
	QString source;
	QByteArray send;
	send.append(post_data);
	QHttpManager::GetInstance().HttpPost_bitcointalktest(url, send, str, source);

	if (source.indexOf("Please try to re-submit your message") != -1)
	{
		
		QEventLoop eventloop;
		QTimer::singleShot(3000, &eventloop, SLOT(quit()));
		eventloop.exec();
		return true;
	}

	if (source.indexOf("new replies have been posted") != -1)
	{

		QEventLoop eventloop;
		QTimer::singleShot(3000, &eventloop, SLOT(quit()));
		eventloop.exec();
		return true;
	}

	if (source.indexOf("was less than 360 seconds ago") != -1)
	{

		QEventLoop eventloop;
		QTimer::singleShot(1000 * 60 * 1, &eventloop, SLOT(quit()));
		eventloop.exec();
		return true;
	}
	return false;
}

void QBizManager::appendCookie(const QString& texts)
{
	QStringList sl = texts.split("\n");
	for (int i = 0; i < sl.size(); i++)
	{
		QString str = sl.at(i);
		if (str.isEmpty())
			continue;
		str = str.trimmed();
		m_cookieList.append(str);
	}
	// "SMFCookie129=a%3A4%3A%7Bi%3A0%3Bs%3A6%3A%22986828%22%3Bi%3A1%3Bs%3A40%3A%228c6a3ab7616a6b91f0f5c40c8e023c47704f3b5b%22%3Bi%3A2%3Bi%3A1506492326%3Bi%3A3%3Bi%3A0%3B%7D; PHPSESSID=lvnnlvljdn2hk88ho1fa4gs295");

}



bool QBizManager::Checkboard()
{
	qDebug() << "Checkboard 1" << m_cookieList.size();
	QHttpManager::GetInstance().setCookie(m_cookieList.at(0));
	QString url = QString("https://bitcointalk.org/index.php?board=159.0");
	QString web;
	for (int i = 0; i <= 3; i++)
	{

		QHttpManager::GetInstance().HttpGet_bitcointalk(url, web);
		if (!web.isEmpty())
		{
			break;
		}
	}

	QStringList QStrinlist = web.split("https://bitcointalk.org/Themes/custom1/images/top");

	if (QStrinlist.size() == 0)
		return 0;
	QStrinlist.removeFirst();

	qDebug() << "Checkboard"<<QStrinlist.size();

	int num = 1000;
	for (int i = 0; i < QStrinlist.size(); i++)
	{
		QString str = QStrinlist.at(i);

		QString strfind = "TVT";
		int p = str.lastIndexOf(strfind);

		if (p != -1)
		{
			num = i;
			break;
		}
	}

	if(num<=15)
		return 1;
	return 0;
}