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
	request.setRawHeader("Cookie", "__cfduid=dc3aa35b1442cd77ab20bd50c94668dca1555571431; LLXR=1555571440; LLXUR=64c01c5ec62b; Rfr=https%3A%2F%2Fyobit.net%2Fen%2F; wallet_check_hide_zero=0; localechaten=en; 665ed720d93d161f21b64e9eddab748e=1; localechatcn=en; locale=en; 7a5f1a51f1e0be737b068d11a1b96636=1; locale_chat=en; marketbase=doge; chartmode=12H; PHPSESSID=e8s9dgqcbipqt43dgkalh4rrc6; s1=f%2FOiwLVLe08kVdAOdggBseNPLpigqGW09x9c6FzpP%2F9hMum3bi%2Bg7ovgR6hKEFMnLsuvdony0FRApyMUcItCBkgycrTq5qOGiD4wMt06oia7M4q61CS3PgNWp8Srhcg2WyGkeCrxTq5FKI28DO%2FHuS4c1gwNSo%2FttExhqfPAJ8%2FsrhJQzWb5lfpH2MaFA%2FlGJjx2k%2FP2yvj1EU11K1IaejjPWI1P1O%2BT05YegzEikBmQgYtxIh0ec9gd3CNEXU60yOlPDB5vS8zI54bC714O9kOAH%2FYivORFeTojvCWdhWR4XwlSrSBMn1lMCjQp3pWJUEgOu%2FbfPlpGW%2F1sNugiiqL7XWCxz6InMFT12ySWOYRnZqttQ5E0YGLdxSmtgoXblbZ8nBlWDWFYO9JbqPPA%2FQ5b0G%2BNEDM7D1mL19pwfE%2B5IlhGFFonlQCOeXTw4XnIEMs%2FWVqfdBMUU9kzn7dmmx7ZIAd8vSF8BeVJflW3GbZLSHIxWc0HQscAq5MU3sfffbXQ5Lf%2BFcWpT3V%2FPmBFgz6JWhH90xcaLjwxX3YdC3T3J8jayQOUni8EueRHP3esg5i9Ud3iwJ%2F2uFDSnqJII%2FA7Pe%2BDgS7PeVkDqBpPPDfFMZ4%2FahYCJCIqm721hfTpwuVPCFhJt5vHy0jyFnpKecjwX1ERWoQ4RLoZWtiJ0A0%3D; s2=JBycAFUti77TOs4O0gQGia%2BuCpIrBT23lieyntQJopbLEGJCkhJVSwDUAttgxVfCqrZr8wN4yzlPHNziJNQ2txu5j5i7uYhEL6L3KeR7nnAnnqxN%2F7oxF0DNsvISwF8KBePNQtp8jbPBqW5KnGNR2Xr7c8N4oOtZcG00fmXNZWnDI%2Buh1YMWj05n4vp%2F0KwxkVIUmotLtPn024z86kD7QE7AqnOj120mkiOegKfm0uHp8HkkKBL1BcSLXSrZlyLLocAsUXucw5UypwThXxViZ3JNLebc2iIHW0v5lOeyWnLx4czT9ak9OpSs6ZFf3pi9rU2vKzvwIMvabaMSP787mtfqfL2ANrUDmaSoA73J%2Fpp1xIIHKMiFnO0bMjG2T2E9ShmoMj7ALp0EFxgma%2Brq8qs%2FMk5qApn8G5eVGtbUBIZKJ9GAvb5ZhxILhrhjSGfRgTTtETehQM38e%2FK51zvLkpKBTTclytuRh4K0g9NQN5j3YFRpJFQWpkGvE%2Bhe0C%2F3mSlpe2Sf5C%2Fo0JSlqtoLlEE2GmNyEPqjzKEAtHOAdh7JO8ZOVbGHj31jLdw0wZjhcqmK9zwL4zdkvh8SVYUcgWL5tUZj4skM%2F0P8RqRMoxa4ZzQtUMBivQFcFPK63xCl%2B54QSd3nsjZfrl4cxGATSxTIzARBEkhg7eFjV74K6VQ%3D");
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
    request.setRawHeader("Key", "85C21D892C5E9C1AA0EF87E2AC9B5D8F");
	request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.108 Safari/537.36");
	//request.setRawHeader("Cookie", "__cfduid=dc3aa35b1442cd77ab20bd50c94668dca1555571431; LLXR=1555571440; LLXUR=64c01c5ec62b; Rfr=https%3A%2F%2Fyobit.net%2Fen%2F; wallet_check_hide_zero=0; localechaten=en; 665ed720d93d161f21b64e9eddab748e=1; localechatcn=en; locale=en; marketbase=doge; locale_chat=en; chartmode=12H; PHPSESSID=5vcc768ku87ehltqt1ki26cqm7; s1=GwqGY5Y9yxGOllPJrefNDM%2FOB9EBBy7DFdpgw7ywZ4cUBEIJ1A4ZO1mSjnCA6RqHIjuH2tlanBgoUnGUlAnh6HleGcPz5iKfzH6RpgdmR6vmo5YrWWIOstPQw%2BPv8%2BELaofFmSxbCBQPdyI6kF0RdWEbJjZh0TOk%2FvWHXRweUPuU4%2FJd6KKpR%2FO3Hilt7VtOFSTCXsUODPPVldd8is5dFxpPq8Lcr7eL6nCh%2FCNwHPr2Ty9uixL6vbg8YOyIB%2Fsl8gwv3VW0b3mpL01FRKwaBHLWYtD7BMowi5XmVZLFuYqCWr4ogxC8Z61AECNzzFSQlb7dT6BfE7R06XXkt1xGMdbqgVS5wcAV2%2F2wq2lWSvdoEigkSvBRE686pZNbeTIk%2FBe6xQU6uq0XTeMHeZbGpp6nBYUJjZxeDD5R7JhuPRV%2BhGnFfPYFQa4rCSkvZGKFi5KXndL13yEFg%2Fv2GnDO4wE%2BvKDRuZnHUl%2Fet5caGLsIuJgVfFxa6rFG6l5mWyMK0NSf8p67T%2F6smelEJ0JSCsoDO7cCA019KzbMOn5wtiF9QmJrQBJ1ytvtP8m%2FTrwaCiK0Fnj%2F0Qwn%2B8AUUfkUdWqw1vUMJ600Y62%2BTmYJe%2BbRKfO7yb0AXPHVMV%2BQKAUjwFp7G4aSCAaBmmln8%2FfBwpUQDxh7hx3QBC%2BVNUmBEuY%3D; s2=Qsofv7DNnrKj035yBVmZ9luHmr69NIVOK5LrP2hrb7Bk7tV56iRkFI65%2BhGve59uDAinuU7%2F9a6Llfg5S7UNlKDmTazwwLpYPvwzG%2Bz8Za5%2BuCd3pdMZPYyh48CPNWHwMbP%2BpafuWrB8dESPb3PiWmLOIeAarkFnNgZgJ0j9wzP2ZJ54AKSsT8YElD8g5rh%2FqU8MGtgYLoNANppP14nilHYy1IhjQFiEoDD3cMCQxoGGwojLFjT6O5QwPNFj3IjjFAY5TnjBmliTxI%2BxP43N1GRJAyvM2X4Cx0QBDyfdjJ396t4CHyAqwbdyDA8o9cNgwsXpLjE8nd9H6woz%2B3rzD%2BhWvohytz7IBrmXQsh2RkjBdbZxHWY0FxWAz3xpa51amMH5UuT7n8Uk7%2FXJqODY%2BOEuZ7v%2F2Sh7WbvhhKEFjoUULNJILAsWg7i23p4HPQ4S4Oun7t0%2FYEA2mOUTakXenGUyO7CrdUr1yiGNfzLfZkZT5viENNfeCDvpsty%2Bga2vxoPzLiUcBZ%2F3RHiko%2B1%2FJitdY39vjT5GGl46nRZkjpjxlqwY3KPep%2BzWPkqMpy9J0h3BEXzVxB6z0FXHNGBuL63mx%2BWEGrSliqTphxunVC6CRIdAJrSkS%2FeDSTTDg7gwktt1hTHtIhxBWO%2F5ijNYvonoEawDdQMUsflp0Xwz%2FZs%3D");
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
