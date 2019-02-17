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
#include "twitter.h"
#include <QtScript/QtScript>



#define SIZE_THRESHOLD 56
#define OFFSET_SHORT_LIST 0xc0
#define OFFSET_LONG_LIST 0xf7
#define OFFSET_LONG_ITEM  0xb7
#define OFFSET_SHORT_ITEM 0x80
// (nullptr);
QBizManager::QBizManager(QObject *parent) : QObject(parent)
{
	m_gas_add = 1;
	m_cookienum = 0;
	m_startnonce = 17;
	m_signature_r = 38;

	_QImap =new QImap(nullptr);
	_QImap->connectToServer("imap.gmail.com", 993);
	_QImap->login("rahmirra12@gmail.com", "tvt@@@111111");


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

void QBizManager::do_cf_clearance()
{
	for (int i = 0; i < m_cookieList.size(); i++)
	{
		QString strcokk = m_cookieList.at(i);
		QString res;

		for (size_t nn = 0; nn < 3; nn++)
		{
			Get_cf_clearance(strcokk, res);
			if (res.length() < 900)
				break;
		}
		m_cookieList[i] = res;
	}
}


void QBizManager::sendmx()
{
	if(0)
	{
		QString strcokk = m_cookieList.at(1);
		QString res = strcokk;

		for (size_t i = 0; i < 5; i++)
		{
			Get_cf_clearance(strcokk, res);
			if (res.length() < 900)
				break;
		}
		m_cookieList[1] = res;
	}

	QHttpManager::GetInstance().setCookie(m_cookieList.at(0));
	QString url = QString("https://twitter.com/tvt_io/status/1088288005754769408");

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

	QString authenticity_token;
	{
		QString strfind = "name=\"authenticity_token\" value=\"";
		int p = web.indexOf(strfind);
		int p1 = web.indexOf("\"", p + strfind.length() + 2);
		authenticity_token = web.mid(p + strfind.length(), p1 - p - strfind.length());
	}
	if (authenticity_token.length()!=40)
	{
		qDebug() << "authenticity_token error";
		return;
	}
	qDebug() << "authenticity_token:" << authenticity_token << endl;

	int pp = web.indexOf("ata-component-context=\"replies");
	int ddn = web.indexOf("btn-link back-to-top hidden",pp);
	QString sss= web.mid(pp, ddn - pp);

	QStringList slist = sss.split("<li class=\"ThreadedConversation");
	for (size_t i = 1; i < slist.size(); i++)
	{
		QString str = slist.at(i);

		//已经回复过了
		if (str.indexOf("data-aria-label-part>@<b>tvt_io</b></span></a>") != -1)
			continue;

		QString strfind = "ata-aria-label-part";
		int p = str.indexOf(strfind);
		int p1 = str.indexOf("</p>", p + strfind.length() + 2);
		QString mxstr = str.mid(p + strfind.length()+5, p1 - p - strfind.length()-5);
		strfind = "wallet";
		p = mxstr.toLower().indexOf(strfind);
		QString reply_con = mxstr.mid(p);

		strfind = "mx";
		p = reply_con.toLower().indexOf(strfind);		
		QString mxaddress = reply_con.mid(p,34);

		if (p==-1)
			continue;
		qDebug() << "mxaddress:" << mxaddress << endl;
		SendCoin(mxaddress);
		strfind = "status/";
		p = str.indexOf(strfind);
		p1 = str.indexOf("\"", p + strfind.length() + 2);
		QString in_reply_to_status_id = str.mid(p + strfind.length(), p1 - p - strfind.length());

		create(authenticity_token, in_reply_to_status_id);
		continue;
		QEventLoop eventloop;
		QTimer::singleShot(1000 * 16, &eventloop, SLOT(quit()));
		eventloop.exec();
	}
}


void QBizManager::Get_cf_clearance(QString coo, QString & res)
{
	QHttpManager::GetInstance().setCookie(coo);
	int p = coo.indexOf("cf_clearance");
	QString cl = coo.mid(p, 72);
	coo = coo.replace(cl, "");	
	QEventLoop loop;
	QTimer::singleShot(2000, &loop, SLOT(quit()));
	loop.exec();

	QString web;
	QHttpManager::GetInstance().HttpGet("https://mercatox.com/", web);
	p = web.indexOf("jschl_vc");
	QString jschl_vc = web.mid(p + 17, 32);

	p = web.indexOf("pass");
	QString pass = web.mid(p + 13, 25);

	p = web.indexOf("s,t,o,p,b,r,e,a,k,i,n,g,f");
	p = web.indexOf(R"(":)", p);
	int p1 = web.indexOf("};", p);
	QString vafasfa = web.mid(p + 2, p1 - p - 2);
	
	p = web.indexOf("challenge-form");
	p = web.indexOf(R"(;)", p);
	p = web.indexOf(R"(;)", p);
	p1 = web.indexOf("a.value", p);
	QString vafassssfa = web.mid(p + 2, p1 - p - 2);

	p = vafassssfa.indexOf(R"(;)");
	p1 = vafassssfa.indexOf("=", p);
	QString keyword = vafassssfa.mid(p + 1, p1 - p - 2);
	vafassssfa = vafassssfa.replace(keyword, "LQbNxnX");

	QScriptEngine engine;
	QScriptValue global = engine.globalObject();
	QString vasl = "var LQbNxnX = ";
	vasl.append(vafasfa);
	vasl.append(vafassssfa);
	vasl.append(";LQbNxnX = LQbNxnX.toFixed(10) ;");

	engine.evaluate(vasl);
	QScriptValue vv = global.property("LQbNxnX");
	qDebug() << vv.toString();
	qsreal ds = vv.toNumber() + 12;
	QString ddsss = QString::number(ds, '10', 10);
	loop;
	QTimer::singleShot(7000, &loop, SLOT(quit()));
	loop.exec();
	QString cfuid = "_cfduid=";
	QString url = "https://mercatox.com/cdn-cgi/l/chk_jschl?jschl_vc=" + jschl_vc + "&pass=" + pass + "&jschl_answer=" + ddsss;
	QHttpManager::GetInstance().HttpGet_ht(url, web, cfuid);
	if (cfuid.length() >= 10)
	{
		int pp = coo.indexOf(";");
		coo = coo.mid(pp);
		coo = cfuid + coo;
	}
	coo.append(";");
	coo.append("cf_clearance=");
	coo.append(web);
	res = coo;
}

bool QBizManager::SendCoin(QString address)
{	
	QHttpManager::GetInstance().setCookie(m_cookieList.at(1));

	QString web;
	QHttpManager::GetInstance().HttpGet_t("https://mercatox.com/wallet/transfer/tvt", web);

	int p = web.indexOf("_csrf");
	int p1 = web.indexOf("\">", p + 20);
	QString _csrf = web.mid(p + 41, p1 - p - 43);
	_csrf.append("%3D%3D");

	web = "";
	QString _operation = R"(_csrf=%1)";
	_operation = _operation.arg(_csrf);
	qDebug() << "_csrf:"<<_operation;
	
	QHttpManager::GetInstance().HttpPost_email("https://mercatox.com/wallet/send-transfer-mail", _operation.toUtf8(), web);
	qDebug() << "send-transfer-mail. "<<web;
	if( (web.indexOf("ok") != -1)|| (web.indexOf("1") != -1))
	{
		for (size_t e = 0; e < 3; e++)
		{
			QString code = GetEmailCode();
			for (size_t i = 0; i < 3; i++)
			{
				if (code.length() == 6)
					break;
				code = GetEmailCode();
				QEventLoop loop;
				QTimer::singleShot(12000, &loop, SLOT(quit()));
				loop.exec();
			}
			
			if (code.length() != 6)
				continue;
				
			qDebug() << "code  "<<code;
			_operation = QString("email=%1&amount=12&currency=194&_csrf=%3&transfer_key=%2").arg(address).arg(code).arg(_csrf);
			web = "";

			QHttpManager::GetInstance().HttpPost_email("https://mercatox.com/wallet/transfer-check", _operation.toUtf8(), web);
			qDebug() <<"transfer-check:"<< web.mid(0, 50) << endl;
			if (web.indexOf("status\":\"ok\", \"data") == -1)
				break;

			if (web.indexOf("Incorrect code. Try again") != -1)
				continue;
		}
	
	}
	return true;
}

QString QBizManager::GetEmailCode()
{
	_QImap->queryEmail();
	_QImap->select("INBOX");

	QString str;
	for (size_t i = 0; i < 5; i++)
	{
		QEventLoop loop;
		QTimer::singleShot(2000, &loop, SLOT(quit()));
		loop.exec();
		str = _QImap->Getselect();
		if (!str.isEmpty())
			break;
	}

	_QImap->status("INBOX");

	int po = str.indexOf("UIDs");
	int p = str.indexOf("* ",po);
	int p1 = str.indexOf(" EXISTS", p);
	QString uid = str.mid(p + 2, p1 - p - 2);
	int uint = uid.toInt();
	uid = QString::number(uint);
	_QImap->fetch(uid, "BODY[TEXT]");

	_QImap->setemail(11);

	str = "";
	for (size_t i = 0; i < 5; i++)
	{
		QEventLoop loop;
		QTimer::singleShot(2000, &loop, SLOT(quit()));
		loop.exec();
		str = _QImap->Getmail();
		if (!str.isEmpty())
			break;
	}
	
	p = str.indexOf("=C2=A0");
	p1 = str.indexOf("=", p + 10);
	QString uigd = str.mid(p + 10, p1 - p - 14);

	qDebug()<<"\r\n\r\n\r\n\r\n"<< uigd<<endl<<endl;
	{
		p = str.indexOf("=C2=A0",p1+500);
		if (p != -1)
		{
			p1 = str.indexOf("=", p + 10);
			QString ouigd = str.mid(p + 10, p1 - p - 14);
			return ouigd;
		}
	}
	return uigd;	
}


void QBizManager::testmail()
{
	//for(int i=0;i<=10;i++)
	//QString code = GetEmailCode();
	QString id;
	Twitter twitter(0);
	twitter.show(id);
	
	
	QEventLoop eventloop;
	QTimer::singleShot(1000 * 16, &eventloop, SLOT(quit()));
	eventloop.exec();
}

bool QBizManager::create(const QString& authenticity_token, const QString& subject)
{
	QString str;
	QString post_data = QString("authenticity_token=%1&auto_populate_reply_metadata=true&batch_mode=off&in_reply_to_status_id=%2&is_permalink_page=true&place_id=&status=payment please check it&tagged_users=").arg(authenticity_token).arg(subject);

	QString url = QString("https://twitter.com/i/tweet/create");
	QString source;
	QByteArray send;
	send.append(post_data);
	QHttpManager::GetInstance().HttpPost_bitcointalk(url, send, str, source);
	qDebug() << "create:" << source.mid(0,20) << endl;
	if (source.indexOf("tweet_id") != -1)
	{
		return true;
	}
	return false;
}

