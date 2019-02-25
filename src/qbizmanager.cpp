#include "qbizmanager.h"
#include "qhttpmanager.h"
#include "twitter.h"
#include <queue>
#include <QObject>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <QVariant>
#include <QtScript/QtScript>

QBizManager::QBizManager() 
{


	_QImap =new QImap(nullptr);
	_QImap->connectToServer("imap.gmail.com", 993);
	_QImap->login("rahmirra12@gmail.com", "tvt@@@111111");


	m_StringList.append(R"(  )");
	m_StringList.append(R"( )");
	m_StringList.append(R"( )");
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
			qDebug() << "Get_cf_clearance ."<< res.length() << res;
			if (res.length() < 900)
				break;
		}
		m_cookieList[i] = res;
	}
}

void QBizManager::Get_cf_clearance(QString coo, QString & res)
{
	QHttpManager::GetInstance().setCookie(coo);
	int p = coo.indexOf("cf_clearance");
	QString cl = coo.mid(p, 72);
	coo = coo.replace(cl, "");	


	QString web;
	QHttpManager::GetInstance().HttpGet("https://mercatox.com/", web);
	if (web.isEmpty())
	{
		qDebug() << "Get_cf_clearance is empty.";
		return;
	}

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

	QEventLoop loop;
	QTimer::singleShot(8000, &loop, SLOT(quit()));
	loop.exec();

	qsreal ds = vv.toNumber() + 12;
	QString ddsss = QString::number(ds, '10', 10);
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
	qDebug() <<"cf_clearance=" << web;

	QTimer::singleShot(8000, &loop, SLOT(quit()));
	loop.exec();
}

bool QBizManager::SendCoin(const QString & address, QString & out)
{ 
	int rets = 3;
	QHttpManager::GetInstance().setCookie(m_cookieList.at(0));

	QString web;
	QHttpManager::GetInstance().HttpGet_t("https://mercatox.com/wallet/transfer/tvt", web);

	int p = web.indexOf("_csrf");
	int p1 = web.indexOf("\">", p + 20);
	QString _csrf = web.mid(p + 41, p1 - p - 43);

	if (_csrf.length() <= 70)
	{
		qDebug() << "SendCoin _csrf error" << _csrf;
		return rets;
	}
	if (_csrf.length() >= 200)
	{
		qDebug() << "_csrf error" << _csrf;;
		return rets;
	}

	_csrf.append("%3D%3D");
	QString _operation = R"(_csrf=%1)";
	_operation = _operation.arg(_csrf);
	qDebug() << "_csrf:"<<_operation;
	
	QHttpManager::GetInstance().HttpPost_email("https://mercatox.com/wallet/send-transfer-mail", _operation.toUtf8(), web);

    qDebug() << "send-transfer-mail. "<<web;
	if( (web.indexOf("ok") != -1)|| (web.indexOf("1") != -1))
	{
		qDebug() << "send-transfer-mail. a ";;

		for (size_t e = 0; e < 3; e++)
		{
			QString code = GetEmailCode();						
			if (code.length() != 6)
				continue;
				
			qDebug() << "code  "<<code;
			_operation = QString("email=%1&amount=100000&currency=194&_csrf=%3&transfer_key=%2").arg(address).arg(code).arg(_csrf);
			QHttpManager::GetInstance().HttpPost_email("https://mercatox.com/wallet/transfer-check", _operation.toUtf8(), web);
			qDebug() <<"transfer-check:"<< web.mid(0, 50) << endl;
			if (web.indexOf("status\":\"ok\",\"data") != -1)
			{
				out = "Tvt Successfully Sent. Please Check It.";
				qDebug() << out;
				rets = 1;
				break;
			}

			if (web.indexOf("Incorrect code. Try again") != -1)
			{
				rets = 0;
				continue;
			}

			if (web.indexOf("User with this email not found.") != -1)
			{
				out = "This Email Not Found. Please Comment Mercatox E-mail or E-Wallet ID Again";
				qDebug() << out;
				rets = 1;
				break;
			}				
		}	
	}
	qDebug() << "send-transfer-mail. b " << rets;;
	return rets;
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
    _QImap->setemail(11);

    QString email_code ;
    _QImap->fetch(uid,"BODY[]<3200.400>");

    for (size_t i = 0; i < 6; i++)
    {
        QEventLoop loop;
        QTimer::singleShot(2000, &loop, SLOT(quit()));
        loop.exec();
        str = _QImap->Getmail();
        if (!str.isEmpty())
        {
            p = str.indexOf("=C2=A0");
            p1 = str.indexOf("=", p + 10);
            email_code = str.mid(p + 10, p1 - p - 14);

            if (email_code.length() == 6)
                    break;
        }
        _QImap->fetch(uid, "BODY[]<3200.400>");
    }
    return email_code;
}

QString QBizManager::WriteCookie()
{
	QFile outFile(qApp->applicationDirPath() + "/cok");
	outFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
	for (int i = 0; i < m_cookieList.size(); i++)
	{
		QString strcokk = m_cookieList.at(i);
		QTextStream ts(&outFile);
		ts << strcokk << endl;
	}
	outFile.close();

	return 0;
}
