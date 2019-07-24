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
	request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.108 Safari/537.36");
	request.setRawHeader("Cookie", "__cfduid=df63a3682625b7cf1860aeb35cc43130d1557796694; locale=en; LLXR=1557796696; LLXUR=e1d86087b640; Rfr=https%3A%2F%2Fyobit.net%2Fen%2F; marketbase=doge; wallet_check_hide_zero=1; 7a5f1a51f1e0be737b068d11a1b96636=1; locale_chat=en; chartmode=12H; PHPSESSID=kcidgiafd9dpbdnm5gr6a31b66; s1=KdX%2B6fs2tkoyUsgAEtvFnwuWJIQBZDv4%2BrSwaUIbDxwEw7U%2FygPICBu6ni8JciwwWu6%2BMTaBJHRPOX4pilfZXXOAhqNoOm28cIVDNnUYcg75CiMv0nJ7Kky6ZHBhMJqVFiA47UXAAi5LxUwDQqWOZpqCUtRxtYmPamC5s4NP0w%2BB%2BQ%2FJwkQOfrA41m3Gexg3%2BklnP9IbHlBEL4sdfjMvQMMxJVocjyPGn7njmtRIKeMLIEfOZrVIUlM%2BUKJtZ%2Fbxoq12mySq9Ci3Rn1Cz9kvKcUK16t3tZWgVvsOfI09eHE7QvXb95vKNRnSWGb5VemD4%2BW1xLcD02ZwIs1EPFaAYQi5Ed24Py2gVHPMthxphRS6kbtjNADYhDD7VhuFuyqSpnWfy%2Fl5Dsanng7cX9Idq%2Bm8LPu09EpH%2Fg95TtTcZLa0XOOk0JFmSA8dOYF3vz2YVuBZYib9mO08OsL4YhDr9fO%2Fpfwk9cYdXL6CpOBcrjnqRJI4a15EHUcOkI4P5ERhL1PFy2O7UapMejoybAk0RZJWW48HgOZGn92gN5dxH6wEsPSV%2BuCuJtCxi2c3AhX9ygV87VAkpoIhbOOkO5BF89fbsN%2F7HrJ0kefsmHIlKlCSfpwVxyoh20%2BYO7FXtAzzihbj%2BVOI3n1MsGg6TKmV%2B0i0cUkwLXE%2FxLiSgJBgFVU%3D; s2=jACq7ho9eaB%2BfthPsbOzxjBwFbjxmXB%2BA4ev2b0ZsUJEX1sR4MF1T144GwhteKZDRzNhT%2FLfO5xXR5BxqxIr9qam4TppsuqxbAaTPgFf%2FGlcxeNmGuiohglOzphoSoOycU1JLnJUVNcviMPRd9AdD8HAG7ivE9OxeBf2mgNjbrlKNhXiaKyiUArjPFJwBJAON3qiwfJnbaJW7VG2MNHcsZ7HWgV3dQ%2Fy2Ejeckf3fFGbGWTig%2BFaDHLbIrxWrkf1OXr8bSiNy32Y8bxX5V9o5Umr%2BLlvVeq37bCY0VG2PrDsULd4P%2FUlYJW39P6wuyftS%2FXeE9gZ5goWotITTiZ3vArMQJbNDNNHdh2E%2FXiwhZhPTdyBrU7MeXSIT1ncHTt8mxfNiCeCaBFcrGLycmdpJ5RZCvT4saviupIofMubCQuS2RmCXtewhVpZLSbTegX5x3gS4FLSGz0fWrY2gvB8sDVr6zSP3Citk0t%2FKXpCJhthDyDpaIHXo3sMaaBxBIFhqPpDPRem%2BdR4cfoZjVJEu93kKiezVZ4p5v6cJUs12nBmwA%2B1JAl6%2FatpvWNQJ7aKj7XvEwDtUOeo7XOfQrFnTws5cnzW2BbjMCxYNnZM9J3GCYkFNrbGvDHLnkG8fNSDtCnjFWzevf1rjdvVye2EjB0S65K1Fs7Vy0TeYlNorG4%3D");
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
    request.setRawHeader("Key", "8575345D0F4BECF4607D472F00AACA93");
	request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.108 Safari/537.36");
	request.setRawHeader("Cookie", "__cfduid=df63a3682625b7cf1860aeb35cc43130d1557796694; locale=en; LLXR=1557796696; LLXUR=e1d86087b640; Rfr=https%3A%2F%2Fyobit.net%2Fen%2F; marketbase=doge; wallet_check_hide_zero=1; 7a5f1a51f1e0be737b068d11a1b96636=1; locale_chat=en; chartmode=12H; PHPSESSID=kcidgiafd9dpbdnm5gr6a31b66; s1=KdX%2B6fs2tkoyUsgAEtvFnwuWJIQBZDv4%2BrSwaUIbDxwEw7U%2FygPICBu6ni8JciwwWu6%2BMTaBJHRPOX4pilfZXXOAhqNoOm28cIVDNnUYcg75CiMv0nJ7Kky6ZHBhMJqVFiA47UXAAi5LxUwDQqWOZpqCUtRxtYmPamC5s4NP0w%2BB%2BQ%2FJwkQOfrA41m3Gexg3%2BklnP9IbHlBEL4sdfjMvQMMxJVocjyPGn7njmtRIKeMLIEfOZrVIUlM%2BUKJtZ%2Fbxoq12mySq9Ci3Rn1Cz9kvKcUK16t3tZWgVvsOfI09eHE7QvXb95vKNRnSWGb5VemD4%2BW1xLcD02ZwIs1EPFaAYQi5Ed24Py2gVHPMthxphRS6kbtjNADYhDD7VhuFuyqSpnWfy%2Fl5Dsanng7cX9Idq%2Bm8LPu09EpH%2Fg95TtTcZLa0XOOk0JFmSA8dOYF3vz2YVuBZYib9mO08OsL4YhDr9fO%2Fpfwk9cYdXL6CpOBcrjnqRJI4a15EHUcOkI4P5ERhL1PFy2O7UapMejoybAk0RZJWW48HgOZGn92gN5dxH6wEsPSV%2BuCuJtCxi2c3AhX9ygV87VAkpoIhbOOkO5BF89fbsN%2F7HrJ0kefsmHIlKlCSfpwVxyoh20%2BYO7FXtAzzihbj%2BVOI3n1MsGg6TKmV%2B0i0cUkwLXE%2FxLiSgJBgFVU%3D; s2=jACq7ho9eaB%2BfthPsbOzxjBwFbjxmXB%2BA4ev2b0ZsUJEX1sR4MF1T144GwhteKZDRzNhT%2FLfO5xXR5BxqxIr9qam4TppsuqxbAaTPgFf%2FGlcxeNmGuiohglOzphoSoOycU1JLnJUVNcviMPRd9AdD8HAG7ivE9OxeBf2mgNjbrlKNhXiaKyiUArjPFJwBJAON3qiwfJnbaJW7VG2MNHcsZ7HWgV3dQ%2Fy2Ejeckf3fFGbGWTig%2BFaDHLbIrxWrkf1OXr8bSiNy32Y8bxX5V9o5Umr%2BLlvVeq37bCY0VG2PrDsULd4P%2FUlYJW39P6wuyftS%2FXeE9gZ5goWotITTiZ3vArMQJbNDNNHdh2E%2FXiwhZhPTdyBrU7MeXSIT1ncHTt8mxfNiCeCaBFcrGLycmdpJ5RZCvT4saviupIofMubCQuS2RmCXtewhVpZLSbTegX5x3gS4FLSGz0fWrY2gvB8sDVr6zSP3Citk0t%2FKXpCJhthDyDpaIHXo3sMaaBxBIFhqPpDPRem%2BdR4cfoZjVJEu93kKiezVZ4p5v6cJUs12nBmwA%2B1JAl6%2FatpvWNQJ7aKj7XvEwDtUOeo7XOfQrFnTws5cnzW2BbjMCxYNnZM9J3GCYkFNrbGvDHLnkG8fNSDtCnjFWzevf1rjdvVye2EjB0S65K1Fs7Vy0TeYlNorG4%3D");


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
