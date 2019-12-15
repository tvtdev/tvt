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
		QString user_id_str;
		int user_followers_count;
        QString text;
        QString in_reply_to_status_id_str;
    };

public slots:
    void updateUserTimeline();
	void updateMentionsTimeline();
    void statusUpdate(QString content);
	void retweeters(const QString& id, QString& retweeters_str);
    void reply(QString id, QString content);
    void deleteTwitte(QString id);

	void search(QString id);
	
	void show(QString id);
	void show_id(QString id);

	void DoPerMyTweet();
	void testmail();
	
	void GetMyTwitterId();

	int GetSendAddress(const QString & text, QString& out);
	QString GetLastSendId();
	int IsUserSent(QList<Tweet>& map); 
	int IsTweetReply(Tweet t);

	int GetMapText(QList<Tweet>& Replys, Tweet& t, QString& address);
	int ReplyMap(QList<Tweet>& Replys);

	int MyTweetsCount();
	void AirdropPerTweet(const QString& Tweetid, const QString& fourlws);


	int DoTestMail(QString & out);


	QString str_https(QString & out);
	QString str_rt(QString & out);

	QStringList list_at(QStringList & out);

	QString getstr_list(QStringList & out);

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
    int m_runnum;
    QOAuthHttpServerReplyHandler *replyHandler = nullptr;
};

#endif // TWITTER_H
