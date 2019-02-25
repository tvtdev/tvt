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
	void get_statuses_home_timeline();
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
	
	void GetMyTwitterId();

	int GetSendAddress(const QString & text, QString& out);
	QString GetLastSendId();
	int IsUserSent(QList<Tweet>& map); 
	int IsTweetReply(Tweet t);

	int GetMapText(QList<Tweet>& Replys, Tweet& t, QString& address);
	int ReplyMap(QList<Tweet>& Replys);

	int MyTweetsCount();
	void AirdropPerTweet(const QString& Tweetid);

signals:
    void tweetsChanged();

private:
    Q_DISABLE_COPY(Twitter)
		
	QList<Tweet> m_MyTweets;
    QList<Tweet> m_tweets;
    QList<Tweet> m_MentionsTweets;
	QList<Tweet> m_home_timeline_Tweets;

    QString m_lastSendId;
	QString m_MyLastTwitterId;

    QOAuthHttpServerReplyHandler *replyHandler = nullptr;
};

#endif // TWITTER_H
