#include "qhttpmanager.h"
#include <QObject>
QHttpManager::QHttpManager( QObject *parent) :
    QObject(parent)
{
	m_agent = QString(R"(Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36)");
}

bool QHttpManager::HttpGet_twitter(const QString& url,QString& web)
 {
    QNetworkAccessManager* manager = new QNetworkAccessManager();
    QNetworkRequest request;
	QSslConfiguration sslConfig = request.sslConfiguration();
	sslConfig.setPeerVerifyMode(QSslSocket::QueryPeer);
	request.setSslConfiguration(sslConfig);

    request.setUrl(QUrl(url));
    request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
    request.setRawHeader("Cookie", "zh-CN,zh;q=0.8,en;q=0.6");
	request.setRawHeader("User-Agent", m_agent.toUtf8());

    QNetworkReply *reply =    manager->get(request);
    QTimer timer;
    timer.setSingleShot(true);
    QEventLoop loop;

    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    QByteArray responseData;
    responseData =  reply->readAll();
    web = QString(responseData);
    reply->close();
    reply->abort();

    delete manager;

    return true;
 }


bool QHttpManager::HttpGet(const QString& url, QString& web)
{
	QNetworkAccessManager* manager = new QNetworkAccessManager();
	QNetworkRequest request;
	QSslConfiguration sslConfig = request.sslConfiguration();
	sslConfig.setPeerVerifyMode(QSslSocket::QueryPeer);
	request.setSslConfiguration(sslConfig);

	request.setUrl(QUrl(url));
	request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
	request.setRawHeader("Cookie", "zh-CN,zh;q=0.8,en;q=0.6");
	request.setRawHeader("User-Agent", m_Cookie.toUtf8());

	request.setRawHeader("User-Agent", m_agent.toUtf8());

	QNetworkReply *reply = manager->get(request);

	QTimer timer;
	timer.setSingleShot(true);
	QEventLoop loop;

	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	QByteArray responseData;
	responseData = reply->readAll();
	web = QString(responseData);
	reply->close();
	reply->abort();

	delete manager;

	return true;
}


bool QHttpManager::HttpGet_t(const QString& url, QString& web)
{
	QNetworkAccessManager* manager = new QNetworkAccessManager();
	QNetworkRequest request;
	QSslConfiguration config;
	config.setPeerVerifyMode(QSslSocket::VerifyNone);
	config.setProtocol(QSsl::TlsV1_1);
	request.setSslConfiguration(config);

	request.setUrl(QUrl(url));
	request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8");
	request.setRawHeader("Accept-Language", "zh-CN,zh;q=0.8,en;q=0.6");
	request.setRawHeader("Cookie", m_Cookie.toUtf8());
	request.setRawHeader("User-Agent", m_agent.toUtf8());
	QNetworkReply *reply = manager->get(request);

	QTimer timer;
	timer.setSingleShot(true);
	QEventLoop loop;

	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	QByteArray responseData;
	responseData = reply->readAll();
	web = QString(responseData);
	reply->close();
	reply->abort();

	delete manager;

	return true;
}



bool QHttpManager::HttpGet_ht(const QString& url, QString& web, QString& cfuid)
{
	QNetworkAccessManager* manager = new QNetworkAccessManager();
	QNetworkRequest request;
	QSslConfiguration config;
	config.setPeerVerifyMode(QSslSocket::VerifyNone);
	config.setProtocol(QSsl::TlsV1_1);
	request.setSslConfiguration(config);

	request.setUrl(QUrl(url));
	request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
	request.setRawHeader("Accept-Language", "zh-CN,zh;q=0.8,en;q=0.6");
	request.setRawHeader("Cookie", m_Cookie.toUtf8());
	request.setRawHeader("User-Agent", m_agent.toUtf8());
	QNetworkReply  *reply = manager->head(request);

	QTimer timer;
	timer.setSingleShot(true);
	QEventLoop loop;

	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	auto _cookies = manager->cookieJar()->cookiesForUrl(QUrl("https://mercatox.com/"));
	for (QNetworkCookie cookie : _cookies)
	{
		qDebug() << cookie.name() << ":" << cookie.value();
		if (cookie.name().indexOf("cfduid") != -1)
		{
			cfuid.append(cookie.value());

		}
		web = cookie.value();
	}

	QByteArray responseData;
	responseData = reply->readAll();
	//web = QString(responseData);
	reply->close();
	reply->abort();

	delete manager;

	return true;
}

bool QHttpManager::HttpPost(const QString &url, const QByteArray &send, QString& web, QString sign)
{
	QNetworkAccessManager* manager = new QNetworkAccessManager();
	QNetworkRequest request(url);
	QSslConfiguration sslConfig = request.sslConfiguration();
	sslConfig.setPeerVerifyMode(QSslSocket::QueryPeer);
	request.setSslConfiguration(sslConfig);

;
	QString  ContentTypeHeader = "application/x-www-form-urlencoded; charset=UTF-8";
	request.setHeader(QNetworkRequest::ContentTypeHeader, ContentTypeHeader);
	request.setHeader(QNetworkRequest::ContentLengthHeader, send.length());
	request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
	request.setRawHeader("X-Requested-With", "XMLHttpRequest");
	request.setRawHeader("User-Agent", m_agent.toUtf8());
	request.setRawHeader("Cookie", "__cfduid=d64baeeeb5d6789e5118e7f692664c2ba1477227437; _ym_uid=1477227447927562658; localechatcn=en; wallet_check_hide_zero=0; Rfr=https%3A%2F%2Fyobit.net%2Fen%2Ftrade%2FLIZI%2FBTC; localechaten=en; s1=ba7rbh1GXZIkKo7ytPhTEwb7%2FrlRjDa6uNMF2EABS2EDH5HD5fDk%2ByLnQPVk571t5wUCbIVjujFspRZKYwvp5csyop1RfdAKbWCmfQSgSuBthNjaisD%2F6BVxzCKcmqvEsRdY78XW0J1EuV0tpGwd6vhHnGSox9BdqELbflJkEG1EqGLI%2F1MOvEQZqhIskoDU43B4SF3c9rNoeCby7HSswli9LkTwvDBMbRz2KJOPfaho7HhaJ8M1B4PfUlb8RpoVoBFDxG8aakXsKDlOacgWyegiosW5k7xb6HBxraV1yWHvFhdSR2BBPIxPhoFqw4FkIShny1bwevZUfoNwdpTdmRPHCrazvpLOvbzBHvT10E9EPbKAks%2FwdhrcQ8GvdqF9SRsB33lgN%2BSpy1nluhCxk2uRqgwnDf5Bgwnx0xXe7nKlvVqCxbfhPzgv%2BJlOmxEjG5685v7X52SXjz16NeqjAtwvZ5VGCfq68YUKPi7FB%2FEVFAVeLZd8TgmFPOLZfDIMzJULiDHqCpkGVg3UVomb%2BeRAXuyrJNYy8kNDSo3%2B15zXXG9hrjSlQZxpOFVlszwJwo68P%2FAQtlNUbS3GXvN5W9ivvWv2XoKF3ig5AAJotcOFFsB0%2FozfeJcz3spBhYK36wGHA7Xhhlk6fkXnRiIXGtiz59kewlZjhZ%2B%2FjZ9aWYo%3D; s2=hMeAGJvgpOR36NAADZzBBwLurT%2BtqARhag7I%2FvyjSm64EH8NxBr7iJ3oaiRtouJRWDd5h%2BrA3H%2B0JHEm9JJyrQd6qcYcG0q1Z63q5GloAiN3GsgTxJJ7pP2Bxgowf8uDnAg5E2Nugoso1Lpsmyx1RDq9skopTtLGAvpmOnXBT0XsxcyOXtUob1iWsTH5SFnJEkwvaR0xCs3b75YthsZAbuiJ1AZjN48vTcs7jqK0cVAQx4UepOFs39pYo5pEi%2FMM18T6lVJcG5bYieukat6PMvrzFDb3CKF%2F%2FvWLsdeEjVKVFrPALuZTlnP%2FNsv0C16R5R1WNFwlc94JOd1tfopVuj14Sma7pMuzrtaLwguX3fnQD9IECS7Fu244ol47Mo4dOGaBxxwPMYqQJPGg%2B6%2Fx1rsszldMmOHE2YjB0xMVwI%2B%2BQR1NQ9mPZGICoewQawVspHRo2QQ6bmJr1YhaW%2Frd4r5GRx%2FhYyVFJifTLbjr9Vav41Mc9TdSvFYJy0ayYa5Ep37cRCGz9Oiu%2F16zKqxpkuHil%2BpCDc0zWE0nEKc2QnjSEF43YqLIZgWj5q%2F2zEu8%2BHlbO28R4Tat1F0fAMrla9WLvprGPrUJ4%2FvdScbr5UHSgORnMGwaq8cum%2BTnKNdXbXhDMu7%2FkG6fIqGvh6kHtCyVG%2BWEuzVGhHKXgBCFetk%3D; _ym_isad=2; PHPSESSID=na4pvh3r6m871k2ig0b3csc4q7; c8dd96f3214c7a26d09a8102aaf6b231=1; chartmode=12H; locale_chat=en; 378162cf9e770468623322b3db697142=1; locale=en; _ym_visorc_27854913=w");
	QNetworkReply *reply = manager->post(request, send);

	QTimer timer;
	timer.setSingleShot(true);
	QEventLoop loop;

	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	QByteArray responseData;
	responseData = reply->readAll();
	web = QString(responseData);
	reply->close();
	reply->abort();
	manager->deleteResource(request);
	manager->deleteLater();
	delete manager;

	return true;
}

bool QHttpManager::onFinished(QString& web)
{
//    if (reply->error() == QNetworkReply::NoError) {
//        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
//        switch (statusCode) {
//            case 301:
//            case 302:
//            case 307:
//                   {
//                    QString redirect = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl().toString();
//                    if(redirect.indexOf("http://")==-1)
//                    {
//                        QString tmp= "http://www.beyond.com";
//                        tmp += redirect;
//                        redirect = tmp;
//                    }
//                    HttpGet(redirect,web);
//        }
//            break;

//            case 200:
//                QByteArray responseData;
//                responseData =  reply->readAll();
//                web = QString(responseData);
//                reply->close();
//                break;
//        }
//    }else if(reply->error()==QNetworkReply::UnknownNetworkError)
//    {
//        int e = reply->error();
//         //murl ="net e";
//    }else  if(reply->error()==QNetworkReply::ServiceUnavailableError)
//    {
//        int e = reply->error();
//        // murl ="dd";
//    }else
//    {
//        int e = reply->error();
//    }
	return 0;
}





bool QHttpManager::HttpGet_bitcointalk(const QString& url, QString& web)
{
	QNetworkAccessManager* manager = new QNetworkAccessManager();
	QNetworkRequest request(url);
	QSslConfiguration sslConfig = request.sslConfiguration();
	sslConfig.setPeerVerifyMode(QSslSocket::QueryPeer);
	request.setSslConfiguration(sslConfig);

	request.setUrl(QUrl(url));
	request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8");
	request.setRawHeader("Accept-Language", "zh-CN,zh;q=0.8,en;q=0.6");
	request.setRawHeader("Referer", "https://twitter.com/hitbtc/status/1042426155863863296");
	request.setRawHeader("User-Agent", m_agent.toUtf8());
	request.setRawHeader("Cookie", m_Cookie.toUtf8());
	QNetworkReply *reply = manager->get(request);

	QTimer timer;
	timer.setSingleShot(true);
	QEventLoop loop;

	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	QByteArray responseData;
	responseData = reply->readAll();
	web = QString(responseData);
	reply->close();
	reply->abort();

	delete manager;

	return true;
}


bool QHttpManager::HttpPost_bitcointalk(const QString &url, const QByteArray &send, const QString &str, QString& web)
{
	QNetworkAccessManager* manager = new QNetworkAccessManager();
	QNetworkRequest request(url);
	QSslConfiguration sslConfig = request.sslConfiguration();
	sslConfig.setPeerVerifyMode(QSslSocket::QueryPeer);
	request.setSslConfiguration(sslConfig);

	request.setUrl(QUrl(url));
	QString  ContentTypeHeader = "application/x-www-form-urlencoded; charset=UTF-8" ;
	request.setHeader(QNetworkRequest::ContentTypeHeader, ContentTypeHeader);
	request.setHeader(QNetworkRequest::ContentLengthHeader, send.length());


	request.setRawHeader("Referer", "https://twitter.com/hitbtc/status/1042426155863863296");

	request.setRawHeader("Origin", "https://twitter.com");
	request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
	request.setRawHeader("User-Agent", m_agent.toUtf8());
	request.setRawHeader("Cookie", m_Cookie.toUtf8());
	QNetworkReply *reply = manager->post(request, send);

	QTimer timer;
	timer.setSingleShot(true);
	QEventLoop loop;
	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	QByteArray responseData;
	responseData = reply->readAll();
	web = QString(responseData);
	reply->close();
	reply->abort();
	manager->deleteResource(request);
	manager->deleteLater();
	delete manager;

	return true;
}

bool QHttpManager::HttpPost_retweet(const QString &url, const QByteArray &send, const QString &authorization, QString& web)
{
	QNetworkAccessManager* manager = new QNetworkAccessManager();
	QNetworkRequest request(url);
	QSslConfiguration sslConfig = request.sslConfiguration();
	sslConfig.setPeerVerifyMode(QSslSocket::QueryPeer);
	request.setSslConfiguration(sslConfig);

	request.setUrl(QUrl(url));
	QString  ContentTypeHeader = "application/x-www-form-urlencoded; charset=UTF-8";
	request.setHeader(QNetworkRequest::ContentTypeHeader, ContentTypeHeader);
	request.setHeader(QNetworkRequest::ContentLengthHeader, send.length());


	request.setRawHeader("x-csrf-token", "ccab21a49b6d8126e1a1c245fee17838");
	request.setRawHeader("authorization", "Bearer AAAAAAAAAAAAAAAAAAAAAPYXBAAAAAAACLXUNDekMxqa8h%2F40K4moUkGsoc%3DTYfbDKbT3jJPCEVnMYqilB28NHfOPqkca3qaAxGfsyKCs0wRbw");
	request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
	request.setRawHeader("User-Agent", m_agent.toUtf8());
	request.setRawHeader("Cookie", m_Cookie.toUtf8());
	QNetworkReply *reply = manager->post(request, send);

	QTimer timer;
	timer.setSingleShot(true);
	QEventLoop loop;

	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	QByteArray responseData;
	responseData = reply->readAll();
	web = QString(responseData);
	reply->close();
	reply->abort();
	manager->deleteResource(request);
	manager->deleteLater();
	delete manager;

	return true;
}


bool QHttpManager::HttpPost_Transaction(const QString &url, const QByteArray &send, const QString &authorization, QString& web)
{
	QNetworkAccessManager* manager = new QNetworkAccessManager();
	QNetworkRequest request(url);
	QSslConfiguration sslConfig = request.sslConfiguration();
	sslConfig.setPeerVerifyMode(QSslSocket::QueryPeer);
	request.setSslConfiguration(sslConfig);

	request.setUrl(QUrl(url));
	QString  ContentTypeHeader = "application/json";
	request.setHeader(QNetworkRequest::ContentTypeHeader, ContentTypeHeader);
	request.setHeader(QNetworkRequest::ContentLengthHeader, send.length());



	//request.setRawHeader("authorization", "Bearer AAAAAAAAAAAAAAAAAAAAAPYXBAAAAAAACLXUNDekMxqa8h%2F40K4moUkGsoc%3DTYfbDKbT3jJPCEVnMYqilB28NHfOPqkca3qaAxGfsyKCs0wRbw");
	request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
	request.setRawHeader("User-Agent", m_agent.toUtf8());
	//request.setRawHeader("Cookie", m_Cookie.toUtf8());
	QNetworkReply *reply = manager->post(request, send);

	QTimer timer;
	timer.setSingleShot(true);
	QEventLoop loop;

	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	QByteArray responseData;
	responseData = reply->readAll();
	web = QString(responseData);
	reply->close();
	reply->abort();
	manager->deleteResource(request);
	manager->deleteLater();
	delete manager;

	return true;
}

bool QHttpManager::HttpPost_email(const QString &url, const QByteArray &send, QString& web)
{
	QNetworkAccessManager* manager = new QNetworkAccessManager();

	QNetworkRequest request;
	QSslConfiguration config;
	config.setPeerVerifyMode(QSslSocket::VerifyNone);
	config.setProtocol(QSsl::TlsV1_1);
	request.setSslConfiguration(config);


	request.setUrl(QUrl(url));
	QString  ContentTypeHeader = "application/x-www-form-urlencoded; charset=UTF-8";
	request.setHeader(QNetworkRequest::ContentTypeHeader, ContentTypeHeader);
	request.setHeader(QNetworkRequest::ContentLengthHeader, send.length());
	request.setRawHeader("Accept", "*/*");
	request.setRawHeader("Referer", "https://mercatox.com/wallet/transfer/TVT");
	request.setRawHeader("X-Requested-With", "XMLHttpRequest");
	request.setRawHeader("Accept-Language", "zh-CN,zh;q=0.9");
	request.setRawHeader("Cookie", m_Cookie.toUtf8());
	request.setRawHeader("User-Agent", m_agent.toUtf8());
	QNetworkReply *reply = manager->post(request, send);

	QTimer timer;
	timer.setSingleShot(true);
	QEventLoop loop;

	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	QByteArray responseData;
	responseData = reply->readAll();
	web = QString(responseData);
	reply->close();
	reply->abort();
	manager->deleteResource(request);
	manager->deleteLater();
	delete manager;

	return true;
}
