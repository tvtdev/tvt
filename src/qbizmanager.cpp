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

	m_StringList.append(R"(
	mercatox.com/exchange/TVT/E
TVT Airdrop #1

Airdrop 100000tvt each
1. Follow @tvtio2
2. Like and Retweet
3. Tag 5 friends
4. Comment Mercatox E-mail or E-Wallet ID 

#airdrop #freetoken #token #ETH  #Crypto  #BTC #bitcoin #ethereum #blockchain

https://twitter.com/tvtio2/status/1092806072757338114?s=21
	
	)");



	




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
	int start = 0;
	QHttpManager::GetInstance().setCookie(m_cookieList.at(0));
	
	while(1)
	{
		if(1)
		{
			QString url = QString("https://bitcointalk.org/index.php?board=238.0");
	
			QString web; 
			QHttpManager::GetInstance().HttpGet_bitcointalk(url, web);
        		if (web.length() < 600)
			{
				break;
			}
			QString	strfind = "View the profile of tvt.io";
			int p = web.indexOf(strfind);
			if(p!=-1)
			{
				qDebug()<<"fin a ad";
	                        continue;
			}
			QEventLoop eventloop;
		        QTimer::singleShot(1000 * 30, &eventloop, SLOT(quit()));
		        eventloop.exec();
	
		}
		
		
		for(int i=0;i<=3;i++)
		{
			QString url = QString("https://bitcointalk.org/index.php?topic=3351900.new#new");
	
			QString web; 
			QHttpManager::GetInstance().HttpGet_bitcointalk(url, web);
			qDebug()<<web;
                        QString	strfind = "deletemsg";
			int p = web.indexOf(strfind);
			if(p==-1)
			{
	                       continue;
			}
			
			int p1 = web.indexOf("\"", p + strfind.length() + 2);
			QString deleteurl = web.mid(p + strfind.length()+1-51, p1 - p - strfind.length()-1+51);
                        {
	                	QString web; 
		                QHttpManager::GetInstance().HttpGet_bitcointalk(deleteurl, web);
        	                if (web.length() > 1000)
		                {
			                break;
				}
		        }
	
		}
	
		
		{
		QString url = QString("https://bitcointalk.org/index.php?action=post;topic=3351900.0;num_replies=0");
	
		QString web; 
		QHttpManager::GetInstance().HttpGet_bitcointalk(url, web);
        	if (web.length() < 1000)
		{
			break;
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

		if (num_replies.length() > 20)
			continue;

		QEventLoop eventloop;
		QTimer::singleShot(1000 * 6, &eventloop, SLOT(quit()));
		eventloop.exec();

			
		qDebug()<<board <<sc<<seqnum;
		for (int i = 0; i <= 5; i++)
		{
			if (!bitcointalkPosttest_seqnum(QString("%1").arg(3351900),subject,sc, num_replies,seqnum, board))
				break;
		}
		
		eventloop;
		QTimer::singleShot(1000*60*3, &eventloop, SLOT(quit()));
		eventloop.exec();
		}

	}
}



bool QBizManager::bitcointalkPosttest_seqnum(const QString& topic, const QString& subject, const QString& sc, const QString& num_replies, const QString& seqnum, const QString& board)
{
	QString bstr = QString::number(QDateTime::currentMSecsSinceEpoch()).toUtf8().toBase64();
	;// QString bsta = "15t9cba8pT5n6Kar";
	QString str = "------WebKitFormBoundary" + bstr.mid(1,16) + "\r\n";;//

    str = "------WebKitFormBoundarybAxG6QCUvDGMDAJM\r\n";
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

	QString url = QString("https://bitcointalk.org/index.php?action=post2;start=0;board=%1").arg(board);
	QString source;
	QByteArray send;
	send.append(post_data);
	QHttpManager::GetInstance().HttpPost_bitcointalktest(url, send, str, source);

	qDebug()<<post_data;
	
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
		qDebug()<<"ad";
		
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
}

