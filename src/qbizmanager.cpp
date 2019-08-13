#include "qbizmanager.h"
#include "qhttpmanager.h"

void QBizManager::doTransfer(const QString & source)
{
	qDebug() << ".";
	QStringList buy_list;
	QStringList sell_list;
	GetPrice(source, buy_list, sell_list);
	QString price_sell = sell_list.at(0).split(",").at(0);
	QString amount_sell = sell_list.at(0).split(",").at(1);;
	QString price_buy = buy_list.at(0).split(",").at(0);
	QString amount_buy = buy_list.at(0).split(",").at(1);


	if (price_buy.toDouble() <= my_trade.low.toDouble())
	{
		qDebug() << "doTransfer 1.";
		if (amount_buy.toDouble() >= amount_sell.toDouble() * 1.1  && m_price_buy.length() == 0)
		{
			qDebug() << "doTransfer 1..";
			m_price_buy = price_buy;
			return;
		}

		if (amount_buy.toDouble() <= amount_sell.toDouble() && m_price_buy == price_buy && m_price_buy.length() >= 1)
		{
			m_price_buy = "";
			return;
		}

		if (price_buy >m_price_buy && price_buy.toDouble() - m_price_buy.toDouble() <= 2 && m_price_buy.length() != 0)
		{
			qDebug() << "doTransfer 1...";
			QUrlQuery param;
			param.addQueryItem("symbol", "XBTUSD");
			param.addQueryItem("orderQty", "1");
			param.addQueryItem("side", "Buy");
			param.addQueryItem("ordType", "Market");
			createOrder(param);
			m_price_buy = "";
		}
		else
		{
			m_price_buy = "";
		}
	}


	if (price_sell.toDouble() <= my_trade.high.toDouble())
	{
		qDebug() << "doTransfer 2.";
		if (amount_sell.toDouble() >= amount_buy.toDouble()*1.1    && m_price_sell.length() == 0)
		{
			qDebug() << "doTransfer 2..";
			m_price_sell = price_sell;
			return;
		}
		if (amount_sell.toDouble() <= amount_buy.toDouble() && m_price_sell == price_sell && m_price_sell.length() >= 1)
		{
			m_price_sell = "";
			return;
		}
		if (price_sell < m_price_sell && m_price_sell.toDouble() - price_sell.toDouble() <= 12 && m_price_sell.length() != 0)
		{
			qDebug() << "doTransfer 2...";
			QUrlQuery param;
			param.addQueryItem("symbol", "XBTUSD");
			param.addQueryItem("orderQty", "1");
			param.addQueryItem("side", "Sell");
			param.addQueryItem("ordType", "Market");
			createOrder(param);
			m_price_sell = "";
		}
		else
		{
			m_price_sell = "";
		}
	}
}

QBizManager::QBizManager()
{
	m_signed = false;
	m_currentTotalRequests = 0;
	m_maxRequests = 60;

	m_websocketHost = "wss://www.bitmex.com/realtime";
	connect(&m_webSocket, &QWebSocket::connected, this, &QBizManager::connected, Qt::UniqueConnection);
	connect(&m_webSocket, &QWebSocket::disconnected, this, &QBizManager::closed, Qt::UniqueConnection);
	connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &QBizManager::textMessageReceived, Qt::UniqueConnection);
	connect(&m_webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), [=](QAbstractSocket::SocketError error) {
		qDebug() << "web error:" << error;
		//m_webSocket.open(QUrl(m_websocketHost));
	});
	m_webSocket.open(QUrl(m_websocketHost));
	m_pingTimer.setInterval(1000 * 5);
	connect(&m_pingTimer, &QTimer::timeout, this, [&]() {
		if (m_webSocket.state() == QAbstractSocket::ConnectedState) {
			m_webSocket.ping("ping");
		}
		else {
			qDebug() << "Websocket is closed?please check.";
		}
	});

	m_TradeTimer.setInterval(1000 * 5);
	connect(&m_TradeTimer, &QTimer::timeout, this, &QBizManager::trade);
	m_TradeTimer.start();

	m_price_buy = "";
	m_price_sell = "";
	m_price_amount_buy = 0;
	m_price_amount_sell = 0;
	QString amouant = "3";// QString::number(my_postion.currentQty.toDouble()*0.5);
						  //amount = QString::number(amount.toInt());
	QString amount = QString::number(amouant.toDouble()*0.5, 'g', 0);


}

QBizManager::~QBizManager()
{
}

void QBizManager::connected()
{
	qDebug() << "websocket connected.";
	QString expires = QHttpManager::GetInstance().currentTimestamp();
	QString signature = QHttpManager::GetInstance().generateSignature("GET", "/realtime", "");
	m_webSocket.sendTextMessage(QString(R"({"op": "authKeyExpires", "args": ["%1",%2,"%3"]})").arg(m_apiId).arg(expires).arg(signature));
}

void QBizManager::closed()
{
	qDebug() << "websocket disconnected.";
	m_signed = false;
}

void QBizManager::textMessageReceived(const QString &message)
{
	QJsonDocument jDoc = QJsonDocument::fromJson(message.toUtf8());
	QJsonObject jObj = jDoc.object();
	if (!jObj.isEmpty()) {
		if (jObj.contains("request")) {
			auto rObj = jObj["request"].toObject();
			if (rObj.contains("op")) {
				auto op = rObj["op"].toString();
				if (op == WEBSOCKET_OP_AUTH) {
					m_pingTimer.start();
					m_webSocket.sendTextMessage(QString(R"({"op": "subscribe", "args": ["orderBook10:XBTUSD"]})"));
					m_webSocket.sendTextMessage(QString(R"({"op": "subscribe", "args": ["position"]})"));
				}
			}
		}
		else if (jObj.contains("table")) {
			if (message.indexOf("position") != -1)
			{
				GetPostion(message);

			}
			if (message.indexOf("orderBook10") != -1)
			{
				doTransfer(message);
			}
		}
	}
}


void QBizManager::queryWalletInfo(QString coinType)
{
	QUrlQuery q;
	q.addQueryItem("currency", coinType);
	QHttpManager::GetInstance().query("GET", FUNCTION_WALLET, q, REQUEST_WALLET, true);
}

void QBizManager::createOrder(QUrlQuery param)
{
	QHttpManager::GetInstance().query("POST", FUNCTION_CREATE_ORDER, param, REQUEST_CREATE_ORDER, true);
}

void QBizManager::queryAllOrder(QUrlQuery param)
{
	QHttpManager::GetInstance().query("GET", FUNCTION_QUERY_ORDER, param, REQUEST_QUERY_ORDER, true);
}


void QBizManager::isolatePosition(QUrlQuery param)
{
	QHttpManager::GetInstance().query("POST", FUNCTION_ISOLATE_POSITION, param, REQUEST_ISOLATE_POSITION, true);
}

void QBizManager::leveragePosition(QUrlQuery param)
{
	QHttpManager::GetInstance().query("POST", FUNCTION_LEVERAGE_POSITION, param, REQUEST_LEVERAGE_POSITION, true);
}

void QBizManager::riskLimitPosition(QUrlQuery param)
{
	QHttpManager::GetInstance().query("POST", FUNCTION_RISKLIMIT_POSITION, param, REQUEST_RISKLIMIT_POSITION, true);
}

void QBizManager::transferMarginPosition(QUrlQuery param)
{
	QHttpManager::GetInstance().query("POST", FUNCTION_TRANSFERMARGIN_POSITION, param, REQUEST_TRANSFERMARGIN_POSITION, true);
}
void QBizManager::cancelOrder(QString orderId, QString clOrderId, QString comment)
{
	QUrlQuery q;
	q.addQueryItem("orderID", orderId);
	q.addQueryItem("clOrdID", clOrderId);
	q.addQueryItem("text", comment);
	QHttpManager::GetInstance().query("DELETE", FUNCTION_CANCEL_ORDER, q, REQUEST_CANCEL_ORDER, true);
}

bool QBizManager::bitmex_depth(QString & source, QString coinType, QString depth)
{
	QUrlQuery q;
	q.addQueryItem("symbol", coinType);
	q.addQueryItem("depth", depth);
	source = QHttpManager::GetInstance().query("GET", FUNCTION_ORDER_BOOK, q, REQUEST_ORDER_BOOK, true);
	if (source.length() < 50 || source.indexOf("!DOCTYPE html") != -1 || source.indexOf("<!DOCTYPE HTML") != -1 || source.indexOf("error") != -1 || source.indexOf("html>") != -1)
	{
		qDebug() << "bitmex_depth" << source.mid(0, 10);
		return  0;
	}
	QEventLoop loop;
	QTimer::singleShot(1500, &loop, SLOT(quit()));
	loop.exec();
	return  1;
}

bool QBizManager::bitmex_bucketed(QString & source)
{
	QHttpManager::GetInstance().HttpGet("https://www.bitmex.com/api/v1/trade/bucketed?binSize=1m&partial=false&symbol=XBTUSD&count=100&reverse=true", source);
	if (source.length() < 50 || source.indexOf("!DOCTYPE html") != -1 || source.indexOf("<!DOCTYPE HTML") != -1 || source.indexOf("error") != -1 || source.indexOf("html>") != -1)
	{
		qDebug() << "bitmex_bucketed" << source.mid(0, 10);
		return  0;
	}
	QEventLoop loop;
	QTimer::singleShot(1500, &loop, SLOT(quit()));
	loop.exec();
	return  1;
}

void QBizManager::GetPostion(const QString & source)
{
	QJsonDocument jDoc = QJsonDocument::fromJson(source.toUtf8());
	QJsonObject jObj = jDoc.object();
	if (!jObj.isEmpty())
	{
		auto jObjmap = jObj.toVariantMap();
		auto data = jObjmap["data"].toList();
		auto dataMap = data.at(0);
		my_postion.currentQty = dataMap.toMap().value("currentQty").toString();
		if (source.indexOf("unrealisedRoePcnt") != -1)
			my_postion.unrealisedRoePcnt = dataMap.toMap().value("unrealisedRoePcnt").toString();
	}
	qDebug() << "Postion." << my_postion.currentQty << " " << my_postion.unrealisedRoePcnt;
}

bool QBizManager::parse_bucketed(const QString & source, QStringList& trade_list)
{
	int pp = source.indexOf("data");
	int p = source.indexOf("bids", pp);
	int p2 = source.indexOf("asks", p + 10);
	QString buy_price_str = source.mid(3);
	trade_list = buy_price_str.split("timestamp");
	trade_list.removeFirst();
	return 0;
}

int QBizManager::GetPrice(const QString & source, QStringList& buy_list, QStringList& sell_list)
{
	int pp = source.indexOf("data");
	int p = source.indexOf("bids", pp);
	int p2 = source.indexOf("asks", p + 10);
	QString buy_price_str = source.mid(p + 8, p2 - p - 7);
	QString sell_price_str = source.mid(p2 + 8);

	if (p2 == -1)
	{
		sell_price_str = source.mid(pp + 35);
	}
	buy_list = buy_price_str.split("],[");
	sell_list = sell_price_str.split("],[");
	return 0;
}

void QBizManager::trade()
{
	QStringList  trade_list;
	QString  soure;
	bitmex_bucketed(soure);
	parse_bucketed(soure, trade_list);

	for (int i = 0; i < trade_list.size(); i++)
	{
		QString  trade_str = trade_list.at(i);
		QString  volume = trade_str.split(",").at(7).split(":").at(1);

		if (volume.toDouble() >= 4500000)
		{
			qDebug() << "trade." << trade_str;
			trade_str = trade_list.at(i);
			my_trade.high = trade_str.split(",").at(3).split(":").at(1);
			my_trade.low = trade_str.split(",").at(4).split(":").at(1);
			return;
		}
	}
}

int QBizManager::Sell_Amount_Up()
{
	int size = price_amount_list_sell.size();
	int avg_size = size / 3;

	int front = 0;
	for (int i = 0; i < avg_size; i++)
	{
		price_amount  list_pa = price_amount_list_sell.at(i);
		front += list_pa.amount.toInt();
	}
	int avg_front = front / avg_size;

	int mid = 0;
	for (int i = avg_size; i < avg_size * 2; i++)
	{
		price_amount  list_pa = price_amount_list_sell.at(i);
		mid += list_pa.amount.toInt();
	}
	int avg_mid = mid / avg_size;

	int end = 0;
	for (int i = avg_size * 2; i < size; i++)
	{
		price_amount  list_pa = price_amount_list_sell.at(i);
		end += list_pa.amount.toInt();
	}
	int avg_end = end / avg_size;


	if (avg_end > avg_front * 2)
	{
		if (avg_end > avg_mid)
		{
			if (avg_mid > avg_front)
				return 1;
		}
	}

	if (avg_end > avg_mid)
	{
		if (avg_mid > avg_front)
			return 2;
	}

	return 0;
}


int QBizManager::Sell_Amount_Down()
{
	int size = price_amount_list_sell.size();
	int avg_size = size / 3;

	int front = 0;
	for (int i = 0; i < avg_size; i++)
	{
		price_amount  list_pa = price_amount_list_sell.at(i);
		front += list_pa.amount.toInt();
	}
	int avg_front = front / avg_size;

	int mid = 0;
	for (int i = avg_size; i < avg_size * 2; i++)
	{
		price_amount  list_pa = price_amount_list_sell.at(i);
		mid += list_pa.amount.toInt();
	}
	int avg_mid = mid / avg_size;

	int end = 0;
	for (int i = avg_size * 2; i < size; i++)
	{
		price_amount  list_pa = price_amount_list_sell.at(i);
		end += list_pa.amount.toInt();
	}
	int avg_end = end / avg_size;


	if (avg_front > avg_end * 2)
	{
		if (avg_front > avg_mid)
		{
			if (avg_mid > avg_end)
				return 1;
		}
	}


	if (avg_front > avg_mid)
	{
		if (avg_mid > avg_end)
			return 2;
	}



	return 0;
}


int QBizManager::Buy_Amount_Up()
{
	int size = price_amount_list_buy.size();
	int avg_size = size / 3;

	int front = 0;
	for (int i = 0; i < avg_size; i++)
	{
		price_amount  list_pa = price_amount_list_buy.at(i);
		front += list_pa.amount.toInt();
	}
	int avg_front = front / avg_size;

	int mid = 0;
	for (int i = avg_size; i < avg_size * 2; i++)
	{
		price_amount  list_pa = price_amount_list_buy.at(i);
		mid += list_pa.amount.toInt();
	}
	int avg_mid = mid / avg_size;

	int end = 0;
	for (int i = avg_size * 2; i < size; i++)
	{
		price_amount  list_pa = price_amount_list_buy.at(i);
		end += list_pa.amount.toInt();
	}
	int avg_end = end / avg_size;

	if (avg_end > avg_front * 2)
	{
		if (avg_end > avg_mid)
		{
			if (avg_mid > avg_front)
				return 1;
		}
	}

	if (avg_end > avg_mid)
	{
		if (avg_mid > avg_front)
			return 2;
	}

	return 0;
}



int QBizManager::Buy_Amount_Down()
{
	int size = price_amount_list_buy.size();
	int avg_size = size / 3;

	int front = 0;
	for (int i = 0; i < avg_size; i++)
	{
		price_amount  list_pa = price_amount_list_buy.at(i);
		front += list_pa.amount.toInt();
	}
	int avg_front = front / avg_size;

	int mid = 0;
	for (int i = avg_size; i < avg_size * 2; i++)
	{
		price_amount  list_pa = price_amount_list_buy.at(i);
		mid += list_pa.amount.toInt();
	}
	int avg_mid = mid / avg_size;

	int end = 0;
	for (int i = avg_size * 2; i < size; i++)
	{
		price_amount  list_pa = price_amount_list_buy.at(i);
		end += list_pa.amount.toInt();
	}
	int avg_end = end / avg_size;


	if (avg_front > avg_end * 2)
	{
		if (avg_front > avg_mid)
		{
			if (avg_mid > avg_end)
				return 1;
		}
	}

	if (avg_front > avg_mid)
	{
		if (avg_mid > avg_end)
			return 2;
	}

	return 0;
}