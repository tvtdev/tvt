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
	//request.setRawHeader("Cookie", "__cfduid=dc3aa35b1442cd77ab20bd50c94668dca1555571431; locale=en; LLXR=1555571440; LLXUR=64c01c5ec62b; wallet_check_hide_zero=1; Rfr=https%3A%2F%2Fyobit.net%2Fen%2F; marketbase=doge; 665ed720d93d161f21b64e9eddab748e=1; PHPSESSID=iam8b06m81f1dnitencbkr3th3; s1=flvOobsTOYzZD1yc%2FYDF%2F7UIqEf1kxfIE0nrcEvTZsiVHTrB9t85cFzC8HYeQ%2BpaLBU3eJMeUcWgHO2oR0hW8CTYLnE4GXgWu2QdPT3hq5%2BbYilNPWOfNavRzOMBd19z74LP4EehfnWsie9ClAs7jxlriO7UKtsE0I3EUBquZ24GKJX2jzvNDDAqhixfF5cLDOdeXqINynAlE66ZmUdE7zRnRTZqfVKYyCVg1EzvVYCTgX96Lv5b0mNm7AncIwcMT596c8jgCg4q24yxXkR%2BpiX8lp38DBARAtS99qdShyniTAAxBux%2FKLCuyHldrxy8HaGnWU%2Bd5D7DPVq7Tw8YovblzIGJ5QJxBBBURWWPudF4Gm7QoMoa2G1JpibDDecKzzDReFA288cZf7LoaWZE9tRHHD0wMKFUCrZX0cfRMy206QOdkkbg8oEoiINlCMpCXdJErjVu1FRmj6DydDKoVj%2BZHyoMfuxj3bKZIYCt%2FGt%2BXz0eSd42CruOQAemtdTwZFgoOgaRNE0%2B3LCPaF453Rt6MtvV47FuBFp3KnTkwohhnbAcKmUhbePHNtK0Rq7hFZ6iIjWOcpcUzV4xDPSZkzSXccxGXZ4LYc45kMoG5RLYGkSF7wHQ%2FIk7XKlByOjXZBEeaxTsIHa%2FpMgBBoD6iuaaS3IosZHc8Umf6OtQ8xQ%3D; s2=ICxLsuzLPY%2FP%2B7LfWR7T5vd8OJFPnrZMw469bT41enrQn7Ph2g4xBQqScibhS6tyhbTAI8TrQ27E0y68g9lb8aVQr3UyMojVyJonYavfZ6zcNiZJ9%2BaaiDap7CzMLShd5wb1StEIs7b40175jcrndQYE9%2BTaaGC%2F%2FZWha%2BOTus1MHpc1O8I25oMW13%2FZZ7xXZnrVHJJxFI0GG1yCEB%2B7Eu%2FnXg8EoFHrY2jVrCvKJJuSoIia3allSSGBhE%2BnZ5YFBSN%2BNEkDxpRccJX9IKjaL%2Bo%2BgmqbiNo4Spu5%2FAfK5UC45eoWOYWLov6ogkSS1ElX8WPQXQNtrwM7pjOMEVPQRGHRZOW6YhHCqqpKSH%2BjXXZ6Gj%2B9s9jN%2BNmQ61CK5OEYU%2BpG3eGxXxIm4UR3YBfJlRKsI8J6l662Sou6lxk3hJqh9nw73Uw%2FRmR%2FdK%2BpHsFBZ716gGJEn6N9SwwGYD4i6YLzIgfK344a7zRLRADHNyHoqFUacDdZAdeOVBLkuJYK%2F%2FQAy%2FGKpXv%2FGzUY3uOFc82Je5RWsu00iDuHfKBFzBR5WcptjvssFxFrpdgrEFPlzIGoY3kK45z7eYKWCWWhwQcaUFwLlfTn5qs1JGpmi8zfa6vKs8OoF1RzcYmER2eZz%2Bu7lRBMkvSvLQ9E%2FX3L8rx2ANpZ8Vd9Bk2627t3Qfc%3D; locale_chat=en; chartmode=12H");
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
    request.setRawHeader("Key", "66666EEFA9A1241A4F91DA3755473B05");
	request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.108 Safari/537.36");
	//request.setRawHeader("Cookie", "__cfduid=dc3aa35b1442cd77ab20bd50c94668dca1555571431; locale=en; LLXR=1555571440; LLXUR=64c01c5ec62b; wallet_check_hide_zero=1; Rfr=https%3A%2F%2Fyobit.net%2Fen%2F; marketbase=doge; 665ed720d93d161f21b64e9eddab748e=1; PHPSESSID=iam8b06m81f1dnitencbkr3th3; s1=flvOobsTOYzZD1yc%2FYDF%2F7UIqEf1kxfIE0nrcEvTZsiVHTrB9t85cFzC8HYeQ%2BpaLBU3eJMeUcWgHO2oR0hW8CTYLnE4GXgWu2QdPT3hq5%2BbYilNPWOfNavRzOMBd19z74LP4EehfnWsie9ClAs7jxlriO7UKtsE0I3EUBquZ24GKJX2jzvNDDAqhixfF5cLDOdeXqINynAlE66ZmUdE7zRnRTZqfVKYyCVg1EzvVYCTgX96Lv5b0mNm7AncIwcMT596c8jgCg4q24yxXkR%2BpiX8lp38DBARAtS99qdShyniTAAxBux%2FKLCuyHldrxy8HaGnWU%2Bd5D7DPVq7Tw8YovblzIGJ5QJxBBBURWWPudF4Gm7QoMoa2G1JpibDDecKzzDReFA288cZf7LoaWZE9tRHHD0wMKFUCrZX0cfRMy206QOdkkbg8oEoiINlCMpCXdJErjVu1FRmj6DydDKoVj%2BZHyoMfuxj3bKZIYCt%2FGt%2BXz0eSd42CruOQAemtdTwZFgoOgaRNE0%2B3LCPaF453Rt6MtvV47FuBFp3KnTkwohhnbAcKmUhbePHNtK0Rq7hFZ6iIjWOcpcUzV4xDPSZkzSXccxGXZ4LYc45kMoG5RLYGkSF7wHQ%2FIk7XKlByOjXZBEeaxTsIHa%2FpMgBBoD6iuaaS3IosZHc8Umf6OtQ8xQ%3D; s2=ICxLsuzLPY%2FP%2B7LfWR7T5vd8OJFPnrZMw469bT41enrQn7Ph2g4xBQqScibhS6tyhbTAI8TrQ27E0y68g9lb8aVQr3UyMojVyJonYavfZ6zcNiZJ9%2BaaiDap7CzMLShd5wb1StEIs7b40175jcrndQYE9%2BTaaGC%2F%2FZWha%2BOTus1MHpc1O8I25oMW13%2FZZ7xXZnrVHJJxFI0GG1yCEB%2B7Eu%2FnXg8EoFHrY2jVrCvKJJuSoIia3allSSGBhE%2BnZ5YFBSN%2BNEkDxpRccJX9IKjaL%2Bo%2BgmqbiNo4Spu5%2FAfK5UC45eoWOYWLov6ogkSS1ElX8WPQXQNtrwM7pjOMEVPQRGHRZOW6YhHCqqpKSH%2BjXXZ6Gj%2B9s9jN%2BNmQ61CK5OEYU%2BpG3eGxXxIm4UR3YBfJlRKsI8J6l662Sou6lxk3hJqh9nw73Uw%2FRmR%2FdK%2BpHsFBZ716gGJEn6N9SwwGYD4i6YLzIgfK344a7zRLRADHNyHoqFUacDdZAdeOVBLkuJYK%2F%2FQAy%2FGKpXv%2FGzUY3uOFc82Je5RWsu00iDuHfKBFzBR5WcptjvssFxFrpdgrEFPlzIGoY3kK45z7eYKWCWWhwQcaUFwLlfTn5qs1JGpmi8zfa6vKs8OoF1RzcYmER2eZz%2Bu7lRBMkvSvLQ9E%2FX3L8rx2ANpZ8Vd9Bk2627t3Qfc%3D; locale_chat=en; chartmode=12H");


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

	return 0;
}
