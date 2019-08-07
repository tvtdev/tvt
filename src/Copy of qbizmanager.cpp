#include "qbizmanager.h"
#include "qhttpmanager.h"

void QBizManager::doTransfer()
{
	while (1)
	{
		QString source;
		if (!bitmex_depth(source))
			continue;

		QStringList buy_list;
		QStringList sell_list;
		GetPrice(source, buy_list, sell_list);


		QString price_sell = sell_list.at(2).split(",").at(4).split(":").at(1);
		QString amount_sell = sell_list.at(2).split(",").at(3).split(":").at(1);

		QString price_buy = buy_list.at(1).split(",").at(4).split(":").at(1);
		QString amount_buy = buy_list.at(1).split(",").at(3).split(":").at(1);

		if (amount_buy.toDouble() >= amount_sell.toDouble() * 20 && amount_buy.length() >= 6 && amount_sell.length() <= 5 && m_price_buy.length() == 0)
		{
			m_price_buy = price_buy;
			continue;
		}		
		if (price_buy >m_price_buy&& m_price_buy.length() != 0)
		{


		qDebug()<<price_buy<<m_price_buy;
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


		if (amount_sell.toDouble() >= amount_buy.toDouble() * 20 && amount_sell.length() >= 6 && amount_buy.length() <= 5 && m_price_sell.length() == 0)
		{
			m_price_sell = price_sell;
			continue;
		}
		if (price_sell < m_price_sell&& m_price_sell.length() != 0)
		{
			qDebug()<<price_sell<<m_price_sell;
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
	
	return;
}

QBizManager::QBizManager()
{
	 m_signed = false;
	 m_currentTotalRequests = 0;
	 m_maxRequests = 60;

	 m_websocketHost = "wss://testnet.bitmex.com/realtime";
    //connect(&m_webSocket, &QWebSocket::connected, this, &QBizManager::connected,Qt::UniqueConnection);
    //connect(&m_webSocket, &QWebSocket::disconnected, this, &QBizManager::closed,Qt::UniqueConnection);
    //connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &QBizManager::textMessageReceived,Qt::UniqueConnection);
    //connect(&m_webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),[=](QAbstractSocket::SocketError error){
    //    qDebug() << "web error:" << error;
    //});
   // m_webSocket.open(QUrl(m_websocketHost));
    //m_maxRequestsTimer.setInterval(1000 * 60);
    //connect(&m_maxRequestsTimer,&QTimer::timeout,this,[&](){
    //    m_currentTotalRequests = 0;
    //});
	 m_price_buy = "";
	 m_price_sell = "";
	 m_price_amount_buy = 0;
	 m_price_amount_sell = 0;
}

QBizManager::~QBizManager()
{
}

void QBizManager::connected()
{
    qDebug() << "websocket connected.";
    m_webSocket.sendTextMessage(R"({"op": "subscribe", "args": ["orderBookL2_25:XBTUSD"]}})");
}

void QBizManager::closed()
{
    qDebug() << "websocket disconnected.";
    m_signed = false;
}

void QBizManager::textMessageReceived(const QString &message)
{
    qDebug() << message;
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


int QBizManager::GetPrice(const QString & source, QStringList& buy_list, QStringList& sell_list)
{
	int p = source.indexOf("Buy");
	QString sell_price_str = source.mid(1, p-46);
	QString buy_price_str = source.mid(p-44);
	buy_price_str = buy_price_str.replace("}", "");
	buy_list = buy_price_str.split("{");
	sell_list = sell_price_str.split("{");

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
