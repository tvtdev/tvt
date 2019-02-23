#ifndef TWITTER_H
#define TWITTER_H

#include <QObject>
#include <QtCore>
#include <QtNetwork>
#include <QtNetworkAuth>
#include <map>


class Twitter : public QOAuth1
{
    Q_OBJECT
public:
    Twitter(QObject *parent = nullptr);
    struct Tweet {
        QString id;
        QDateTime createdAt;
        QString user;
        QString text;
        QString in_reply_to_status_id_str;

    };

public slots:
    void updateUserTimeline();
	void updateMentionsTimeline();
    void statusUpdate(QString content);
    void retwitte(QString id);
    void reply(QString id, QString content);
    void deleteTwitte(QString id);

	void show(QString id);
	void show_id(QString id);

	void clearTable();
	void testmail();
	
	int GetSendAddress(const QString & text, QString& out);
	QString GetLastSendId();
	int IsUserReply(QList<Tweet>& map);
	int GetMapText(QList<Tweet>& Replys, Tweet& t, QString& address);
signals:
    void tweetsChanged();

private:
    Q_DISABLE_COPY(Twitter)

    QList<Tweet> m_tweets;
	QList<Tweet> m_MentionsTweets;


    QOAuthHttpServerReplyHandler *replyHandler = nullptr;
};

#endif // TWITTER_H
