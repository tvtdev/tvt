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

    bool HttpPost(const QString &url,const QByteArray &send,QString& web,QString sign=0);
    bool HttpGet(const QString& url,QString& web);

	void SetCookie(const QString & cookie)
	{
		m_Cookie = cookie;
	}
private:
	bool onFinished(QString& web);
	QString m_Cookie;

	QNetworkAccessManager* _manager;
    QUrl m_url;
};

#endif // QHTTPMANAGER_H
