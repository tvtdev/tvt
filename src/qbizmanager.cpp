#include "qbizmanager.h"
#include "qhttpmanager.h"


//cnh
void QBizManager::doTransfer(const QString & source)
{
	//if (trade_list.size() == 0)
		return ;

	QStringList buy_list;
	QStringList sell_list;
	if (!GetPrice(source, buy_list, sell_list))
		return;

	QString price_sell = sell_list.at(0).split(",").at(0);
	QString amount_sell = sell_list.at(0).split(",").at(1);;
	QString price_buy = buy_list.at(0).split(",").at(0);
	QString amount_buy = buy_list.at(0).split(",").at(1);

	m_price = price_sell;
	int ret_up = Up(price_sell);



	if (ret_up == 5)
	{
		if (oneord == 1)
			return;

		oneord = 1;

		for (int i = 0; i <= 5; i++)

		{
			QEventLoop loop;
			QTimer::singleShot(12000, &loop, SLOT(quit()));
			loop.exec();

			qDebug() << "Alert  2";
			Alert();

		}

		m_TradeTimer_order.start();
		return;
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
        qDebug() << "web error:" << error;;
    });
   // m_webSocket.open(QUrl(m_websocketHost));
   	m_pingTimer.setInterval(1000 * 5);
	connect(&m_pingTimer, &QTimer::timeout, this, [&]() {
		if (m_webSocket.state() == QAbstractSocket::ConnectedState) {
			m_webSocket.ping("ping");
		}
		else {
			qDebug() << "Websocket is closed?please check.";
		}
	});

	qDebug() << "init.";

	m_TradeTimer.setInterval(10000 * 9);
	connect(&m_TradeTimer, &QTimer::timeout, this, &QBizManager::trade);
	m_TradeTimer.start();

	
	m_TradeTimer_order.setInterval(10000 * 6*2);
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
	 numm = 0;
	 
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
					m_webSocket.sendTextMessage(QString(R"({"op": "subscribe", "args": ["trade:XBTUSD"]})"));
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
			else if (message.indexOf("trade") != -1)
				doTrade(message);
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

//#include "../smtp/SmtpMime"
bool QBizManager::Alert()
{
	
	QString source;
	QHttpManager::GetInstance().HttpGet("https://i3gogonun1.execute-api.us-east-1.amazonaws.com/usd/dxy", source);
	if (source.length() < 50 || source.indexOf("!DOCTYPE html") != -1 || source.indexOf("<!DOCTYPE HTML") != -1 || source.indexOf("error") != -1 || source.indexOf("html>") != -1)
	{
		return  0;
	}
	return 0 ;
}



void QBizManager::cancelOrder(QString orderId, QString clOrderId, QString comment)
{
    QUrlQuery q;
    q.addQueryItem("orderID",orderId);
    q.addQueryItem("clOrdID",clOrderId);
    q.addQueryItem("text",comment);
	QHttpManager::GetInstance().query("DELETE",FUNCTION_CANCEL_ORDER,q,REQUEST_CANCEL_ORDER,true);
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
	return  1;
}



bool QBizManager::bitmex_bucketed_5(QString & source)
{
	QHttpManager::GetInstance().HttpGet("https://www.bitmex.com/api/v1/trade/bucketed?binSize=5m&partial=false&symbol=XBTUSD&count=20&reverse=true", source);
	if (source.length() < 50 || source.indexOf("!DOCTYPE html") != -1 || source.indexOf("<!DOCTYPE HTML") != -1 || source.indexOf("error") != -1 || source.indexOf("html>") != -1)
	{
		return  0;
	}
	return  1;
}


bool QBizManager::bitmex_bucketed_1h(QString & source)
{
	QHttpManager::GetInstance().HttpGet("https://www.bitmex.com/api/v1/trade/bucketed?binSize=1h&partial=false&symbol=XBTUSD&count=5&reverse=true", source);
	if (source.length() < 50 || source.indexOf("!DOCTYPE html") != -1 || source.indexOf("<!DOCTYPE HTML") != -1 || source.indexOf("error") != -1 || source.indexOf("html>") != -1)
	{
		return  0;
	}
	return  1;
}


bool QBizManager::bitmex_bucketed_1day(QString & source)
{
	QHttpManager::GetInstance().HttpGet("https://www.bitmex.com/api/v1/trade/bucketed?binSize=1d&partial=false&symbol=XBTUSD&count=5&reverse=true", source);
	if (source.length() < 50 || source.indexOf("!DOCTYPE html") != -1 || source.indexOf("<!DOCTYPE HTML") != -1 || source.indexOf("error") != -1 || source.indexOf("html>") != -1)
	{
		return  0;
	}
	return  1;
}



bool QBizManager::bitmex_USD_Index(QString & source)
{
	QHttpManager::GetInstance().HttpGet("https://query1.finance.yahoo.com/v8/finance/chart/DX-Y.NYB?symbol=DX-Y.NYB&interval=1m", source);
	if (source.length() < 50 || source.indexOf("!DOCTYPE html") != -1 || source.indexOf("<!DOCTYPE HTML") != -1 || source.indexOf("eror") != -1 || source.indexOf("html>") != -1)
	{
		return  0;
	}
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
			if (data.size() == 0)
				return;
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
		m_tradeBin1m.volume = dataMap.toMap().value("volume").toString();
		m_tradeBin1m.close = dataMap.toMap().value("close").toString();
		m_tradeBin1m.high = dataMap.toMap().value("high").toString();
		m_tradeBin1m.low = dataMap.toMap().value("low").toString();
	}
}



void QBizManager::doTrade(const QString & source)
{
	{
		QJsonDocument jDoc = QJsonDocument::fromJson(source.toUtf8());
		QJsonObject jObj = jDoc.object();
		if (!jObj.isEmpty())
		{
			auto jObjmap = jObj.toVariantMap();
			auto data = jObjmap["data"].toList();
			if (data.size() == 0)
				return;
			auto dataMap = data.at(0);

			struct_trade st;
			st.side = dataMap.toMap().value("side").toString();
			st.price = dataMap.toMap().value("price").toString();
			st.size = dataMap.toMap().value("size").toString();
			st.timestamp = dataMap.toMap().value("timestamp").toString();
			if (m_trade.size() <= 100)
				m_trade.push_front(st);
			else if (m_trade.size() >= 100)
			{
				m_trade.push_front(st);
				m_trade.pop_back();
			}
		}
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

bool QBizManager::parse_USDT(const QString & source, QStringList& trade_list)
{
	int pp1 = source.indexOf("[{}]}}]");
	if(pp1!=-1)
		return 0;

	int pp = source.indexOf("data");
	int p = source.indexOf("timestamp", pp);
	int p2 = source.indexOf("indicators", p + 10);
	QString timestamp = source.mid(p+12,p2-p);

	QStringList timestamp_list = timestamp.split(",");
	
	 p = source.indexOf("high", pp);
	 p2 = source.indexOf("]", p + 10);
	QString high = source.mid(p+7, p2 - p);
	QStringList high_list = high.split(",");
	
	p = source.indexOf("low", pp);
	p2 = source.indexOf("]", p + 10);
	QString low = source.mid(p + 6, p2 - p);
	QStringList low_list = low.split(",");

	p = source.indexOf("open", pp);
	p2 = source.indexOf("]", p + 10);
	QString open = source.mid(p + 7, p2 - p);
	QStringList open_list = open.split(",");

	p = source.indexOf("close", pp);
	p2 = source.indexOf("]", p + 10);
	QString close = source.mid(p + 8, p2 - p);
	QStringList close_list = close.split(",");

	{
		QString high = high_list.at(290);
		QString low = low_list.at(300);

		QString high_1 = high_list.at(290);
		QString low_1 = low_list.at(300);


		if (high.toDouble() == 0 || low_1.toDouble() == 0)
			return 0;

		if (high_1.toDouble() == 0 || low_1.toDouble() == 0)
			return 0;


		if (high.toDouble() - low.toDouble() > 0.253 || high_1.toDouble() - low_1.toDouble() > 0.253)
		{
			if (oneord == 1)
				return 0;

			oneord = 1;

			for (int i = 0; i <= 2; i++)

			{
				QEventLoop loop;
				QTimer::singleShot(12000, &loop, SLOT(quit()));
				loop.exec();

				qDebug() << "Alert  12"<< high << " "<< low_1;
				Alert();

			}

			m_TradeTimer_order.start();
			return 0;
		}
	}


	{
		QString high = high_list.at(240);
		QString low = low_list.at(300);

		QString high_1 = high_list.at(240);
		QString low_1 = low_list.at(300);

		if (high.toDouble() - low.toDouble() > 0.253 || high_1.toDouble() - low_1.toDouble() > 0.253)
		{
			if (oneord == 1)
				return 0;

			oneord = 1;

			for (int i = 0; i <= 2; i++)

			{
				QEventLoop loop;
				QTimer::singleShot(12000, &loop, SLOT(quit()));
				loop.exec();

				qDebug() << "Alert  2";
				Alert();

			}

			m_TradeTimer_order.start();
			return 0;
		}
	}

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

	bitmex_USD_Index(soure);
	parse_USDT(soure, m_trade_list_1day);
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
	QString high = m_trade_list_5.at(0).split(",").at(3).split(":").at(1);
	QString low = m_trade_list_5.at(0).split(",").at(4).split(":").at(1);
	QString open = m_trade_list_5.at(0).split(",").at(2).split(":").at(1);
	QString close = m_trade_list_5.at(0).split(",").at(5).split(":").at(1);
	if (open.toDouble() > close.toDouble())
		return 0;

	if (high.toDouble() - low.toDouble()>110)		
		return 1;

	return 0;
}


//严格标准在上方
bool QBizManager::Up_Check_1day()
{
//	return 1;
	int num = 0;
	{
		QString open = m_trade_list_1day.at(0).split(",").at(2).split(":").at(1);
		QString close = m_trade_list_1day.at(0).split(",").at(5).split(":").at(1);
		if (open.toDouble() < close.toDouble())
			num++;
	}

	{
		QString open = m_trade_list_1day.at(1).split(",").at(2).split(":").at(1);
		QString close = m_trade_list_1day.at(1).split(",").at(5).split(":").at(1);
		if (open.toDouble() < close.toDouble())
			num++;
	}


	{
		QString high1 = m_trade_list_1day.at(0).split(",").at(3).split(":").at(1);
		QString high2 = m_trade_list_1day.at(1).split(",").at(3).split(":").at(1);
		if (high1.toDouble() < high2.toDouble())
			return 0;
	}

	if (num >= 2)
		return 1;
	return 0;
}


bool QBizManager::Up_Check_1h()
{
	if (Up_Check_Green_1h())
	{
		QString high1 = m_trade_list_1h.at(0).split(",").at(3).split(":").at(1);
		if(m_price > high1)
			return 1;
	}		

	return 0;
}





//严格标准在上方
bool QBizManager::Up_Check_volume_1h()
{
	if (m_trade_list_1h.size() == 0)
		return 0;

	QString volume5 = m_trade_list_1h.at(4).split(",").at(7).split(":").at(1);
	QString volume4 = m_trade_list_1h.at(3).split(",").at(7).split(":").at(1);
	QString volume3 = m_trade_list_1h.at(2).split(",").at(7).split(":").at(1);
	QString volume2 = m_trade_list_1h.at(1).split(",").at(7).split(":").at(1);
	QString volume1 = m_trade_list_1h.at(0).split(",").at(7).split(":").at(1);

	if (volume1.toDouble() > 200000000)
		return 0;

	if (volume2.toDouble() > 200000000)
		return 0;

	if (volume3.toDouble() > 200000000)
		return 0;

	if (volume4.toDouble() > 200000000)
		return 0;

	if (volume5.toDouble() > 200000000)
		return 0;


	return 1;
}


//严格标准在上方
bool QBizManager::Up_Check_volume_5()
{
	if (m_trade_list_5.size() == 0)
		return 0;

	QString volume9 = m_trade_list_5.at(8).split(",").at(7).split(":").at(1);
	QString volume8 = m_trade_list_5.at(7).split(",").at(7).split(":").at(1);
	QString volume7 = m_trade_list_5.at(6).split(",").at(7).split(":").at(1);
	QString volume6 = m_trade_list_5.at(5).split(",").at(7).split(":").at(1);
	QString volume5 = m_trade_list_5.at(4).split(",").at(7).split(":").at(1);
	QString volume4 = m_trade_list_5.at(3).split(",").at(7).split(":").at(1);
	QString volume3 = m_trade_list_5.at(2).split(",").at(7).split(":").at(1);
	QString volume2 = m_trade_list_5.at(1).split(",").at(7).split(":").at(1);
	QString volume1 = m_trade_list_5.at(0).split(",").at(7).split(":").at(1);

	if (volume1.toDouble() > 200000000)
		return 0;

	if (volume2.toDouble() > 200000000)
		return 0;

	if (volume3.toDouble() > 200000000)
		return 0;;

	if (volume4.toDouble() > 200000000)
		return 0;

	if (volume5.toDouble() > 200000000)
		return 0;

	if (volume6.toDouble() > 200000000)
		return 0;
	return 1;
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


//严格标准在上方
bool QBizManager::Up_Check_Green_1h()
{
	int num = 0;
	{
		QString open = m_trade_list_1h.at(0).split(",").at(2).split(":").at(1);
		QString close = m_trade_list_1h.at(0).split(",").at(5).split(":").at(1);
		if (open.toDouble() < close.toDouble())
			num++;
	}



	{
		QString open = m_trade_list_1h.at(1).split(",").at(2).split(":").at(1);
		QString close = m_trade_list_1h.at(1).split(",").at(5).split(":").at(1);
		if (open.toDouble() < close.toDouble())
			num++;
	}

	{
		QString open = m_trade_list_1h.at(2).split(",").at(2).split(":").at(1);
		QString close = m_trade_list_1h.at(2).split(",").at(5).split(":").at(1);
		if (open.toDouble() < close.toDouble())
			num++;
	}

	{
		QString open = m_trade_list_1h.at(3).split(",").at(2).split(":").at(1);
		QString close = m_trade_list_1h.at(3).split(",").at(5).split(":").at(1);
		if (open.toDouble() > close.toDouble())
			num++;
	}

	{
		QString open = m_trade_list_1h.at(4).split(",").at(2).split(":").at(1);
		QString close = m_trade_list_1h.at(4).split(",").at(5).split(":").at(1);
		if (open.toDouble() > close.toDouble())
			num++;
	}



	{
		QString high1 = m_trade_list_1h.at(1).split(",").at(3).split(":").at(1);
		QString high2 = m_trade_list_1h.at(2).split(",").at(3).split(":").at(1);
		if (high1.toDouble() < high2.toDouble())
			return 0;
	}


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


int QBizManager::Down(QString p)
{
	if (trade_list.size() == 0)
		return 0;
	if (m_trade_list_5.size() == 0)
		return 0;
	if (m_trade_list_1h.size() == 0)
		return 0;
	if (m_trade_list_1day.size() == 0)
		return 0;

	int ret = 0;
	QString low5 = trade_list.at(4).split(",").at(4).split(":").at(1);
	QString low4 = trade_list.at(3).split(",").at(4).split(":").at(1);
	QString low3 = trade_list.at(2).split(",").at(4).split(":").at(1);
	QString low2 = trade_list.at(1).split(",").at(4).split(":").at(1);
	QString low1 = trade_list.at(0).split(",").at(4).split(":").at(1);

	//if (Down_Check_1day())
	//{
	//	ret = 3;
	//}
	//if (Down_Check_1h())
	//{
	//	ret = 4;
	//}
	if (Down_Check_5()) //前面2个5分钟是红色的
	{
		if (!Down_Check_volume_5()) 
			ret = 5;
	}
				//if (Down_Check_volume_1h()) //前面2个5分钟是红色的
				//{

				//	if (Down_Check_volume_5()) //前面2个5分钟是红色的
				//	{

				//		if (low1.toDouble() >= low2.toDouble())
				//			if (low2.toDouble() >= low3.toDouble())
				//			{
				//				QString low = low1;
				//				if (p.toDouble() < low.toDouble())
				//				{
				//					text = "1.两个五分钟是红色";
				//					///return 2;
				//				}
				//			}
				//	}
				//}
	return ret;
	//标准梯度上升 ，第1个回调。无空缺
	if (Up_Low_Check())
	{
		if (Up_Check()) //严格升趋势
		{
			QString low = low2;
			if (p.toDouble() < low.toDouble())
			{
				qDebug() << "Up_Fan  3";
				text = "1标准梯度上升 ，第1个回调";
				return 2;
			}
		}
	}

	//标准梯度上升 ，第1个回调。中间有空缺 上升很快
	if (Up_Low_Check())
	{
		if (Up_Check()) //严格升趋势
		{
			if (Up_Check_Red_Front()) //严格升趋势
			{
				QString low = low2;
				if (p.toDouble() < low.toDouble())
				{
					qDebug() << "Up_Fan  3";
					text = "2标准梯度上升 ，第1个回调。中间有空缺";
					return 2;
				}
			}
		}
	}

	//第三个底部低于等于第二个底部，第二个上升很快 ，第1个回调。
	if (low3.toDouble() <= low2.toDouble())
		if (low1.toDouble() > low2.toDouble())
		{
			QString high2 = trade_list.at(1).split(",").at(3).split(":").at(1);
			if (high2.toDouble() - low2.toDouble() >= 12) //第二个上升很快
			{
				if (low1.toDouble() - low5.toDouble() >= 50) //第二个上升很快
			
				if (Up_Check()) //严格升趋势
				{
					QString low = low1;
					if (p.toDouble() < low.toDouble() )
					{
						text = "3第三个底部低于等于第二个底部，第二个上升很快 ，第1个回调。";
						return 2;
					}
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

bool QBizManager::Down_Add()
{
	if (trade_list.size() == 0)
		return 0;
	if (m_trade_list_5.size() == 0)
		return 0;
	if (m_trade_list_1h.size() == 0)
		return 0;

	if (m_price.length() == 0)
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
			if (m_price.toDouble() < low.toDouble())
			{
				text = "1.两个五分钟是红色";
				return 2;
			}
		}
		
	
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
	if (m_trade_list_5.size() == 0)
		return 0;
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

	if (high1.toDouble() > high2.toDouble()&& high1.toDouble() > high3.toDouble())
		return 0;;

	int num = 0;
	if (high4.toDouble() > high3.toDouble())
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
bool QBizManager::Down_Check_volume_5()
{
	if (m_trade_list_5.size() == 0)
		return 0;
	
	int num = 0;
	QString volume12 = m_trade_list_5.at(11).split(",").at(7).split(":").at(1);
	QString volume11 = m_trade_list_5.at(10).split(",").at(7).split(":").at(1);
	QString volume10 = m_trade_list_5.at(9).split(",").at(7).split(":").at(1);
	QString volume9 = m_trade_list_5.at(8).split(",").at(7).split(":").at(1);
	QString volume8 = m_trade_list_5.at(7).split(",").at(7).split(":").at(1);
	QString volume7 = m_trade_list_5.at(6).split(",").at(7).split(":").at(1);
	QString volume6 = m_trade_list_5.at(5).split(",").at(7).split(":").at(1);
	QString volume5 = m_trade_list_5.at(4).split(",").at(7).split(":").at(1);
	QString volume4 = m_trade_list_5.at(3).split(",").at(7).split(":").at(1);
	QString volume3 = m_trade_list_5.at(2).split(",").at(7).split(":").at(1);
	QString volume2 = m_trade_list_5.at(1).split(",").at(7).split(":").at(1);
	QString volume1 = m_trade_list_5.at(0).split(",").at(7).split(":").at(1);

	if (volume1.toDouble() > 50000000)
		num++;

	if (volume2.toDouble() > 50000000)
		num++;

	if (volume3.toDouble() > 50000000)
		num++;

	if (volume4.toDouble() > 50000000)
		num++;

	if (volume5.toDouble() > 50000000)
		num++;

	if (volume6.toDouble() > 50000000)
		num++;

	if (volume7.toDouble() > 50000000)
		num++;

	if (volume8.toDouble() > 50000000)
		num++;

	if (volume9.toDouble() > 50000000)
		num++;

	if (volume10.toDouble() > 50000000)
		num++;

	if (volume11.toDouble() > 50000000)
		num++;

	if(num>=2)
		return 1;

	return 0;
}




//严格标准在上方
bool QBizManager::Down_Check_volume_1h()
{
	if (m_trade_list_1h.size() == 0)
		return 0;


	QString volume5 = m_trade_list_1h.at(4).split(",").at(7).split(":").at(1);
	QString volume4 = m_trade_list_1h.at(3).split(",").at(7).split(":").at(1);
	QString volume3 = m_trade_list_1h.at(2).split(",").at(7).split(":").at(1);
	QString volume2 = m_trade_list_1h.at(1).split(",").at(7).split(":").at(1);
	QString volume1 = m_trade_list_1h.at(0).split(",").at(7).split(":").at(1);

	if (volume1.toDouble() > 300000000)
		return 0;;

	if (volume2.toDouble() > 300000000)
		return 0;;

	if (volume3.toDouble() > 300000000)
		return 0;;

	if (volume4.toDouble() > 300000000)
		return 0;;

	if (volume5.toDouble() > 300000000)
		return 0;;


	return 1;
}

//严格标准在上方
bool QBizManager::Down_Check_1h()
{
	if (Down_Check_Red_1h()==1)
	{
		QString low = m_trade_list_1h.at(0).split(",").at(4).split(":").at(1);
		if (m_price < low)
			return 1;
	}
	
	if (Down_Check_Red_1h()==2)
	{
		return 1;
	}

	if (Down_Check_Red_1h() == 1)
	{
		return 1;
	}
	return 0;
}


//严格标准在上方
bool QBizManager::Down_Check_1day()
{
	//return 1;
	int num = 0;
	{
		QString open = m_trade_list_1day.at(0).split(",").at(2).split(":").at(1);
		QString close = m_trade_list_1day.at(0).split(",").at(5).split(":").at(1);
		if (open.toDouble() > close.toDouble())
			num++;
	}

	{
		QString open = m_trade_list_1day.at(1).split(",").at(2).split(":").at(1);
		QString close = m_trade_list_1day.at(1).split(",").at(5).split(":").at(1);
		if (open.toDouble() > close.toDouble())
			num++;
	}


	{
		QString high1 = m_trade_list_1day.at(0).split(",").at(3).split(":").at(1);
		QString high2 = m_trade_list_1day.at(1).split(",").at(3).split(":").at(1);
		if (high1.toDouble() > high2.toDouble())
			return 0;
	}

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



//严格标准在上方
int QBizManager::Down_Check_Red_1h()
{
	int num = 0;
	{
		QString open = m_trade_list_1h.at(0).split(",").at(2).split(":").at(1);
		QString close = m_trade_list_1h.at(0).split(",").at(5).split(":").at(1);
		if (open.toDouble() > close.toDouble())
			num++;
	}

	{
		QString open = m_trade_list_1h.at(1).split(",").at(2).split(":").at(1);
		QString close = m_trade_list_1h.at(1).split(",").at(5).split(":").at(1);
		if (open.toDouble() > close.toDouble())
			num++;
	}


	{
		QString high1 = m_trade_list_1h.at(0).split(",").at(3).split(":").at(1);
		QString high2 = m_trade_list_1h.at(1).split(",").at(3).split(":").at(1);	
		if (high1.toDouble() > high2.toDouble())
			return 0;
	}

	{
		QString high1 = m_trade_list_1h.at(0).split(",").at(3).split(":").at(1);
		QString high2 = m_trade_list_1h.at(1).split(",").at(3).split(":").at(1);
		if (high1.toDouble() < high2.toDouble())
		{
			QString low1 = m_trade_list_1h.at(0).split(",").at(4).split(":").at(1);
			QString low2 = m_trade_list_1h.at(1).split(",").at(4).split(":").at(1);
			if (low1.toDouble() < low2.toDouble())
				return 1;
		}
	}

	{
		QString high2 = m_trade_list_1h.at(1).split(",").at(3).split(":").at(1);
		QString high3 = m_trade_list_1h.at(2).split(",").at(3).split(":").at(1);
		if (high2.toDouble() > high3.toDouble())
			return 0;
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

	//if (num >= 1)
		//return 1;

	return num;
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

int QBizManager::Up(QString p)
{
	if (trade_list.size() == 0)
		return 0;
	if (m_trade_list_5.size() == 0)
		return 0;

	int ret = 0;



	
	if (Up_Check_5())//5分钟行情  一致
	{
		ret = 5;
	}
			
	
	
	return ret;

	
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

	if (high1.toDouble() == high2.toDouble())
		if (high2.toDouble() == high3.toDouble())
				return 1;

	return 0;
}



int QBizManager::Side()
{
	if (m_trade.size() <= 9)
		return 0;

	QString strAa = m_trade.at(0).timestamp;
	strAa.replace("T", " ");
	strAa.replace("Z", "");
	QString strAFAa = m_trade.at(m_trade.size() - 1).timestamp;
	strAFAa.replace("T", " ");
	strAFAa.replace("Z", "");

	QDateTime begin_time = QDateTime::fromString(strAa, "yyyy-MM-dd HH:mm:ss.zzz");
	QDateTime end_time = QDateTime::fromString(strAFAa, "yyyy-MM-dd HH:mm:ss.zzz");

	uint stime = begin_time.toTime_t();
	uint etime = end_time.toTime_t();

	int setime = stime - etime;

	int sellnum = 0;
	int buynum = 0;
	for (int i = 0; i < m_trade.size(); i++)
	{
		QString str = m_trade.at(i).side;
		if (str == "Sell")
			sellnum++;

		if (str == "Buy")
			buynum++;
	}


	if (setime < 80)
	{
		if (sellnum > buynum * 1.5)
		{
			text = "sell < 7";
			QString begin_price = m_trade.at(0).price;
			QString end_price = m_trade.at(m_trade.size() - 1).price;
			if (begin_price.toDouble() +0.5 < end_price.toDouble() )
				return 1;
		}

		if (buynum > 1.5 * sellnum )
		{
			text = "buy < 7";
			QString begin_price = m_trade.at(0).price;
			QString end_price = m_trade.at(m_trade.size() - 1).price;
			if (begin_price.toDouble() > end_price.toDouble() + 0.5)
				return 2;
		}

	}

	//if (sellnum > 67)
	//{
	//	text = "amount < 6";

	//	QString strAaprice = m_trade.at(0).price;
	//	QString strAFAaprice = m_trade.at(m_trade.size() - 1).price;

	//	if(strAaprice.toDouble()+0.5<strAFAaprice.toDouble())
	//	return 1;
	//}
	//	

	//if (buynum >67)
	//{
	//	text = "amount < 6";
	//	QString strAaprice = m_trade.at(0).price;
	//	QString strAFAaprice = m_trade.at(m_trade.size() - 1).price;

	//	if (strAaprice.toDouble() > strAFAaprice.toDouble() + 0.5)
	//		return 2;
	//}

	return 0;

}
