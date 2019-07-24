#include "qhttpmanager.h"
#include <QObject>
QHttpManager::QHttpManager( QObject *parent) :
    QObject(parent)
{
	 _manager = new QNetworkAccessManager();

}

bool QHttpManager::HttpGet(const QString& url,QString& web)
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

bool QHttpManager::HttpPost(const QString &url,const QByteArray &send,QString& web,QString sign)
{
	QNetworkAccessManager* manager = new QNetworkAccessManager();

    QNetworkRequest request;
    QSslConfiguration config;
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1_1);
    request.setSslConfiguration(config);


	request.setUrl(QUrl(url));
	QString  ContentTypeHeader = "application/x-www-form-urlencoded";
	request.setHeader(QNetworkRequest::ContentTypeHeader, ContentTypeHeader);
	request.setHeader(QNetworkRequest::ContentLengthHeader, send.length());
	request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
	request.setRawHeader("Accept-Language", "zh-CN,zh;q=0.8,en;q=0.6");
    request.setRawHeader("Sign", sign.toLatin1());
        request.setRawHeader("Key", "32400CC4AF0290188A845D94854C43D5");


	QNetworkReply *reply = manager->post(request, send);

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
	manager->deleteResource(request);
	manager->deleteLater();
	delete manager;

    return true;
}

bool QHttpManager::huobi_HttpPost(const QString &url,const QByteArray &send,const QByteArray &authToken,QString tonce,QString& web)
{
    QNetworkAccessManager* manager = new QNetworkAccessManager();
    QNetworkRequest request;
    QSslConfiguration config;
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1_1);
    request.setSslConfiguration(config);


    request.setUrl(QUrl(url));
    QString  ContentTypeHeader = "application/json-rpc";
    request.setHeader(QNetworkRequest::ContentTypeHeader,ContentTypeHeader);
    request.setHeader(QNetworkRequest::ContentLengthHeader,send.length());

    request.setRawHeader("Accept", "application/json, text/javascript, */*; q=0.01");

    request.setRawHeader("Authorization", "Basic " + authToken);
    request.setRawHeader("Json-Rpc-Tonce", tonce.toLatin1());
    request.setRawHeader("Accept-Language", "zh-CN,zh;q=0.8,en;q=0.6");

    QNetworkReply* reply = manager->post(request,send);

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


bool QHttpManager::HttpPost_msg(const QString &url, const QByteArray &send, QString& web, const QString& cookie)
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
	request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
	request.setRawHeader("Accept-Language", "XMLHttpRequest");
	//;;//	request.setRawHeader("X-Requested-With", "zh-CN,zh;q=0.8,en;q=0.6");

	request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/56.0.2924.87 Safari/537.36");// "__RequestVerificationToken=CuO9KlS5yVnK2EzTfK9VJP9J4WUYW4M0l9GCV-JPu9J4NcEe8kDHnA4kNeOmVC8kBqCBeiQTXK0IhMUAxPbmfoXEnyKj1VvW0N5Ijg2lUWo1; nlbi_1244263=sI3SV/GTQ3Aku5c1Pnz9kwAAAADeoU4hlTdMW8X4J+1cJ3GR; incap_ses_485_1244263=oQ4FKbw8HzKpao6YfhG7Bhakp1kAAAAAwUqY7Iq/idNLqY53cJzM3A==; incap_ses_426_1244263=Mnr2L776eVCnAStk/3TpBWuop1kAAAAASKktHIWKvpf+fiAbXImm9A==; Cryptopia=RCKGvHWX3ts5dMlJCJAPL3Av2I8J_z9hgDyREiJzY6GJrR_JCQvSRMBnM_cwYB9NmhDKBoKx_oNSR0l1mmC5tZ1Dn5Rf6vmyo0I9Q0GOHx9JEXzhQuBuMM-OdO56oZ9MLzgm3tNWXeM8fzBEZSUswriUHniwGAAqIGHt8ndRWiInKlNP_2Mf1A5G5Qp8UECAxNWjP1gmgeTviNIRmk7aR4gqpwVvBAZ3ta1U5vWDems6pSy8JaxN59KhpyV5BrrG01JaCXVvI6toFjZp6JGEKCEkSnjCYk9UAOpw29zu377Jf9TVvFyObfMLs0V_nmPSt5zA66ug3KuXMAilZQGsHfR2og-AEQg03ddmDfeFbdCOhufo3tW0bk2FGQ5tLgw63P_IWDpAcOR-kNirQX4Q1iEgWKBLKVAKGGEVPZzW3g0-92vnRhshenRe8KGXpetz8nFkdqFlyZF6-T95gpFtrsMiS4-fC1nUcolpqEmX8N0wq-KTpcdcLO9Lk9EVil7OQczPiqvxecm2CujfZ4Lf4ylvg599Nl8sB4j9R3-tx7AJTvIHj_QoOWFKC-Xdf-QfVXh56GDXBtn1sFILs_EX-I0EOM08xY78IZ9pZx_Km8otfA9De3Ch_ibMwf7nN7XM19WtWo7Z9TXdcQoWN9lWyA; visid_incap_1244263=+bVT8gDhTDGl3O2DykbMxOAZmVkAAAAAQUIPAAAAAACDD0SSFvUVU3cf/RqyoS3s; incap_ses_265_1244263=Btwvffktmij8/RSk0HmtA4e5p1kAAAAA6gkgf9h+vJmD6QytPZWzMA==; __asc=b7a9557f15e372d5f3dd893c159; __auc=ce80168e1581a727645194e4178");
	//request.setRawHeader("Cookie", "visid_incap_1244263=+bVT8gDhTDGl3O2DykbMxOAZmVkAAAAAQUIPAAAAAACDD0SSFvUVU3cf/RqyoS3s; __RequestVerificationToken=9DA2paQdaGGVQQO--q4wLaTVxVnT7Y4azXTZfFwOViOSUKjksfAHFjQItSwM5MEpXPgs87qBZt8yP-pDEMLYX0GKke__ihiDB7zOoCh7_hQ1; nlbi_1244263=oK/7ftTMS0NiktSPaz1cMwAAAACiIXWeK3+7sBq1X6npkOxp; incap_ses_624_1244263=NeqoR4YE2hQk82t0DOWoCFZ33FkAAAAAX/NFP5kIbjcPMC51Pzof7g==; incap_ses_258_1244263=rDpoKNvr7mMglPxtTpqUA2KW3FkAAAAAeRCOH+qMSdqTNpj5UnlhQQ==; __asc=f615e48b15f05abcc777949e5e2; __auc=ce80168e1581a727645194e4178; Cryptopia=Usm9ImwH4NDhZMqJiQap0f-35JWLdHyIEi_56DCj5CiBkP1D0OdE6vYZifvavBIWJNoQ8-KKFEdJghSgO7u4iNYFzpPriLqZntGiK_wVlD6HP7SnCxGp-zDpv84qarsaYBaT121GgBqhQvqN8aNAhSqUQvAIxbACG3F7xxt2DG00H4nYz0DAlxqc5YpO1vpBSmkMUkJA75vL2aishmuqtR4Pv5jb3DYOUlqMBoi0Rmze8UyGe-wpXXaJJJTQRNx0rckyUaWFClTJCtPwe0OjaB4suxl8u97JzQw4Pd01uxB5gLDrZ_JfI02RnLgKXG0s7XuuLz8YuiyEvzYEV8ZKAAxvnc7_ynM0kdmKyirVixjXfq4YnLKoW4Gcif8QINjiXzBhv8aZzJXUkbttzxlD8ADcqKIOB5kDL7yzrajet4IPNWYr6BfUUQRTQnBGHUhIlOxEB9ObzfoI_iYeKTc2pJD4QavQQ5pGUeAZNBxBbfPwYHnPZ1yuiX7TzVdGI9VbDyBXu5AY7MqUF7rK66elqmxEe_-q2TMzM3r32dgNNp5k7Eh5QkDSVaegLBud-warJbu05F82Y67ulYxrIk6N1AQophPjucl7BG4ORnE-klwqJq-eKMTviA0TCFFajjs1oOeZvnpKPiRXo5K9eQm9PQ");
     request.setRawHeader("Cookie", cookie.toLatin1());
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
