#include "twitter.h"
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
   	m_runnum = 0;
}

void Twitter::updateUserTimeline()
{
	QUrl url("https://api.twitter.com/1.1/statuses/user_timeline.json");
	QVariantMap parameters;
	parameters.insert("count", 200);
	QNetworkReply *reply = get(url, parameters);

	QTimer timer;
	timer.setSingleShot(true);
	QEventLoop loop;

	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();
	
	QJsonParseError parseError;
	Q_ASSERT(reply);
        const auto data = reply->readAll();

	const auto document = QJsonDocument::fromJson(data, &parseError);
	if (parseError.error)
	{
		qCritical() << "parse reply error at:" << parseError.offset << parseError.errorString();
                reply->close();
                reply->abort();
		return;
	}
	else if (document.isObject())
	{
		const auto object = document.object();
		const auto errorArray = object.value("errors").toArray();
		Q_ASSERT_X(errorArray.size(), "parse", data);
		QStringList errors;
		for (const auto error : errorArray)
		{
			Q_ASSERT(error.isObject());
			Q_ASSERT(error.toObject().contains("message"));
			errors.append(error.toObject().value("message").toString());
		}
                reply->close();
                reply->abort();
		return;
	}

	m_tweets.clear();
	Q_ASSERT_X(document.isArray(), "parse", data);
	const auto array = document.array();
	if (array.size()) 
	{
		auto before = m_tweets.begin();
		for (const auto &value : array)
		{
			Q_ASSERT(value.isObject());
			const auto object = value.toObject();
			auto locale = QLocale(QLocale::English, QLocale::UnitedStates);
			const auto createdAt =
				locale.toDateTime(object.value("created_at").toString(), "ddd MMM dd HH:mm:ss +0000 yyyy");
				before = m_tweets.insert(before,
				Tweet{ object.value("id_str").toString(),
				createdAt,
					object.value("user").toObject().value("screen_name").toString(),
					object.value("user").toObject().value("id_str").toString(),
					object.value("user").toObject().value("followers_count").toInt(),
					object.value("text").toString(),
					object.value("in_reply_to_status_id_str").toString() });
			std::advance(before, 1);
		}
	}
        reply->close();
        reply->abort();
}

void Twitter::updateMentionsTimeline()
{
	QUrl url("https://api.twitter.com/1.1/statuses/mentions_timeline.json");
	QVariantMap parameters;
	parameters.insert("count", 200);

	QNetworkReply *reply = get(url, parameters);
	QTimer timer;
	timer.setSingleShot(true);
	QEventLoop loop;

	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();
	QJsonParseError parseError;
	Q_ASSERT(reply);
	const auto data = reply->readAll();
	
	const auto document = QJsonDocument::fromJson(data, &parseError);
	if (parseError.error)
	{
		qCritical() << "parse reply error at:" << parseError.offset << parseError.errorString();
		reply->close();
		reply->abort();
		return;
	}
	else if (document.isObject())
	{
		const auto object = document.object();
		const auto errorArray = object.value("errors").toArray();
		Q_ASSERT_X(errorArray.size(), "parse", data);
		QStringList errors;
		for (const auto error : errorArray) {
			Q_ASSERT(error.isObject());
			Q_ASSERT(error.toObject().contains("message"));
			errors.append(error.toObject().value("message").toString());
		}
                reply->close();
                reply->abort();
		return;
	}
	m_MentionsTweets.clear();

	Q_ASSERT_X(document.isArray(), "parse", data);
	const auto array = document.array();
	if (array.size())
	{
		auto before = m_MentionsTweets.begin();
		for (const auto &value : array) 
		{
			Q_ASSERT(value.isObject());
			const auto object = value.toObject();
			auto locale = QLocale(QLocale::English, QLocale::UnitedStates);
			const auto createdAt = locale.toDateTime(object.value("created_at").toString(), "ddd MMM dd HH:mm:ss +0000 yyyy");
			before = m_MentionsTweets.insert(before,
				Tweet {object.value("id_str").toString(),
				createdAt,
				object.value("user").toObject().value("screen_name").toString(),
				object.value("user").toObject().value("id_str").toString(),
				object.value("user").toObject().value("followers_count").toInt(),
				object.value("text").toString(),
				object.value("in_reply_to_status_id_str").toString() });
                std::advance(before, 1);
		}
	}

        reply->close();
        reply->abort();
}

void Twitter::statusUpdate(QString content)
{
    QUrl url("https://api.twitter.com/1.1/statuses/update.json");
    QVariantMap parameters;
    parameters.insert("status", content);
	//parameters.insert("media_ids", "1106392519841992704");
    QNetworkReply *reply = post(url, parameters);
    connect(reply, &QNetworkReply::finished, this, [&]() {
        auto reply = qobject_cast<QNetworkReply *>(sender());
        Q_ASSERT(reply);
        const auto data = reply->readAll();
        qDebug() << "statusUpdate reply:" << data;
		//updateMentionsTimeline();
    });
}

void Twitter::retweeters(const QString& id, QString& retweeters_str)
{
    QUrl url("https://api.twitter.com/1.1/statuses/retweeters/ids.json");
    QVariantMap parameters;
    parameters.insert("id", id);

    QNetworkReply *reply = get(url, parameters);

    QTimer timer;
    timer.setSingleShot(true);
    QEventLoop loop;

    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    const auto data = reply->readAll();
    retweeters_str = data;
    //reply->close();
    //reply->abort();
//    qDebug() << "retweeters .." << data;

}

void Twitter::reply(QString id, QString content)
{
    qDebug() <<"reply"<< id<<""<<content;

    QUrl url("https://api.twitter.com/1.1/statuses/update.json");
    QVariantMap parameters;
    parameters.insert("status", content);
    parameters.insert("in_reply_to_status_id", id);
    parameters.insert("batch_mode", "off");
	parameters.insert("auto_populate_reply_metadata", "true");

    QNetworkReply *reply = post(url, parameters);
    connect(reply, &QNetworkReply::finished, this, [&]() {
        auto reply = qobject_cast<QNetworkReply *>(sender());
        Q_ASSERT(reply);
        const auto data = reply->readAll();
//        qDebug() << "reply:" << data<<"\r\n\r\n\r\n";
    });
}



void Twitter::search( QString content)
{

	QUrl url("https://api.twitter.com/1.1/search/tweets.json");
	QVariantMap parameters;
	parameters.insert("q", content);

	QNetworkReply *reply = get(url, parameters);

	QTimer timer;
	timer.setSingleShot(true);
	QEventLoop loop;

	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();
	QByteArray responseData;
	responseData = reply->readAll();
	const auto dataweb = QString(responseData);

		QString datastr = dataweb;
		QStringList stlist = datastr.split(",\"text\":");
		for (int i = 1; i < stlist.size(); i++)
		{
			QString str = stlist.at(i);
			int p = str.indexOf("\",\"", 10);
			QString text = str.mid(1, p - 1);


			if (text.indexOf("RT") != -1)
				text = str_rt(text) ;

			text = text.replace("\u2019", "'");
			text = text.replace("\\u"," *u");
			text = text.replace("\\n", " ");

			QStringList text_list = text.split(" ");
			text_list = list_at(text_list);

			if (text_list.size() <= 5)
				continue;

		
			QString str_statusUpdate = getstr_list(text_list);	
			if (str_statusUpdate.toLower().indexOf("retweet") != -1&& str_statusUpdate.toLower().indexOf("like") != -1)
				continue;
			if (str_statusUpdate.toLower().indexOf("retweet") != -1 && str_statusUpdate.toLower().indexOf("follow") != -1)
				continue;
			if (str_statusUpdate.toLower().indexOf("like") != -1 && str_statusUpdate.toLower().indexOf("follow") != -1)
				continue;

			if (str_statusUpdate.length() <19)
				continue;
				
			statusUpdate(str_statusUpdate);
			QEventLoop eventloop;
			QTimer::singleShot(1000 * 26, &eventloop, SLOT(quit()));
			eventloop.exec();
		}
		
	//	qDebug() << "reply:" << data<<"\r\n\r\n\r\n";
	//});
}


QString Twitter::str_rt(QString & tweet)
{
	QString ret_str = tweet;
	if (tweet.mid(0, 5).indexOf("RT") != -1)
	{
		ret_str = tweet.mid(3);
	}
	return ret_str;
}

QStringList Twitter::list_at(QStringList & list_in)
{
	QStringList ret_list;
	for (int i = 0; i < list_in.size(); i++)
	{
		QString str = list_in.at(i);
		if (str.length() > 32)
			continue;
		if (str.indexOf("https") != -1)
			continue;;
		if (str.mid(0, 3).indexOf("@") != -1)
			continue;

		if (str.mid(0, 2).indexOf("*u") != -1 && str.length() == 6)
			continue;

		if (str.mid(0, 2).indexOf("*u") != -1 && str.length()>6)
		{
			if (str.at(1).isDigit()|| str.at(2).isDigit() || str.at(3).isDigit() || str.at(4).isDigit() )
				str = str.mid(6);
		}


		if (str.length() == 2)
		{
			if (str.at(0).isDigit() &&  str.at(1).isDigit() )
			continue;
		}


		if (str.mid(0, 2).indexOf("*u") != -1 && str.length() == 6)
			continue;


		ret_list.push_back(str);
		
	}
	return ret_list;
}


QString Twitter::getstr_list(QStringList & list_in)
{
	QString ret_str;
	for (int i = 0; i < list_in.size(); i++)
	{
		ret_str += list_in.at(i);
		ret_str += " ";
	}
	return ret_str;
}


QString Twitter::str_https(QString & id)
{
	QString u = "https://api.twitter.com/1.1/statuses/destroy/%1.json";
	QUrl url(u.arg(id));
	QVariantMap parameters;

	return u;
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
    id = "30,1092806072757338114";
    QString url = "https://api.twitter.com/1.1/statuses/lookup.json";
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
}

void Twitter::DoPerMyTweet( )
{
    if (m_MentionsTweets.count() * m_tweets.count() ==0)
    {
		qDebug() << "DoPerMyTweet " << m_MentionsTweets.count() << " " << m_tweets.count();
		return;
    }
	QString time = QDateTime::fromMSecsSinceEpoch(QDateTime::currentDateTime().toMSecsSinceEpoch() - m_MentionsTweets[0].createdAt.toMSecsSinceEpoch()).toUTC().toString("hh:mm:ss");


qDebug()<<time;
	if (time > "01:27:53")
	{

qDebug()<<time;
		QString con = R"(
https://yobit.net/en/trade/TVT/ETH
1. Follow @tvtokens 
2. Retweet and Like
3. Tag 3 friends
4. Comment Eth Address
	)";
//statusUpdate(con);

	}

	qDebug() << "DoPerMyTweet " << m_MentionsTweets.count() << " " << m_tweets.count();
	QMultiMap<QString, Tweet> _MentionsTweetsMap;
	for (int i = 0; i < m_MentionsTweets.count(); ++i)
	{
		Twitter::Tweet tweet = m_MentionsTweets[i];

		int ret = -1;
		for (size_t i = 0; i < m_MyTweets.size(); i++)
		{
			if (tweet.in_reply_to_status_id_str != m_MyTweets.at(i).id)
				ret = 1;
		}

		if (ret==-1)
		{
			if (IsTweetReply(tweet))
				continue;

			if (tweet.id <= m_lastSendId)
				continue;

			reply(tweet.id, "Please Comment Newest Twitter Again.");
			continue;
		}
	}
   
	for (size_t i = 0; i < m_MyTweets.size(); i++)
	{
		QString retweeters_str;
		retweeters(m_MyTweets.at(i).id, retweeters_str);
		if (retweeters_str.indexOf("errors") != -1)
			continue;
		if (retweeters_str.isEmpty())
				continue;	
		AirdropPerTweet(m_MyTweets.at(i).id,retweeters_str );
	}	
}

void Twitter::testmail()
{
	while (1)
	{
		search("#btc");
		search("#bitcoin");
		search("#eth");
		search("#ethereum");
		search("#crypto");
		search("#cryptocurrency");

	
		search("bitcoin");	
		search("ethereum");
		search("crypto");
		search("cryptocurrency");

		//updateUserTimeline();
		//updateMentionsTimeline();
		//GetMyTwitterId();
  //      m_lastSendId = GetLastSendId();
		//DoPerMyTweet();
        QEventLoop eventloop;
        QTimer::singleShot(1000 * 60, &eventloop, SLOT(quit()));
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

	QRegularExpression  ethREX("0x+[a-z0-9._%+-]{40,45}");
	match = ethREX.match(text.toLower());
	if (match.hasMatch())
	{	
		address = match.captured(0);
		return 3;
	}

    return 4;
}

QString Twitter::GetLastSendId()
{
	QString id ;
	for (int i = 0; i < m_tweets.count(); ++i)
	{
		Twitter::Tweet tweet = m_tweets[i];
		if (m_tweets[i].text.indexOf("etherscan.io/tx") != -1)
		{
			id = m_tweets[i].id;
			return id;
		}

		if (m_tweets[i].text.indexOf("vt Successfully Sent. Please Check It") != -1)
		{
			id = m_tweets[i].id;
			return id;
		}

		if (m_tweets[i].text.indexOf("Eth Go To Moon") != -1)
		{
			id = m_tweets[i].id;
			return id;
		}

		if (m_tweets[i].text.indexOf("lease Comment Mercatox E-mail or E-Walle") != -1)
		{
			id = m_tweets[i].id;
			return id;
		}

		if (m_tweets[i].text.indexOf("Please Retweet The Tweet.") != -1)
		{
			id = m_tweets[i].id;
			return id;
		}
	}	
	return id;
}

void Twitter::GetMyTwitterId()
{	
	m_MyTweets.clear();
	for (int i = 0; i < m_tweets.count(); ++i)
	{	
		Twitter::Tweet tweet = m_tweets[i];
		if (m_tweets[i].in_reply_to_status_id_str.isEmpty())
		{
			if (m_tweets[i].text.mid(0,6).indexOf("RT @")!=-1)
				continue;
			m_MyTweets.append(tweet);
		}
	}
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

int Twitter::MyTweetsCount()
{
	int ret = 0;
	for (int i = 0; i < m_MentionsTweets.count(); ++i)
	{
		if (m_tweets[i].in_reply_to_status_id_str.isEmpty())
		{
			ret++;
		}		
	}
	return ret;
}

void Twitter::AirdropPerTweet(const QString& MyTweetid, const QString& fourlws)
{
	QMultiMap<QString, Tweet> _MentionsTweetsMap;
	for (int i = 0; i < m_MentionsTweets.count(); ++i)
	{
		Twitter::Tweet tweet = m_MentionsTweets[i];

		if (tweet.in_reply_to_status_id_str != MyTweetid)
			continue;	
		_MentionsTweetsMap.insert(tweet.user, tweet);
	}

	QMultiMap<QString, Tweet>::iterator iter = _MentionsTweetsMap.begin();
	for (; iter != _MentionsTweetsMap.end(); )
	{
		Twitter::Tweet tweet = *iter;
		QList<Tweet> map = _MentionsTweetsMap.values(iter->user);
		auto lower = _MentionsTweetsMap.lowerBound(iter->user);
		auto upper = _MentionsTweetsMap.upperBound(iter->user);
		while (lower != upper)
		{
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
			if (ret == 3)
			{
				if (one == 1)
				{
				//	reply(tweet.id, "Btc & Eth Go To Moon.");
					continue;
				}
				one = 1;
				
		//		qDebug() << "AirdropPerTweet user_id_str[" << tweet.user_id_str << "] [" << fourlws ;
				if (fourlws.indexOf(tweet.user_id_str)==-1)
				{       
					reply(tweet.id, "Please Retweet The Tweet.");
					continue;
				}

				QString out;
			
				//else
				//	reply(tweet.id, "tvt Successfully Sent. Please Check It");
				continue;
			}

			if (ret == 3)
			{
				reply(tweet.id, "Please Comment Mercatox E-mail or E-Wallet ID.");
				continue;
			}
			reply(tweet.id, "Happy Join To Moon.");
		}
	}
}

int Twitter::DoTestMail(QString & out)
{
	QEventLoop loop;
	QTimer::singleShot(3000, &loop, SLOT(quit()));
	loop.exec();

	qDebug() << "DoTestMail";
	int ret = 0;
	

	//if(ret ==1)
	//{		
	//	qDebug() << "DoTestMail seucess" ;
	//	QBizManager::GetInstance().WriteCookie();
	//	return ret;
	//}

	//if (ret == 3)
	//{
	//	QBizManager::GetInstance().do_cf_clearance();
	//}	
	return ret;
}
