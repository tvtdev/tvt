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


QBizManager::QBizManager( )
{
    bok = true;
    bhuobi = true;
  //  GetBalance(m_Balance);

    initDb();

    tnum =0;

	m_StringList.append(R"( [quote author=GreatTHINKER link=topic=3212532.msg40003429#msg40003429 date=1528867917]
[b]Week : 11 [/b]
Date : (07/06 - 13/06)

Twitter 
Twitter Link : https://twitter.com/aidenfrankie	

Retweet & Like : 

1. https://twitter.com/Triggmine/status/1004280152635920384
2. https://twitter.com/Triggmine/status/1001386039171276800
3. https://twitter.com/Triggmine/status/1000014269834985472
4. https://twitter.com/Triggmine/status/998949161369251840
5. https://twitter.com/Triggmine/status/998948803804876800 )");



	m_StringList.append(R"( Blog: https://icomais.com/destaque/hussy/
User: icomais.com
ETH: 0x70363A714F32575aAB4cd5972A738e14b73f50b8 
Bitcointalk: https://bitcointalk.org/index.php?action=profile;u=1102196
telegram: moedasdofuturo )");


	m_StringList.append(R"( 

	-- - Proof of Authentication-- -
		#Joined Twitter Campaign
		_______________________
		Username : @johnokosun
		Twitter Account : https://twitter.com/johnokosun
	Twitter Follower : 2481

 )");



	
	
	m_cookieList.append("__cfduid=d5381fbd5821b76f56265c4e036eb00231528861346; SMFCookie129=a%3A4%3A%7Bi%3A0%3Bs%3A7%3A%221169050%22%3Bi%3A1%3Bs%3A40%3A%2260bd1c0fe9de78d493d813a24d95316d028485df%22%3Bi%3A2%3Bi%3A1718077687%3Bi%3A3%3Bi%3A0%3B%7D; PHPSESSID=ea904vp6a70sf3vjsns1uu8ll0");
	m_cookieList.append("__cfduid=dffd55961d70e3686fbaae9b0c6f359921512108285; SMFCookie129=a%3A4%3A%7Bi%3A0%3Bs%3A7%3A%221145707%22%3Bi%3A1%3Bs%3A40%3A%227c876b6ef4adb674df22cb245b0687d3c5a3e83e%22%3Bi%3A2%3Bi%3A1718076991%3Bi%3A3%3Bi%3A0%3B%7D; PHPSESSID=676te419num0unjb037vdkpil4");
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

#include <QSet>

void QBizManager::doPost()
{
	while (true)
	{
	
		QString web;
		for (int i = 0; i <= 3; i++)
		{

			QHttpManager::GetInstance().HttpGet("https://bitcointalk.org/index.php?board=238.0", web);
			if (!web.isEmpty())
			{
				break;
			}
			QEventLoop eventloop;
			QTimer::singleShot(5500, &eventloop, SLOT(quit()));
			eventloop.exec();
		}


		if (web.indexOf("TVT TOKEN BOUN") != -1)
			continue;
		

		QString topic_url = "https://bitcointalk.org/index.php?action=post;topic=3351900.0;num_replies=0";
		//	QString cookie = "__cfduid=d5381fbd5821b76f56265c4e036eb00231528861346; SMFCookie129=a%3A4%3A%7Bi%3A0%3Bs%3A7%3A%221169050%22%3Bi%3A1%3Bs%3A40%3A%2260bd1c0fe9de78d493d813a24d95316d028485df%22%3Bi%3A2%3Bi%3A1718077687%3Bi%3A3%3Bi%3A0%3B%7D; PHPSESSID=ea904vp6a70sf3vjsns1uu8ll0";
		//	QHttpManager::GetInstance().setCookie(cookie);
		QHttpManager::GetInstance().setCookie(m_cookieList.at(m_cookienum));
		
		for (int i = 0; i <= 2; i++)
		{
			QHttpManager::GetInstance().HttpGet_bitcointalk(topic_url, web);
			if (!web.isEmpty())
			{
				break;
			}
			QEventLoop eventloop;
			QTimer::singleShot(5500, &eventloop, SLOT(quit()));
			eventloop.exec();
		}

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


		strfind = "<input type=\"hidden\" name=\"num_replies\" value=";
		p = web.indexOf(strfind);
		p1 = web.indexOf("\"", p + strfind.length() + 2);
		QString num_replies = web.mid(p + strfind.length() + 1, p1 - p - strfind.length() - 1);
		if (num_replies.length() > 20)
			continue;

		QEventLoop eventloop;
		QTimer::singleShot(1000 * 6, &eventloop, SLOT(quit()));
		eventloop.exec();

		for (int i = 0; i <= 5; i++)
		{
			if (!bitcointalkPosttest_seqnum("3351900",subject,sc, num_replies,seqnum))
				break;
		}
		
		eventloop;
		QTimer::singleShot(5500, &eventloop, SLOT(quit()));
		eventloop.exec();

	}
}



bool QBizManager::bitcointalkPosttest_seqnum(const QString& topic, const QString& subject, const QString& sc, const QString& num_replies, const QString& seqnum)
{
	QString bstr = QString::number(QDateTime::currentMSecsSinceEpoch()).toUtf8().toBase64();
	;// QString bsta = "15t9cba8pT5n6Kar";
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
	QString msg = m_StringList.at(i % m_StringList.size());
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


	QString source;
	QByteArray send;
	send.append(post_data);
	QHttpManager::GetInstance().HttpPost_bitcointalktest("https://bitcointalk.org/index.php?action=post2;start=0;board=159", send, str, source);

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

	if (source.length() < 10)
	{
		m_cookienum++;

		if (m_cookienum > m_cookieList.size() - 1)
		{
			m_cookienum = 0;
			;// exit(0);
		}
			
		QEventLoop eventloop;
		QTimer::singleShot(1000 * 50 * 2, &eventloop, SLOT(quit()));
		eventloop.exec();
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