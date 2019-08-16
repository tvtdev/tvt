#include "qbizmanager.h"
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

	int ret_up =  Up_Fan(price_sell);
	int ret_down = Down_Fan(price_sell);

	if (ret_up == 2&& numdddd>=30)
	{
		numdddd = 0;
		if (my_postion.currentQty.toDouble() > 0)
		{
			if (oneordfdsf == 1)
				return;

			oneordfdsf = 1;
			QUrlQuery param;
			param.addQueryItem("symbol", "XBTUSD");
			closePosition(param);

			return;
		}	
	}

	if (ret_down == 2 && numdddd >= 30)
	{
		numdddd = 0;
		if (my_postion.currentQty.toDouble() < 0)
		{
			if (oneordfdsf == 1)
				return;

			oneordfdsf = 1;

			QUrlQuery param;
			param.addQueryItem("symbol", "XBTUSD");
			closePosition(param);
			return;
		}
	}


	if (ret_up == 2)
	{
		if (my_postion.currentQty.toDouble() == 0)
		{
			if (oneord == 1)
				return;

			qDebug() << "Sell  1";
			QUrlQuery param;
			param.addQueryItem("symbol", "XBTUSD");
			param.addQueryItem("orderQty", "1");
			param.addQueryItem("side", "Sell");
			param.addQueryItem("ordType", "Market");
			createOrder(param);
			m_price_buy = "";

			oneord = 1;
			return;
		}
	}

	if (ret_down == 2)
	{
			if (my_postion.currentQty.toDouble() == 0)
			{
				if (oneord == 1)
					return;

				qDebug() << "buy  1";
				QUrlQuery param;
				param.addQueryItem("symbol", "XBTUSD");
				param.addQueryItem("orderQty", "1");
				param.addQueryItem("side", "Buy");
				param.addQueryItem("ordType", "Market");
				createOrder(param);
				m_price_buy = "";

				oneord = 1;
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

bool QBizManager::bitmex_depth(QString & source, QString coinType, QString depth)
{
	QUrlQuery q;
	q.addQueryItem("symbol", coinType);
	q.addQueryItem("depth", depth);
	source = QHttpManager::GetInstance().query("GET", FUNCTION_ORDER_BOOK, q, REQUEST_ORDER_BOOK, true);
	if (source.length() < 50 || source.indexOf("!DOCTYPE html") != -1 || source.indexOf("<!DOCTYPE HTML") != -1 || source.indexOf("error") != -1 || source.indexOf("html>") != -1)
	{
		//qDebug() << "bitmex_depth" << source.mid(0, 10);
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
		//qDebug() << "bitmex_bucketed" << source.mid(0, 10);
		return  0;
	}
	QEventLoop loop;
	QTimer::singleShot(1500, &loop, SLOT(quit()));
	loop.exec();
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
	
	
	//qDebug() << "volume." << my_postion.currentQty << " " << my_postion.unrealisedRoePcnt << m_trade.volume;
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
	oneord = 3;
	oneordfdsf = 3;
	//QDateTime now = QDateTime::currentDateTime();
	
	QString  soure;
	bitmex_bucketed(soure);
	parse_bucketed(soure, trade_list);

	//for (int i = 0; i < trade_list.size(); i++)
	//{
	//	QString  trade_str = trade_list.at(i);
	//	QString  volume = trade_str.split(",").at(7).split(":").at(1);
	//	my_trade.time = trade_str.mid(3, 24);
	//	if (volume.toDouble() >= 16100000)
	//	{
	//		QDateTime _QDateTime = QDateTime::fromString(my_trade.time, "yyyy-MM-ddTHH:mm:ss.sssZ");
	//		QString time_str = QDateTime::fromMSecsSinceEpoch(now.toMSecsSinceEpoch() - _QDateTime.toMSecsSinceEpoch()).toUTC().toString("mm");
	//		if (time_str >= "30")
	//				return;
	//		trade_str = trade_list.at(i);
	//		my_trade.high = trade_str.split(",").at(3).split(":").at(1);
	//		my_trade.low = trade_str.split(",").at(4).split(":").at(1);
	//		
	//		return;
	//	}
	//}
}


int QBizManager::Up_Fan(QString p)
{
	if (trade_list.size() == 0)
		return 0;
	my_trade.low = "";

	QString low5 = trade_list.at(4).split(",").at(4).split(":").at(1);
	QString low4 = trade_list.at(3).split(",").at(4).split(":").at(1);
	QString low3 = trade_list.at(2).split(",").at(4).split(":").at(1);
	QString low2 = trade_list.at(1).split(",").at(4).split(":").at(1);
	QString low1 = trade_list.at(0).split(",").at(4).split(":").at(1);


	if (low1.toDouble() >= low2.toDouble())
		if (low2.toDouble() >= low3.toDouble())
		{
			if (Down_Fan(p) == 2) //三角符合  看前面
			{
				if (low4.toDouble() >= low3.toDouble())
					return 0;
			}
			my_trade.low = low1;
		}

	if (low1.toDouble() >= low2.toDouble())
		if (low1.toDouble() >= low3.toDouble())
		{
			if (low1.toDouble() >= low3.toDouble())
				my_trade.low = low1;
		}

	if (p.toDouble() < my_trade.low.toDouble()  && !my_trade.low.isEmpty())
	{
		numdddd++;
		return 2;
	}




	return 0;
}

int QBizManager::Down_Fan(QString p)
{
	if (trade_list.size() == 0)
		return 0;
	my_trade.high = "";
	
	QString high4 = trade_list.at(2).split(",").at(3).split(":").at(1);
	QString high3 = trade_list.at(2).split(",").at(3).split(":").at(1);
	QString high2 = trade_list.at(1).split(",").at(3).split(":").at(1);
	QString high1 = trade_list.at(0).split(",").at(3).split(":").at(1);
	

	if (high3.toDouble() >= high2.toDouble())
		if (high2.toDouble() >= high1.toDouble())
		{
			my_trade.high = high1;

			if (p.toDouble() > my_trade.high.toDouble() && !my_trade.high.isEmpty())
			{
				numdddd++;
				return 2;
			}
		}

	if (high3.toDouble() >= high2.toDouble())
		if (high3.toDouble() >= high1.toDouble())
		{
			my_trade.high = high1;

			if (p.toDouble() > my_trade.high.toDouble() && !my_trade.high.isEmpty())
			{
				numdddd++;
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
					my_trade.high = high1;

			if (p.toDouble() > my_trade.high.toDouble() && !my_trade.high.isEmpty())
			{
				numdddd++;
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
					my_trade.high = high1;

			if (p.toDouble() > my_trade.high.toDouble() && !my_trade.high.isEmpty())
			{
				numdddd++;
				return 2;
			}
		}
	}

	
	return 0;
}
