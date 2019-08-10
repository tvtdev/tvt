﻿#include "qbizmanager.h"
#include "qhttpmanager.h"

void QBizManager::doTransfer(const QString & source)
{
	qDebug() << "doTransfer."<< m_volume.toDouble()<< my_postion.currentQty;
	if (m_volume.toDouble() >= 11000000)
	{
		if (my_postion.currentQty.toDouble() >= 0)
		{
			if (my_postion.unrealisedRoePcnt.toDouble() >= 0.01)
			{
				if (my_postion.currentQty.toInt() == 1)
				{
					QUrlQuery param;
					param.addQueryItem("symbol", "XBTUSD");
					param.addQueryItem("orderQty", "1");
					param.addQueryItem("side", "Sell");
					param.addQueryItem("ordType", "Market");
					createOrder(param);
					param.addQueryItem("text", "withdrawal");
					my_postion.currentQty = "0";
					my_postion.unrealisedRoePcnt = "0";
					return;
				}
				QString amount = QString::number(my_postion.currentQty.toDouble()*0.35+1, 'f', 0);
				QUrlQuery param;
				param.addQueryItem("symbol", "XBTUSD");
				param.addQueryItem("orderQty", amount);
				param.addQueryItem("side", "Sell");
				param.addQueryItem("ordType", "Market");
				param.addQueryItem("text", "withdrawal");
				createOrder(param);

				my_postion.currentQty = "0";
				my_postion.unrealisedRoePcnt = "0";

			}

		}
		else if (my_postion.currentQty.toDouble() <= 0)
		{
			qDebug() << "doTransfer.  <= 0" << m_volume.toDouble()<< my_postion.unrealisedRoePcnt;
			if (my_postion.unrealisedRoePcnt.toDouble() >= 0.01)
			{
				if (qAbs(my_postion.currentQty.toInt()) == 1)
				{
					QUrlQuery param;
					param.addQueryItem("symbol", "XBTUSD");
					param.addQueryItem("orderQty", "1");
					param.addQueryItem("side", "Buy");
					param.addQueryItem("ordType", "Market");
					param.addQueryItem("text", "withdrawal");
					createOrder(param);

					my_postion.currentQty = "0";
					my_postion.unrealisedRoePcnt = "0";
					return;
				}
				QString amount = QString::number(qAbs(my_postion.currentQty.toDouble())*0.35+1, 'f', 0);		
				QUrlQuery param;
				param.addQueryItem("symbol", "XBTUSD");
				param.addQueryItem("orderQty", amount);
				param.addQueryItem("side", "Buy");
				param.addQueryItem("ordType", "Market");
				param.addQueryItem("text", "withdrawal");
				createOrder(param);

				my_postion.currentQty = "0";
				my_postion.unrealisedRoePcnt = "0";
			}
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
	 m_price_buy = "";
	 m_price_sell = "";
	 m_price_amount_buy = 0;
	 m_price_amount_sell = 0;
	 QString amouant = "3";// QString::number(my_postion.currentQty.toDouble()*0.5);
	 //amount = QString::number(amount.toInt());
	 QString amount = QString::number(amouant.toDouble()*0.5,'g',0);


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
					m_webSocket.sendTextMessage(QString(R"({"op": "subscribe", "args": ["position"]})"));
					m_webSocket.sendTextMessage(QString(R"({"op": "subscribe", "args": ["tradeBin1m:XBTUSD"]})"));					
				}
			}
		}
		else if (jObj.contains("table")) {	
			
			if (message.indexOf("position") != -1)
			{
				GetPostion(message);
				
			}
			if (message.indexOf("tradeBin1m") != -1)
			{
				GetVolume(message);
			}
			doTransfer(message);
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

void QBizManager::transferMarginPosition(QUrlQuery param)
{
	QHttpManager::GetInstance().query("POST", FUNCTION_TRANSFERMARGIN_POSITION, param, REQUEST_TRANSFERMARGIN_POSITION, true);
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
		qDebug() << "bitmex_depth" << source.mid(0, 10);
		return  0;
	}
	QEventLoop loop;
	QTimer::singleShot(1500, &loop, SLOT(quit()));
	loop.exec();
	return  1;
}

void QBizManager::GetPostion(const QString & source )
{
	if (source.length () >=600)
	{
		int p = source.indexOf("currentQty");
		p = source.indexOf("currentQty", p + 2);
		int	p1 = source.indexOf(",", p + 2);
		my_postion.currentQty = source.mid(p+12, p1-p-12);

		p = source.indexOf("unrealisedRoePcnt");
		p = source.indexOf("unrealisedRoePcnt", p + 2);
		if (p != -1)
		{
			p1 = source.indexOf(",", p + 2);
			my_postion.unrealisedRoePcnt = source.mid(p + 19, p1 - p - 19);
		}
	}else	if (source.length() <= 600&& source.length() >= 399)
	{
		int p = source.indexOf("currentQty");
		int	p1 = source.indexOf(",", p + 2);
		my_postion.currentQty = source.mid(p + 12, p1 - p - 12);

		p = source.indexOf("unrealisedRoePcnt");
		if (p != -1)
		{
			p1 = source.indexOf(",", p + 2);
			my_postion.unrealisedRoePcnt = source.mid(p + 19, p1 - p - 19);
		}
	}
	else	if (source.length() <= 300)
	{
		int p = source.indexOf("currentQty");
		int	p1 = source.indexOf(",", p + 2);
		my_postion.currentQty = source.mid(p + 12, p1 - p - 12);

		if(my_postion.currentQty.toDouble() ==0)	
			my_postion.unrealisedRoePcnt = "0";

	}
	qDebug() << "Postion." << my_postion.currentQty<<" "<< my_postion.unrealisedRoePcnt;
}



void QBizManager::GetVolume(const QString & source)
{
	if (source.length() >= 600)
	{
		int p = source.indexOf("volume");
		p = source.indexOf("volume", p + 2);
		int	p1 = source.indexOf(",", p + 2);
		m_volume = source.mid(p + 8, p1 - p - 8);

	}
	else	if (source.length() <= 600 && source.length() >= 399)
	{
		int p = source.indexOf("currentQty");
		int	p1 = source.indexOf(",", p + 2);
		my_postion.currentQty = source.mid(p + 12, p1 - p - 12);

		p = source.indexOf("unrealisedRoePcnt");
		if (p != -1)
		{
			p1 = source.indexOf(",", p + 2);
			my_postion.unrealisedRoePcnt = source.mid(p + 19, p1 - p - 19);
		}
	}
	else	if (source.length() <= 300)
	{
		int p = source.indexOf("currentQty");
		int	p1 = source.indexOf(",", p + 2);
		my_postion.currentQty = source.mid(p + 12, p1 - p - 12);

		if (my_postion.currentQty.toDouble() == 0)
			my_postion.unrealisedRoePcnt = "0";

	}
	qDebug() << "Postion." << my_postion.currentQty << " " << my_postion.unrealisedRoePcnt;
}
int QBizManager::GetPrice(const QString & source, QStringList& buy_list, QStringList& sell_list)
{
	int pp = source.indexOf("data"); 
	int p = source.indexOf("bids",pp);
	int p2 = source.indexOf("asks", p + 10);
	QString buy_price_str = source.mid(p + 8, p2 - p-7);
	QString sell_price_str = source.mid(p2 + 8);
	buy_list = buy_price_str.split("],[");
	sell_list = sell_price_str.split("],[");
	return 0;
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
	for (int i = avg_size * 2; i < size;i++)
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
