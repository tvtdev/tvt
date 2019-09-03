﻿#include "qbizmanager.h"
#include "qhttpmanager.h"

void QBizManager::doTransfer(const QString & source)
{
	if (trade_list.size() == 0)
		return ;

	QStringList buy_list;
	QStringList sell_list;
	if (!GetPrice(source, buy_list, sell_list))
		return;

	
	QString price_sell = sell_list.at(0).split(",").at(0);
	QString amount_sell = sell_list.at(0).split(",").at(1);;
	QString price_buy = buy_list.at(0).split(",").at(0);
	QString amount_buy = buy_list.at(0).split(",").at(1);



	if (my_postion.currentQty.toDouble() == 0)
	{
		if (amount_buy.toDouble() >= amount_sell.toDouble()  && amount_buy.length() >= 6 && amount_sell.length() <= 5 && m_price_buy.length() == 0)
		{
			//qDebug() << "Buy 1.";
			m_price_buy = price_buy;
			return;
		}
		if (price_buy >m_price_buy && price_buy.toDouble() - m_price_buy.toDouble() <= 2 && m_price_buy.length() != 0)
		{
			qDebug() << "Buy.";
			QUrlQuery param;
			QString price = QString::number(price_sell.toDouble()+1 , 'f', 1);
			param.addQueryItem("price", price);
			param.addQueryItem("symbol", "XBTUSD");
			param.addQueryItem("orderQty", "1");
			param.addQueryItem("side", "Buy");
			param.addQueryItem("ordType", "Limit");
			createOrder(param);
			m_price_buy = "";

			cancelAllAfter();

		}
		else
		{
			m_price_buy = "";
		}


	}
	
	if (my_postion.currentQty.toDouble() == 0)
	{
		if (amount_sell.toDouble() >= amount_buy.toDouble()  && amount_sell.length() >= 6 && amount_buy.length() <= 5 && m_price_sell.length() == 0)
		{
			//qDebug() << "Sell 21.";
			m_price_sell = price_sell;
			return;
		}
		if (price_sell < m_price_sell && m_price_sell.toDouble() - price_sell.toDouble() <= 12 && m_price_sell.length() != 0)
		{
			qDebug() << "Sell.";
			QUrlQuery param;
			QString price = QString::number(price_buy.toDouble() - 1, 'f', 1);
			param.addQueryItem("price", price);
			param.addQueryItem("symbol", "XBTUSD");
			param.addQueryItem("orderQty", "1");
			param.addQueryItem("side", "Sell");
			param.addQueryItem("ordType", "Limit");
			createOrder(param);
			m_price_buy = "";
			m_price_sell = "";

			cancelAllAfter();
		}
		else
		{
			m_price_sell = "";
		}
	}
//	return;
	if (my_postion.currentQty.toDouble() > 0)
	{
		if (oneord == 1)
			return;

		qDebug() << "Sell.";
		QUrlQuery param;
		QString price = QString::number(price_buy.toDouble() - 1, 'f', 1);
		param.addQueryItem("price", price);
		param.addQueryItem("symbol", "XBTUSD");
		param.addQueryItem("orderQty", "1");
		param.addQueryItem("side", "Sell");
		param.addQueryItem("ordType", "Limit");
		createOrder(param);
		m_price_buy = "";
		m_price_sell = "";

		cancelAllAfter();

		oneord = 1;
		m_TradeTimer_order.start();
	}

	if (my_postion.currentQty.toDouble() < 0)
	{
		if (oneord == 1)
			return;

		qDebug() << "Buy.";
		QUrlQuery param;
		QString price = QString::number(price_sell.toDouble() + 1, 'f', 1);
		param.addQueryItem("price", price);
		param.addQueryItem("symbol", "XBTUSD");
		param.addQueryItem("orderQty", "1");
		param.addQueryItem("side", "Buy");
		param.addQueryItem("ordType", "Limit");
		createOrder(param);
		m_price_buy = "";

		cancelAllAfter();

		oneord = 1;
		m_TradeTimer_order.start();
	}

	/*	}
		else
		{
			m_price_buy = "";
		}
*/


	return;
	if (my_postion.currentQty.toDouble() > 0)
	{
		if (amount_sell.toDouble() <= amount_buy.toDouble()&& m_price_sell.length() == 0)
		{
		
			m_price_sell = price_sell;
			return;
		}
		if ( m_price_sell.length() != 0)
		{
			qDebug() << "Sell.";
			QUrlQuery param;
			QString price = QString::number(price_buy.toDouble() + 0.5, 'f', 1);
			param.addQueryItem("price", price);
			param.addQueryItem("symbol", "XBTUSD");
			param.addQueryItem("orderQty", "1");
			param.addQueryItem("side", "Sell");
			param.addQueryItem("ordType", "Limit");
			createOrder(param);
			m_price_buy = "";
			m_price_sell = "";

			cancelAllAfter();
		}
		else
		{
			m_price_sell = "";
		}
	}
	return;
	
	if (my_postion.currentQty.toDouble() >= 0)
	{
		int ret_up = Up_Fan(price_sell);
		if (ret_up == 2)
		{
			if (oneord == 1)
					return;		
			qDebug() << "sell  1";
			QUrlQuery param;
			param.addQueryItem("symbol", "XBTUSD");
			param.addQueryItem("orderQty", "1");
			param.addQueryItem("side", "Sell");
			param.addQueryItem("ordType", "Market");
			param.addQueryItem("text", text);
			createOrder(param);

			{
				QUrlQuery param;
				QString price = QString::number(price_buy.toDouble() - 15, 'f', 1);
				param.addQueryItem("price", price);
				param.addQueryItem("symbol", "XBTUSD");
				param.addQueryItem("orderQty", "1");
				param.addQueryItem("side", "Buy");
				param.addQueryItem("ordType", "Limit");
				param.addQueryItem("text", text);
				createOrder(param);
			}

			m_price_buy = "";
			text = "";
			oneord = 1;
			m_TradeTimer_order.start();
			return;			
		}
	}
	if (my_postion.currentQty.toDouble() <= 0)
	{
		int ret_down = Down_Fan(price_sell);
		if (ret_down == 2)
		{
			if (oneord == 1)
				return;

			qDebug() << "buy  1";
			QUrlQuery param;
			param.addQueryItem("symbol", "XBTUSD");
			param.addQueryItem("orderQty", "1");
			param.addQueryItem("side", "Buy");
			param.addQueryItem("ordType", "Market");
			param.addQueryItem("text", text);
			createOrder(param);
			{
				QUrlQuery param;
				QString price = QString::number(price_buy.toDouble() + 15, 'f', 1);
				param.addQueryItem("price", price);
				param.addQueryItem("symbol", "XBTUSD");
				param.addQueryItem("orderQty", "1");
				param.addQueryItem("side", "Sell");
				param.addQueryItem("ordType", "Limit");
				param.addQueryItem("text", text);
				createOrder(param);
			}
			m_price_buy = "";
			text = "";
			oneord = 1;
			m_TradeTimer_order.start();
			return;
		}

	}
	

	return;
	{
		if (my_postion.currentQty.toDouble() > 0 )
		{
			if (price_buy.toDouble() > m_trade.low.toDouble()-1)
			{
				//qDebug() << "doTransfer. bao zhang 1.";
				return;
			}

			//qDebug() << "doTransfer. 1.  " << m_trade.volume << my_postion.currentQty << my_postion.unrealisedRoePcnt << price_buy << m_trade.high;
			QUrlQuery param;
			param.addQueryItem("symbol", "XBTUSD");
			closePosition(param);

			my_postion.currentQty = "0";
			my_postion.unrealisedRoePcnt = "0";
			m_trade.volume = "0";
		}
		else if (my_postion.currentQty.toDouble() < 0 )
		{	
			/*if (m_trade.volume.toDouble() <= 2100000)
				return;*/

			if (price_sell.toDouble() < m_trade.high.toDouble())
			{
				//qDebug() << "doTransfer. bao die 1.";
				//dfaf = QDateTime::currentDateTime();
				return;
			}

		/*	if (price_sell.toDouble() < m_trade.high.toDouble())
			{
				qDebug() << "doTransfer. bao die 1.";
				dfaf = QDateTime::currentDateTime();
				return;
			}*/

			//qDebug() << "doTransfer 2  " << m_trade.volume << my_postion.currentQty << my_postion.unrealisedRoePcnt<< price_sell<< m_trade.low;
			QUrlQuery param;
			param.addQueryItem("symbol", "XBTUSD");
			closePosition(param);

			my_postion.currentQty = "0";
			my_postion.unrealisedRoePcnt = "0";
			m_trade.volume = "0";			
		}

	
	}	
}

QBizManager::QBizManager()
{
	 m_signed = false;
	 m_currentTotalRequests = 0;
	 m_maxRequests = 60;

	 m_websocketHost = "wss://www.bitmex.com/realtime";
    connect(&m_webSocket, &QWebSocket::connected, this, &QBizManager::connected,Qt::UniqueConnection);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &QBizManager::closed,Qt::UniqueConnection);
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &QBizManager::textMessageReceived,Qt::UniqueConnection);
    connect(&m_webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),[=](QAbstractSocket::SocketError error){
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

	m_TradeTimer.setInterval(10000 * 3);
	connect(&m_TradeTimer, &QTimer::timeout, this, &QBizManager::trade);
	m_TradeTimer.start();

	
	m_TradeTimer_order.setInterval(10000 * 6);
	connect(&m_TradeTimer_order, &QTimer::timeout, this, &QBizManager::trade_ordre);


	 m_price_buy = "";
	 m_price_sell = "";
	 m_price_amount_buy = 0;
	 m_price_amount_sell = 0;
	 QString amouant = "3";// QString::number(my_postion.currentQty.toDouble()*0.5);
	 //amount = QString::number(amount.toInt());
	 QString amount = QString::number(amouant.toDouble()*0.5,'g',0);

	 oneord = 0;
	 oneordfdsf = 0;
	 
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
					m_webSocket.sendTextMessage(QString(R"({"op": "subscribe", "args": ["tradeBin1m:XBTUSD"]})"));					
				}
			}
		}
		else if (jObj.contains("table")) {				
			if (message.indexOf("position") != -1)
			{
				GetPostion(message);
				doTransfer("");
				
			}else 
			if (message.indexOf("tradeBin1m") != -1)
			{
				GetVolume(message);
				doTransfer("");
			}	else
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
    q.addQueryItem("currency",coinType);
	QHttpManager::GetInstance().query("GET",FUNCTION_WALLET,q,REQUEST_WALLET,true);
}

void QBizManager::createOrder(QUrlQuery param)
{
	QHttpManager::GetInstance().query("POST",FUNCTION_CREATE_ORDER,param,REQUEST_CREATE_ORDER,true);
}

void QBizManager::closePosition(QUrlQuery param)
{
	QHttpManager::GetInstance().query("POST", FUNCTION_closePosition_ORDER, param, REQUEST_TRANSFERMARGIN_POSITION, true);
}

void QBizManager::queryAllOrder(QUrlQuery param)
{
	QHttpManager::GetInstance().query("GET",FUNCTION_QUERY_ORDER,param,REQUEST_QUERY_ORDER,true);
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

void QBizManager::cancelOrder(QString orderId, QString clOrderId, QString comment)
{
    QUrlQuery q;
    q.addQueryItem("orderID",orderId);
    q.addQueryItem("clOrdID",clOrderId);
    q.addQueryItem("text",comment);
	QHttpManager::GetInstance().query("DELETE",FUNCTION_CANCEL_ORDER,q,REQUEST_CANCEL_ORDER,true);
}


void QBizManager::cancelAllAfter()
{
	QUrlQuery q;
	q.addQueryItem("timeout", "60");
	QHttpManager::GetInstance().query("POST", FUNCTION_cancelAllAfter_ORDER, q, REQUEST_CANCEL_ORDER, true);
}

bool QBizManager::bitmex_depth(QString & source, QString coinType, QString depth)
{
	QUrlQuery q;
	q.addQueryItem("symbol", coinType);
	q.addQueryItem("depth", depth);
	source = QHttpManager::GetInstance().query("GET", FUNCTION_ORDER_BOOK, q, REQUEST_ORDER_BOOK, true);
	if (source.length() < 50 || source.indexOf("!DOCTYPE html") != -1 || source.indexOf("<!DOCTYPE HTML") != -1 || source.indexOf("error") != -1 || source.indexOf("html>") != -1)
	{
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
		return  0;
	}
	//QEventLoop loop;
	//QTimer::singleShot(1500, &loop, SLOT(quit()));
	//loop.exec();
	return  1;
}



bool QBizManager::bitmex_bucketed_5(QString & source)
{
	QHttpManager::GetInstance().HttpGet("https://www.bitmex.com/api/v1/trade/bucketed?binSize=5m&partial=false&symbol=XBTUSD&count=100&reverse=true", source);
	if (source.length() < 50 || source.indexOf("!DOCTYPE html") != -1 || source.indexOf("<!DOCTYPE HTML") != -1 || source.indexOf("error") != -1 || source.indexOf("html>") != -1)
	{
		return  0;
	}
	//QEventLoop loop;
	//QTimer::singleShot(1500, &loop, SLOT(quit()));
	//loop.exec();
	return  1;
}

void QBizManager::GetPostion(const QString & source )
{
	{
		QJsonDocument jDoc = QJsonDocument::fromJson(source.toUtf8());
		QJsonObject jObj = jDoc.object();
		if (!jObj.isEmpty())
		{
			auto jObjmap = jObj.toVariantMap();
			auto data = jObjmap["data"].toList();
			auto dataMap = data.at(0);
			my_postion.currentQty = dataMap.toMap().value("currentQty").toString();
			my_postion.unrealisedRoePcnt = dataMap.toMap().value("unrealisedRoePcnt").toString();
		}
	}
}



void QBizManager::GetVolume(const QString & source)
{
	QJsonDocument jDoc = QJsonDocument::fromJson(source.toUtf8());
	QJsonObject jObj = jDoc.object();
	if (!jObj.isEmpty()) 
	{
		auto jObjmap = jObj.toVariantMap();
		auto data = jObjmap["data"].toList();
		auto dataMap = data.at(0);
		m_trade.volume = dataMap.toMap().value("volume").toString();
		m_trade.close = dataMap.toMap().value("close").toString();
		m_trade.high = dataMap.toMap().value("high").toString();
		m_trade.low = dataMap.toMap().value("low").toString();
	}
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
	if (p == -1)
		return 0;
	int p2 = source.indexOf("asks", p + 10);
	QString buy_price_str = source.mid(p + 8, p2 - p - 7);
	QString sell_price_str = source.mid(p2 + 8);
	if (p2 == -1)
	{
		sell_price_str = source.mid(pp + 35);
	}
	buy_list = buy_price_str.split("],[");
	sell_list = sell_price_str.split("],[");
	return 1;
}


void QBizManager::trade()
{
	QString  soure;
	bitmex_bucketed(soure);
	parse_bucketed(soure, trade_list);

	soure = "";
	bitmex_bucketed_5(soure);
	parse_bucketed(soure, m_trade_list_5);
}


void QBizManager::trade_ordre()
{
	oneord = 3;
	m_TradeTimer_order.stop();
}

bool QBizManager::Up_Low_Check()
{
	QString low5 = trade_list.at(4).split(",").at(4).split(":").at(1);
	QString low4 = trade_list.at(3).split(",").at(4).split(":").at(1);
	QString low3 = trade_list.at(2).split(",").at(4).split(":").at(1);
	QString low2 = trade_list.at(1).split(",").at(4).split(":").at(1);
	QString low1 = trade_list.at(0).split(",").at(4).split(":").at(1);

	if (low3.toDouble() + 2 < low2.toDouble())
		if (low2.toDouble() + 2 < low1.toDouble())
			return 1;

	if (low2.toDouble() + 2 < low1.toDouble())	
		if (low2.toDouble() == low3.toDouble()) //第2和3底部是平的
		{
			if (low4.toDouble() + 2 < low3.toDouble())
				return 1;
		}


	if (low2.toDouble() + 2 < low1.toDouble())
		if (low2.toDouble() == low3.toDouble()) //第2和3底部是平的
		{
			if (low4.toDouble() == low3.toDouble()) //第2和3底部是平的
				if (low5.toDouble() + 2 < low4.toDouble())
				return 1;
		}

	return 0;
}


bool QBizManager::Up_Check()
{
	QString low5 = trade_list.at(4).split(",").at(4).split(":").at(1);
	QString low4 = trade_list.at(3).split(",").at(4).split(":").at(1);
	QString low3 = trade_list.at(2).split(",").at(4).split(":").at(1);
	QString low2 = trade_list.at(1).split(",").at(4).split(":").at(1);
	QString low1 = trade_list.at(0).split(",").at(4).split(":").at(1);

	int num = 0;

	if (low4.toDouble() <= low3.toDouble())
		num++;

	if (low5.toDouble() <= low3.toDouble())
		num++;

	if (low5.toDouble() <= low4.toDouble())
		num++;

	if (num >= 1)
		return 1;
	
	return 0;
}



bool QBizManager::Up_Check_5()
{
	QString low10 = m_trade_list_5.at(9).split(",").at(4).split(":").at(1);
	QString low9 = m_trade_list_5.at(8).split(",").at(4).split(":").at(1);
	QString low8 = m_trade_list_5.at(7).split(",").at(4).split(":").at(1);
	QString low7 = m_trade_list_5.at(6).split(",").at(4).split(":").at(1);
	QString low6 = m_trade_list_5.at(5).split(",").at(4).split(":").at(1);
	QString low5 = m_trade_list_5.at(4).split(",").at(4).split(":").at(1);
	QString low4 = m_trade_list_5.at(3).split(",").at(4).split(":").at(1);
	QString low3 = m_trade_list_5.at(2).split(",").at(4).split(":").at(1);
	QString low2 = m_trade_list_5.at(1).split(",").at(4).split(":").at(1);
	QString low1 = m_trade_list_5.at(0).split(",").at(4).split(":").at(1);

	int num = 0;
	if (low4.toDouble() <= low3.toDouble())
		num++;

	if (low5.toDouble() <= low3.toDouble())
		num++;

	if (low5.toDouble() <= low4.toDouble())
		num++;

	if (low6.toDouble() <= low5.toDouble())
		num++;

	if (low7.toDouble() <= low6.toDouble())
		num++;

	if (low8.toDouble() <= low7.toDouble())
		num++;

	if (low9.toDouble() <= low8.toDouble())
		num++;

	if (num >= 2)
		return 1;

	return 0;
}


//严格标准在上方
bool QBizManager::Up_Check_Red()
{
	QString high5 = trade_list.at(4).split(",").at(3).split(":").at(1);
	QString high4 = trade_list.at(3).split(",").at(3).split(":").at(1);
	QString high3 = trade_list.at(2).split(",").at(3).split(":").at(1);
	QString high2 = trade_list.at(1).split(",").at(3).split(":").at(1);
	QString high1 = trade_list.at(0).split(",").at(3).split(":").at(1);

	int num = 0;
	if (high4.toDouble() >= high3.toDouble())
		num++;

	if (high5.toDouble() >= high3.toDouble())
		num++;

	if (high5.toDouble() <= high4.toDouble())
		num++;

	if (num >= 1)
		return 1;

	return 0;
}


//严格标准在上方
bool QBizManager::Up_Check_Green_5()
{
	int num = 0;
	{
		QString open = m_trade_list_5.at(0).split(",").at(2).split(":").at(1);
		QString close = m_trade_list_5.at(0).split(",").at(5).split(":").at(1);
		if (open.toDouble() < close.toDouble())
			num++;
	}
	{
		QString open = m_trade_list_5.at(1).split(",").at(2).split(":").at(1);
		QString close = m_trade_list_5.at(1).split(",").at(5).split(":").at(1);
		if (open.toDouble() < close.toDouble())
			num++;
	}

	//{
	//	QString open = m_trade_list_5.at(2).split(",").at(2).split(":").at(1);
	//	QString close = m_trade_list_5.at(2).split(",").at(5).split(":").at(1);
	//	if (open.toDouble() < close.toDouble())
	//		num++;
	//}

	//{
	//	QString open = m_trade_list_5.at(3).split(",").at(2).split(":").at(1);
	//	QString close = m_trade_list_5.at(3).split(",").at(5).split(":").at(1);
	//	if (open.toDouble() > close.toDouble())
	//		num++;
	//}

	//{
	//	QString open = m_trade_list_5.at(4).split(",").at(2).split(":").at(1);
	//	QString close = m_trade_list_5.at(4).split(",").at(5).split(":").at(1);
	//	if (open.toDouble() > close.toDouble())
	//		num++;
	//}

	if (num >= 2)
		return 1;

	return 0;
}

//绿色
bool QBizManager::Up_Check_Red_Front()
{
	QString open = trade_list.at(0).split(",").at(2).split(":").at(1);
	QString high1 = trade_list.at(0).split(",").at(3).split(":").at(1);
	QString close = trade_list.at(0).split(",").at(5).split(":").at(1);
	if (open.toDouble() < close.toDouble())
		return 1;
	return 0;
}


int QBizManager::Up_Fan(QString p)
{
	if (trade_list.size() == 0)
		return 0;
	if (m_trade_list_5.size() == 0)
		return 0;


	QString low5 = trade_list.at(4).split(",").at(4).split(":").at(1);
	QString low4 = trade_list.at(3).split(",").at(4).split(":").at(1);
	QString low3 = trade_list.at(2).split(",").at(4).split(":").at(1);
	QString low2 = trade_list.at(1).split(",").at(4).split(":").at(1);
	QString low1 = trade_list.at(0).split(",").at(4).split(":").at(1);

	if (low1.toDouble() >= low2.toDouble())
		if (low2.toDouble() >= low3.toDouble())
		{
			QString low = low1;
			if (p.toDouble() < low.toDouble())
			{
				text = "4第三个底部低于等于第二个底部，第二个上升很快 ，第1个回调。";
				return 2;
			}
		}

	//if (low1.toDouble() <= low2.toDouble())
	//	if (low2.toDouble() <= low3.toDouble())
	//	{
	//		QString low = low1;
	//		if (p.toDouble() < low.toDouble())
	//		{
	//			text = "4第三个底部低于等于第二个底部，第二个上升很快 ，第1个回调。";
	//			return 2;
	//		}
	//	}

	return 0;

	//if (!Down_Check_5())


	//标准梯度上升 ，第1个回调。无空缺
	//if (Up_Low_Check())
	//{
	//	if (Up_Check()) //严格升趋势
	//	{
	//		QString low = low2;
	//		if (p.toDouble() < low.toDouble())
	//		{
	//			qDebug() << "Up_Fan  3";
	//			text = "1标准梯度上升 ，第1个回调";
	//			return 2;
	//		}
	//	}
	//}

	////标准梯度上升 ，第1个回调。中间有空缺 上升很快
	//if (Up_Low_Check())
	//{
	//	if (Up_Check()) //严格升趋势
	//	{
	//		if (Up_Check_Red_Front()) //严格升趋势
	//		{
	//			QString low = low2;
	//			if (p.toDouble() < low.toDouble())
	//			{
	//				qDebug() << "Up_Fan  3";
	//				text = "2标准梯度上升 ，第1个回调。中间有空缺";
	//				return 2;
	//			}
	//		}
	//	}
	//}

	//第三个底部低于等于第二个底部，第二个上升很快 ，第1个回调。
	//if (low3.toDouble() <= low2.toDouble())
	//	if (low1.toDouble() > low2.toDouble())
	//	{
	//		QString high2 = trade_list.at(1).split(",").at(3).split(":").at(1);
	//		if (high2.toDouble() - low2.toDouble() >= 12) //第二个上升很快
	//		{
	//			if (low1.toDouble() - low5.toDouble() >= 50) //第二个上升很快
	//		
	//			if (Up_Check()) //严格升趋势
	//			{
	//				QString low = low1;
	//				if (p.toDouble() < low.toDouble() )
	//				{
	//					text = "3第三个底部低于等于第二个底部，第二个上升很快 ，第1个回调。";
	//					return 2;
	//				}
	//			}
	//		}
	//	}

	

	
	//if (Down_Check_Red_5())
	{
		if (low1.toDouble() >= low2.toDouble())
			if (low2.toDouble() >= low3.toDouble())
			{
				QString low = low1;
				if (p.toDouble() < low.toDouble())
				{
					text = "4第三个底部低于等于第二个底部，第二个上升很快 ，第1个回调。";
					return 2;
				}
			}
	}
	return 0;
	if (low1.toDouble() >= low2.toDouble())
		if (low1.toDouble() >= low3.toDouble())
		{
			if (low1.toDouble() >= low3.toDouble())
				my_trade.low = low2;
		}

	if (p.toDouble() < my_trade.low.toDouble()  && !my_trade.low.isEmpty())
	{
		//my_now = QDateTime::currentDateTime();
		return 2;
	}
	return 0;
}



bool QBizManager::Down_High_Check()
{
	QString high5 = trade_list.at(4).split(",").at(3).split(":").at(1);
	QString high4 = trade_list.at(3).split(",").at(3).split(":").at(1);
	QString high3 = trade_list.at(2).split(",").at(3).split(":").at(1);
	QString high2 = trade_list.at(1).split(",").at(3).split(":").at(1);
	QString high1 = trade_list.at(0).split(",").at(3).split(":").at(1);

	if (high3.toDouble() > high2.toDouble() +1)
		if (high2.toDouble()  > high1.toDouble() +1)
			return 1;

	if (high2.toDouble()  > high1.toDouble() + 1)
		if (high2.toDouble() == high3.toDouble()) //第2和3底部是平的
		{
			if (high4.toDouble()  > high3.toDouble() +2)
				return 1;
		}


	if (high2.toDouble() + 2  >  high1.toDouble())
		if (high2.toDouble() == high3.toDouble()) //第2和3底部是平的
		{
			if (high4.toDouble() == high3.toDouble()) //第2和3底部是平的
				if (high5.toDouble() + 2  >  high4.toDouble())
					return 1;
		}

	return 0;
}


//严格标准在上方
bool QBizManager::Down_Check()
{
	QString high6 = trade_list.at(5).split(",").at(3).split(":").at(1);
	QString high5 = trade_list.at(4).split(",").at(3).split(":").at(1);
	QString high4 = trade_list.at(3).split(",").at(3).split(":").at(1);
	QString high3 = trade_list.at(2).split(",").at(3).split(":").at(1);
	QString high2 = trade_list.at(1).split(",").at(3).split(":").at(1);
	QString high1 = trade_list.at(0).split(",").at(3).split(":").at(1);

	int num = 0;
	if (high4.toDouble() > high3.toDouble())
		num++;

	if (high5.toDouble() > high3.toDouble())
		num++;

	if (high5.toDouble() > high4.toDouble())
		num++;

	if (high6.toDouble() > high5.toDouble())
		num++;

	if (num >= 1)
		return 1;

	return 0;
}



//严格标准在上方
bool QBizManager::Down_Check_5()
{
	QString high11= m_trade_list_5.at(10).split(",").at(3).split(":").at(1);
	QString high10 = m_trade_list_5.at(9).split(",").at(3).split(":").at(1);
	QString high9 = m_trade_list_5.at(8).split(",").at(3).split(":").at(1);
	QString high8 = m_trade_list_5.at(7).split(",").at(3).split(":").at(1);
	QString high7 = m_trade_list_5.at(6).split(",").at(3).split(":").at(1);
	QString high6 = m_trade_list_5.at(5).split(",").at(3).split(":").at(1);
	QString high5 = m_trade_list_5.at(4).split(",").at(3).split(":").at(1);
	QString high4 = m_trade_list_5.at(3).split(",").at(3).split(":").at(1);
	QString high3 = m_trade_list_5.at(2).split(",").at(3).split(":").at(1);
	QString high2 = m_trade_list_5.at(1).split(",").at(3).split(":").at(1);
	QString high1 = m_trade_list_5.at(0).split(",").at(3).split(":").at(1);

	int num = 0;
	if (high4.toDouble() > high3.toDouble())
		num++;

	if (high5.toDouble() > high3.toDouble())
		num++;

	if (high5.toDouble() > high4.toDouble())
		num++;

	if (high6.toDouble() > high5.toDouble())
		num++;

	if (high7.toDouble() > high6.toDouble())
		num++;

	if (high8.toDouble() > high7.toDouble())
		num++;

	if (num >= 2)
		return 1;

	return 0;
}


//严格标准在上方
bool QBizManager::Down_Check_Red()
{
	int num = 0;
	{
		QString open = trade_list.at(1).split(",").at(2).split(":").at(1);
		QString close = trade_list.at(1).split(",").at(5).split(":").at(1);
		if (open.toDouble() > close.toDouble())
			num++;
	}

	{
		QString open = trade_list.at(2).split(",").at(2).split(":").at(1);
		QString close = trade_list.at(2).split(",").at(5).split(":").at(1);
		if (open.toDouble() > close.toDouble())
			num++;
	}

	{
		QString open = trade_list.at(3).split(",").at(2).split(":").at(1);
		QString close = trade_list.at(3).split(",").at(5).split(":").at(1);
		if (open.toDouble() > close.toDouble())
			num++;
	}

	{
		QString open = trade_list.at(4).split(",").at(2).split(":").at(1);
		QString close = trade_list.at(4).split(",").at(5).split(":").at(1);
		if (open.toDouble() > close.toDouble())
			num++;
	}

	if (num >= 2)
		return 1;

	return 0;
}


//严格标准在上方
bool QBizManager::Down_Check_Red_5()
{
	int num = 0;
	{
		QString open = m_trade_list_5.at(0).split(",").at(2).split(":").at(1);
		QString close = m_trade_list_5.at(0).split(",").at(5).split(":").at(1);
		if (open.toDouble() > close.toDouble())
			num++;
	}

	{
		QString open = m_trade_list_5.at(1).split(",").at(2).split(":").at(1);
		QString close = m_trade_list_5.at(1).split(",").at(5).split(":").at(1);
		if (open.toDouble() > close.toDouble())
			num++;
	}

	//{
	//	QString open = m_trade_list_5.at(2).split(",").at(2).split(":").at(1);
	//	QString close = m_trade_list_5.at(2).split(",").at(5).split(":").at(1);
	//	if (open.toDouble() > close.toDouble())
	//		num++;
	//}

	//{
	//	QString open = m_trade_list_5.at(3).split(",").at(2).split(":").at(1);
	//	QString close = m_trade_list_5.at(3).split(",").at(5).split(":").at(1);
	//	if (open.toDouble() > close.toDouble())
	//		num++;
	//}

	//{
	//	QString open = m_trade_list_5.at(4).split(",").at(2).split(":").at(1);
	//	QString close = m_trade_list_5.at(4).split(",").at(5).split(":").at(1);
	//	if (open.toDouble() > close.toDouble())
	//		num++;
	//}

	if (num >= 2)
		return 1;

	return 0;
}
//绿色
bool QBizManager::Down_Check_Green_Front()
{
	QString open = trade_list.at(0).split(",").at(2).split(":").at(1);
	QString high1 = trade_list.at(0).split(",").at(3).split(":").at(1);
	QString close = trade_list.at(0).split(",").at(5).split(":").at(1);
	if (open.toDouble() < close.toDouble())
		return 1;
	return 0;
}

int QBizManager::Down_Fan(QString p)
{
	if (trade_list.size() == 0)
		return 0;
	if (m_trade_list_5.size() == 0)
		return 0;

	
	QString high4 = trade_list.at(3).split(",").at(3).split(":").at(1);
	QString high3 = trade_list.at(2).split(",").at(3).split(":").at(1);
	QString high2 = trade_list.at(1).split(",").at(3).split(":").at(1);
	QString high1 = trade_list.at(0).split(",").at(3).split(":").at(1);
	
	if (high3.toDouble() >= high2.toDouble())
		if (high2.toDouble() >= high1.toDouble())
		{
			QString high = high1;
			if (p.toDouble() > high1.toDouble())
			{
				text = "23标准梯度下降 ，第1个回调。第4，3 ，2红色，第1个绿色";
				return 2;
			}
		}
	return 0;
	//if (!Up_Check_5())//分钟趋势
	//	return 0;
	//顶部平 突破 4个是一排
	if (Down_High_Same())
	{
		if (Down_Check())
		{
			QString high = high1;
			if (p.toDouble() > high1.toDouble()+2)
			{
				text = "1四个顶部平 突破 ";
				return 2;
			}		
			
		}
	}

	//标准梯度下降 ，第1个回调。第4，3 ，2红色，第1个绿色
	if (Down_High_Check())
	{
			if (Down_Check()) //严格升趋势
			{
				if (Down_Check_Green_Front()) //绿色
				{
					qDebug() << "Down_Check  2";
					if (Down_Check_Red()) //绿色
					{
						qDebug() << "Down_Check  3";
						QString high = high1;
						if (p.toDouble() > high1.toDouble())
						{ 
							text = "2标准梯度下降 ，第1个回调。第4，3 ，2红色，第1个绿色";
							return 2;
						}
					}
				}
			}
		}

	if (Up_Check_Green_5())
	{
		if (high3.toDouble() >= high2.toDouble())
			if (high2.toDouble() >= high1.toDouble())
			{
				QString high = high1;
				if (p.toDouble() > high1.toDouble())
				{
					text = "3标准梯度下降 ，第1个回调。第4，3 ，2红色，第1个绿色";
					return 2;
				}
			}
	}

	return 0;

	{
	
	}

	if (high3.toDouble() >= high2.toDouble())
		if (high2.toDouble() >= high1.toDouble())
		{
			//my_trade.high = high2.toDouble():high1.toDouble()? high2.toDouble() > high1.toDouble()  ;

			if (p.toDouble() > my_trade.high.toDouble() && !my_trade.high.isEmpty())
			{
				return 2;
			}
		}

	if (high3.toDouble() >= high2.toDouble())
		if (high3.toDouble() >= high1.toDouble())
		{
			my_trade.high = high2;

			if (p.toDouble() > my_trade.high.toDouble() && !my_trade.high.isEmpty())
			{
				return 2;
			}
		}

	//第3个缺口
	if (high2.toDouble() >= high1.toDouble())
	{
		if (high3.toDouble() < high2.toDouble())
		{
			if (high4.toDouble() >= high3.toDouble())
				if (high4.toDouble() >= high2.toDouble())
					my_trade.high = high2;

			if (p.toDouble() > my_trade.high.toDouble() && !my_trade.high.isEmpty())
			{
				//my_now = QDateTime::currentDateTime();
				return 2;
			}
		}
	}
		
	//第2个缺口
	if (high3.toDouble() >= high1.toDouble())
	{
		if (high3.toDouble() > high2.toDouble())
		{
			if (high4.toDouble() >= high3.toDouble())
				if (high4.toDouble() >= high2.toDouble())
					my_trade.high = high2;

			if (p.toDouble() > my_trade.high.toDouble() && !my_trade.high.isEmpty())
			{
			//	my_now = QDateTime::currentDateTime();
				return 2;
			}
		}
	}

	
	return 0;
}




bool QBizManager::Down_High_Same()
{
	QString high4 = trade_list.at(3).split(",").at(3).split(":").at(1);
	QString high3 = trade_list.at(2).split(",").at(3).split(":").at(1);
	QString high2 = trade_list.at(1).split(",").at(3).split(":").at(1);
	QString high1 = trade_list.at(0).split(",").at(3).split(":").at(1);


	if (high1.toDouble() == high2.toDouble())
		if (high2.toDouble() == high3.toDouble())
			if (high3.toDouble() == high4.toDouble())
				return 1;

	//第三个凹下去
	if (high1.toDouble() == high2.toDouble())
		if (high4.toDouble() == high2.toDouble())
			if (high3.toDouble() < high4.toDouble())
				return 1;

	//第二个凹下去
	if (high1.toDouble() == high3.toDouble())
		if (high4.toDouble() == high3.toDouble())
			if (high2.toDouble() < high4.toDouble())
				return 1;

	return 0;
}