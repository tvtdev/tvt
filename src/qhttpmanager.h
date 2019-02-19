#ifndef QHTTPMANAGER_H
#define QHTTPMANAGER_H
#include <QObject>
#include <QtNetwork>
#include <QNetworkAccessManager>

class QHttpManager  : public QObject
{
    Q_OBJECT
public:
    QHttpManager( QObject *parent = 0);
public:
    static   QHttpManager&							GetInstance(){static QHttpManager cbm;return cbm;}

	bool HttpPost(const QString &url, const QByteArray &send, QString& web, QString sign = 0);
    bool HttpGet(const QString& url,QString& web);
	bool HttpGet_ht(const QString& url, QString& web, QString& cfuid);
	bool HttpGet_twitter(const QString& url, QString& web);


	bool HttpPost_email(const QString &url, const QByteArray &send, QString& web);
	bool HttpPost_bitcointalk(const QString &url, const QByteArray &send, const QString &str, QString& web);
	bool HttpPost_retweet(const QString &url, const QByteArray &send, const QString &authorization, QString& web);
	bool HttpPost_Transaction(const QString &url, const QByteArray &send, const QString &authorization, QString& web);

	bool HttpGet_bitcointalk(const QString& url, QString& web);

	void setCookie(const QString & cookie)
	{
		m_Cookie = cookie;
	}

	bool HttpGet_t(const QString& url, QString& web);
	
private:
        bool onFinished(QString& web);
		QString m_Cookie;
		QString m_agent;


    QUrl m_url;
};

#endif // QHTTPMANAGER_H
