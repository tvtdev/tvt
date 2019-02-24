#include "twitter.h"
#include "qbizmanager.h"
#include <QLocale>
Twitter::Twitter(QObject *parent)
    : QOAuth1(parent)
{
    replyHandler = new QOAuthHttpServerReplyHandler(1337, this);
    replyHandler->setCallbackPath("callback");
    setReplyHandler(replyHandler);
    setTemporaryCredentialsUrl(QUrl("https://api.twitter.com/oauth/request_token"));
    setAuthorizationUrl(QUrl("https://api.twitter.com/oauth/authenticate"));
    setTokenCredentialsUrl(QUrl("https://api.twitter.com/oauth/access_token"));
    setTokenCredentials(qMakePair(QString("1042664255017775104-6VTghZ9Vp3Z9OKItJlHpMVHvxWyToe"),QString("turXH0qQiPaS9xcKxic4DuptlVbAlUYMaAaEtOIU6cJZS")));
    setClientCredentials(qMakePair(QString("qBoLfhBCZoOd94LPedZ9zka05"), QString("GQwLR4DDboM14k9ukJdhyCzGzvqIBY6CJ7SOW0isKGLWZuibSA")));
    connect(this, &Twitter::tweetsChanged, &Twitter::clearTable);
}


void Twitter::updateUserTimeline()
{
	QUrl url("https://api.twitter.com/1.1/statuses/user_timeline.json");
	QVariantMap parameters;
	if (m_tweets.size()) {
		parameters.insert("since_id", m_tweets.first().id);
	}
	QNetworkReply *reply = get(url, parameters);
	connect(reply, &QNetworkReply::finished, this, [&]() {
		QJsonParseError parseError;
		auto reply = qobject_cast<QNetworkReply *>(sender());
		Q_ASSERT(reply);
		const auto data = reply->readAll();
		const auto document = QJsonDocument::fromJson(data, &parseError);
		if (parseError.error) {
			qCritical() << "parse reply error at:" << parseError.offset << parseError.errorString();
			return;
		}
		else if (document.isObject()) {
			const auto object = document.object();
			const auto errorArray = object.value("errors").toArray();
			Q_ASSERT_X(errorArray.size(), "parse", data);
			QStringList errors;
			for (const auto error : errorArray) {
				Q_ASSERT(error.isObject());
				Q_ASSERT(error.toObject().contains("message"));
				errors.append(error.toObject().value("message").toString());
			}
			return;
		}
		m_tweets.clear();
		Q_ASSERT_X(document.isArray(), "parse", data);
		const auto array = document.array();
		if (array.size()) {
			auto before = m_tweets.begin();
			for (const auto &value : array) {
				Q_ASSERT(value.isObject());
				const auto object = value.toObject();
				auto locale = QLocale(QLocale::English, QLocale::UnitedStates);
				const auto createdAt =
					locale.toDateTime(object.value("created_at").toString(), "ddd MMM dd HH:mm:ss +0000 yyyy");
				    before = m_tweets.insert(before,
					Tweet{ object.value("id_str").toString(),
					createdAt,
					object.value("user").toObject().value("screen_name").toString(),
					object.value("text").toString(),
					object.value("in_reply_to_status_id_str").toString() });
				std::advance(before, 1);
			}
			updateMentionsTimeline();
			//emit tweetsChanged();
		}
	});
}

void Twitter::updateMentionsTimeline()
{
    QUrl url("https://api.twitter.com/1.1/statuses/mentions_timeline.json");
    QVariantMap parameters;
   
	QString id ;
	for (int i = 0; i < m_tweets.count(); ++i)
	{
		Twitter::Tweet tweet = m_tweets[i];
		if (m_tweets[i].text.indexOf("vt Successfully Sent. Please Check It")!=-1)
		{	
			id = m_tweets[i].id;
			break;
		}
	}

    QNetworkReply *reply = get(url, parameters);
    connect(reply, &QNetworkReply::finished, this, [&]() {
        QJsonParseError parseError;
        auto reply = qobject_cast<QNetworkReply *>(sender());
        Q_ASSERT(reply);
        const auto data = reply->readAll();
																																		        const auto document = QJsonDocument::fromJson(data, &parseError);
        if (parseError.error) {
            qCritical() << "parse reply error at:" << parseError.offset << parseError.errorString();
            return;
        }
        else if (document.isObject()) {
            // Error received :(
            const auto object = document.object();
            const auto errorArray = object.value("errors").toArray();
            Q_ASSERT_X(errorArray.size(), "parse", data);
            QStringList errors;
            for (const auto error : errorArray) {
                Q_ASSERT(error.isObject());
                Q_ASSERT(error.toObject().contains("message"));
                errors.append(error.toObject().value("message").toString());
            }
            return;
        }
        m_MentionsTweets.clear();

        Q_ASSERT_X(document.isArray(), "parse", data);
        const auto array = document.array();
        if (array.size()) {
            auto before = m_MentionsTweets.begin();
            for (const auto &value : array) {
                Q_ASSERT(value.isObject());
                const auto object = value.toObject();
                auto locale = QLocale(QLocale::English, QLocale::UnitedStates);
                const auto createdAt = locale.toDateTime(object.value("created_at").toString(), "ddd MMM dd HH:mm:ss +0000 yyyy");
                before = m_MentionsTweets.insert(before,
                                         Tweet {object.value("id_str").toString(),
                                                createdAt,
                                                object.value("user").toObject().value("screen_name").toString(),
                                                object.value("text").toString(),
												object.value("in_reply_to_status_id_str").toString() });
                std::advance(before, 1);
            }
            emit tweetsChanged();
        }
    });
}

void Twitter::statusUpdate(QString content)
{
    show(content);
    return;
    QUrl url("https://api.twitter.com/1.1/statuses/update.json");
    QVariantMap parameters;
    parameters.insert("status", content);
    QNetworkReply *reply = post(url, parameters);
    connect(reply, &QNetworkReply::finished, this, [&]() {
        auto reply = qobject_cast<QNetworkReply *>(sender());
        Q_ASSERT(reply);
        const auto data = reply->readAll();
        qDebug() << "statusUpdate reply:" << data;
		//updateMentionsTimeline();
    });
}

void Twitter::retwitte(QString id)
{
    QString u = "https://api.twitter.com/1.1/statuses/retweets/%1.json";
    QUrl url(u.arg(id));
    QVariantMap parameters;

    QNetworkReply *reply = get(url, parameters);
    connect(reply, &QNetworkReply::finished, this, [&]() {
        auto reply = qobject_cast<QNetworkReply *>(sender());
        Q_ASSERT(reply);
        const auto data = reply->readAll();
        qDebug() << "Retwitte reply:" << data;
    });
}

void Twitter::reply(QString id, QString content)
{
    qDebug() <<"reply"<< id<<""<<content;

    QUrl url("https://api.twitter.com/1.1/statuses/update.json");
    QVariantMap parameters;
    parameters.insert("status", content);
    parameters.insert("in_reply_to_status_id", id);
    parameters.insert("auto_populate_reply_metadata", "1");
    QNetworkReply *reply = post(url, parameters);
    connect(reply, &QNetworkReply::finished, this, [&]() {
        auto reply = qobject_cast<QNetworkReply *>(sender());
        Q_ASSERT(reply);
        const auto data = reply->readAll();
        //qDebug() << "reply:" << data<<"\r\n\r\n\r\n";
    });
}

void Twitter::deleteTwitte(QString id)
{
    QString u = "https://api.twitter.com/1.1/statuses/destroy/%1.json";
    QUrl url(u.arg(id));
    QVariantMap parameters;

    QNetworkReply *reply = post(url, parameters);
    connect(reply, &QNetworkReply::finished, this, [&]() {
        auto reply = qobject_cast<QNetworkReply *>(sender());
        Q_ASSERT(reply);
        const auto data = reply->readAll();
        qDebug() << "delete reply:" << data;
		//updateMentionsTimeline();
    });
}

void Twitter::show(QString id)
{
    id = "1092806072757338114";
    QString url = "https://api.twitter.com/1.1/statuses/show.json";
    QVariantMap parameters;
    parameters.insert("id", id);
	
    QNetworkReply *reply = get(url, parameters);
    connect(reply, &QNetworkReply::finished, this, [&]() {
            auto reply = qobject_cast<QNetworkReply *>(sender());
            Q_ASSERT(reply);
            const auto data = reply->readAll();
            QString Mentions;
            QString User;
            updateMentionsTimeline();
            updateUserTimeline();
    });
}

void Twitter::show_id(QString id)
{
	QString url = "https://api.twitter.com/1.1/statuses/show.json";
	QVariantMap parameters;
	parameters.insert("id", id);

	QNetworkReply *reply = get(url, parameters);

	QTimer timer;
	timer.setSingleShot(true);
	QEventLoop loop;

	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	QByteArray responseData;
	responseData = reply->readAll();
	const auto dataweb = QString(responseData);
	reply->close();
	reply->abort();

	//connect(reply, &QNetworkReply::finished, this, [&]() {
	//	auto reply = qobject_cast<QNetworkReply *>(sender());
	//	Q_ASSERT(reply);
	//	const auto data = reply->readAll();
	//	QString Mentions;
	//	QString User;
	//	updateMentionsTimeline(Mentions);
	//	updateUserTimeline(User);
	//});
}

void Twitter::clearTable()
{
    if (m_MentionsTweets.count() * m_tweets.count() ==0)
    {
		qDebug() << "clearTable 0" << m_MentionsTweets.count() << " " << m_tweets.count();
		return;
    }

	m_lastSendId = GetLastSendId();
	if (m_lastSendId.isEmpty())
	{
		qDebug() << "clearTable m_lastSendId.isEmpty" ;
		return;
	}


    qDebug() << "clearTable "<<m_MentionsTweets.count()<<" "<<m_tweets.count();
	QMultiMap<QString, Tweet> m_MentionsTweetsMap;
	for (int i = 0; i < m_MentionsTweets.count(); ++i)
	{
		Twitter::Tweet tweet = m_MentionsTweets[i];

		if (tweet.in_reply_to_status_id_str != "1098274577275187201")
			continue;   

		qDebug() << "text id map " <<  tweet.user <<"  "<< tweet.text.mid(0,30);
		m_MentionsTweetsMap.insert(tweet.user,tweet);
	}
	
	QMultiMap<QString, Tweet>::iterator iter = m_MentionsTweetsMap.begin();
	for ( ; iter != m_MentionsTweetsMap.end(); ) 
	{
		Twitter::Tweet tweet = *iter;
		
		QList<Tweet> map = m_MentionsTweetsMap.values(iter->user);
		auto lower = m_MentionsTweetsMap.lowerBound(iter->user);
		auto upper = m_MentionsTweetsMap.upperBound(iter->user);
		while (lower != upper)
		{
            qDebug() << "user" << iter->user << " id " << iter->id<< " text " << lower->text.mid(0, 90);
			iter++;
			lower++;
		}
		
		if (IsUserSent(map))
		{
			ReplyMap(map);
			continue;
		}	

		
		QString address;
		int  one = -1;
		int ret = -1;
		for (int i = 0; i < map.count(); ++i)
		{
			Twitter::Tweet tweet = map[i];

			if (IsTweetReply(tweet))
				continue;

			if (tweet.id <= m_lastSendId)
				continue;

			ret = GetSendAddress(tweet.text, address);
			if (ret == 1 || ret == 2)
			{
				if (one == 1)
				{
					reply(tweet.id, "Btc & Eth Go To Moon.");
					continue;
				}
				one = 1;
				QString out;
				if (QBizManager::GetInstance().SendCoin(address, out))
				{
					reply(tweet.id, out);
					continue;
				}				
			}

			if (ret == 3)
			{
			    reply(tweet.id, "Please Comment Mercatox E-mail or E-Wallet ID.");
				continue;
			}

			reply(tweet.id, "Btc & Eth Go To Moon.");
		}

	
    }
}

void Twitter::testmail()
{
	while (1)
        {
		QString id;
		updateUserTimeline();

        QEventLoop eventloop;
        QTimer::singleShot(1000 * 120, &eventloop, SLOT(quit()));
		eventloop.exec();
	}
}

int Twitter::GetSendAddress(const QString & text, QString& address)
{
	QRegularExpression  mailREX("[a-z0-9._%+-]+@[a-z0-9.-]+\\.[a-z]{2,4}");
	QRegularExpressionMatch match = mailREX.match(text.toLower());
	if (match.hasMatch()) 
	{
		address = match.captured(0);
		address.replace("@", "%40");
		return 1;
	}

	QRegularExpression  mercatoxREX("mx+[a-z0-9._%+-]{30,34}");
	match = mercatoxREX.match(text.toLower());
	if (match.hasMatch())
	{
		address = match.captured(0);
		return 2;
	}

	QRegularExpression  ethREX("0x+[a-z0-9._%+-]{32,45}");
	match = ethREX.match(text.toLower());
	if (match.hasMatch())
	{		
		return 3;
	}

        return 4;
}

QString Twitter::GetLastSendId()
{
	QString id;
	for (int i = 0; i < m_tweets.count(); ++i)
	{
		Twitter::Tweet tweet = m_tweets[i];
		if (m_tweets[i].text.indexOf("vt Successfully Sent. Please Check It") != -1)
		{
			id = m_tweets[i].id;
			break;
		}
	}
	return id;

}

int Twitter::IsUserSent(QList<Tweet>& Replys)
{
    for (int i = 0; i < Replys.count(); ++i)
        {
                Twitter::Tweet tweet = Replys[i];
                for (int n = 0; n < m_tweets.count(); n++)
                {
                        Twitter::Tweet f_tweet = m_tweets[n];
                        if (tweet.id == f_tweet.in_reply_to_status_id_str)
                        {
                                if (m_tweets[i].text.indexOf("vt Successfully Sent. Please Check It") != -1)
                                {
                                    return 1;
                                }
								
                        }
                }
        }
        qDebug() << "IsUserReply "<<Replys.count();

        return 0;
}


int Twitter::IsTweetReply(Tweet tweet)
{
	for (int n = 0; n < m_tweets.count(); n++)
	{
		Twitter::Tweet f_tweet = m_tweets[n];
		if (tweet.id == f_tweet.in_reply_to_status_id_str)
		{
			return 1;
		}
	}
	return 0;
}


int Twitter::GetMapText(QList<Tweet>& Replys, Tweet& t, QString& address)
{
	int ret = -1;
	for (int i = 0; i < Replys.count(); ++i)
	{
		Twitter::Tweet tweet = Replys[i];

		ret  =	GetSendAddress(tweet.text,address);
		if (ret == 1 || ret == 2)
		{
			t  = tweet;
			break;
        }

		if (ret == 3)
		{
			if (tweet.id <= m_lastSendId)
				continue;

			reply(tweet.id, "Please Comment Mercatox E-mail or E-Wallet ID.");
		}
	}
	return ret;
}

int Twitter::ReplyMap(QList<Tweet>& Replys)
{
	for (int i = 0; i < Replys.count(); ++i)
	{
		Twitter::Tweet tweet = Replys[i];

		if (IsTweetReply(tweet))
				continue;

		if (tweet.id <= m_lastSendId)
			continue;

		reply(tweet.id, "Btc & Eth Go To Moon.");

	}
	return 0;
}
