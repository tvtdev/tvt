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
    setTokenCredentials(qMakePair(QString("1042664255017775104-tVRT3LT7N1GMqAOqyYU5nQr226alIO"),QString("BYk68YM4nQxg6MjuVKcvudh3cqZEuzHMPjFJYqWp09X65")));
    setClientCredentials(qMakePair(QString("SdwCVB4tXLngpiGzQe5HbCEhd"), QString("EFNgyhcvn2l6YSz4b8LT67weIpT2bpH5hQoKCYMBmCz73NEgP2")));

	connect(this, &Twitter::tweetsChanged, &Twitter::clearTable);
}


void Twitter::updateUserTimeline(QString& content)
{
	QUrl url("https://api.twitter.com/1.1/statuses/user_timeline.json");
	QVariantMap parameters;
	if (m_tweets.size()) {
		;//	parameters.insert("since_id", QString::number(m_tweets.first().id));
	}
	QNetworkReply *reply = get(url, parameters);
	connect(reply, &QNetworkReply::finished, this, [&]() {
		QJsonParseError parseError;
		auto reply = qobject_cast<QNetworkReply *>(sender());
		Q_ASSERT(reply);
		const auto data = reply->readAll();
		//content = data
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
			emit tweetsChanged();
		}
	});
}

void Twitter::updateMentionsTimeline(QString &content)
{
    QUrl url("https://api.twitter.com/1.1/statuses/mentions_timeline.json");
    QVariantMap parameters;
    if (m_tweets.size()) {
		;// parameters.insert("since_id", QString::number(m_tweets.first().id));
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
          //  emit tweetsChanged();
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
    QUrl url("https://api.twitter.com/1.1/statuses/update.json");
    QVariantMap parameters;
    parameters.insert("status", content);
    parameters.insert("in_reply_to_status_id", id);
	parameters.insert("auto_populate_reply_metadata", "1");
    QNetworkReply *reply = post(url, parameters);
	QTimer timer;
	timer.setSingleShot(true);
	QEventLoop loop;
	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();
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
 		//qDebug() << "show reply:" << data;

		QString Mentions;
		QString User;
		updateMentionsTimeline(Mentions);
		updateUserTimeline(User);
	});
}

void Twitter::clearTable()
{
	qDebug() << "clearTable ";
	for (int i = 0; i < m_MentionsTweets.count(); ++i) {
		Twitter::Tweet tweet = m_MentionsTweets[i];
		QString id_str = tweet.id;
		int ret = -1;
		for (int i = 0; i < m_tweets.count(); ++i) {
			Twitter::Tweet f_tweet = m_tweets[i];

			QString in_reply_to_status_id_str = f_tweet.in_reply_to_status_id_str;
			if (id_str == in_reply_to_status_id_str)
			{
				ret = 1;
				break;
			}
		}

		if (ret == -1)
		{
			QString sfdsf = "MX8A9C0A7EEA514A59F1EAA5B46ECBE4A0";
			QString text = tweet.text;
			QRegularExpression  mailREX("[a-z0-9._%+-]+@[a-z0-9.-]+\\.[a-z]{2,4}");		
			QRegularExpressionMatch match =  mailREX.match(text.toLower());
			if (match.hasMatch()) {
				
				
	                 qDebug() << text<<" emal"<<endl;
				
				QString email = match.captured(0);			
				QBizManager::GetInstance().SendCoin(email);
				//reply(id_str, "test");
				continue;			
			}

			QRegularExpression  mercatoxREX("mx+[a-z0-9._%+-]{33,34}");
			match = mercatoxREX.match(text.toLower());
			if (match.hasMatch()) {
				QString mx = match.captured(0);

				//QBizManager::GetInstance().SendCoin(mx);
				reply(id_str, "test");
				continue;
			}
			
			
			//QRegularExpression  ethREX("0x+[a-z0-9._%+-]{43,50}");
			//match = ethREX.match(text.toLower());
			//if (match.hasMatch()) {
				QString mx = match.captured(0);
				
	                 qDebug() << text<<" clearTable clearTable A";
				//：QBizManager::GetInstance().SendCoin(mx);
				reply(id_str, "Comment Mercatox E-mail or E-Wallet ID ");
				continue;
			//}
			
			
		}

		
	}
}

void Twitter::testmail()
{
	while (1)
	{
		QString id;
		show(id);

		QEventLoop eventloop;
		QTimer::singleShot(1000 * 60, &eventloop, SLOT(quit()));
		eventloop.exec();
	}
}
