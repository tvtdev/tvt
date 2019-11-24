#include "qbizmanager.h"
#include "qhttpmanager.h"
#include <queue>
#include <QObject>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <QVariant>
void QBizManager::doTransfer()
{
	QStringList buy_list;
	QStringList sell_list;
	int num = 0;
	while (1)
	{
		if (!CancelVol())
			continue;

		GetBalance();
		QString  source;
		if (!yobit_depth(source))
			continue;


		GetPrice(source, buy_list, sell_list);
		
		if (Deal_OrderBook(buy_list, sell_list))
			continue;


		if (make_bids_balance(buy_list, sell_list))
			continue;
	

		if (m_BTC_balance.toDouble() > 0.0001)
		{
			if (m_BTC_balance.toDouble() < 0.00051)
			{
				QEventLoop loop;
				QTimer::singleShot(8800, &loop, SLOT(quit()));
				loop.exec();

			}

			if (m_BTC_balance.toDouble() < 0.00031)
			{
				QEventLoop loop;
				QTimer::singleShot(22800, &loop, SLOT(quit()));
				loop.exec();
			}


			AddTradeVolume(buy_list, sell_list);
		}
		else 
		{
			QString res;
			QString buy_price = buy_list.at(0).split(",").at(0);
			QString sell_price = sell_list.at(0).split(",").at(0);

			QString str_Rate = QString::number(buy_price.toDouble() - 0.00000001, 'f', 8);
			QString amount = QString::number(GenAmount() / str_Rate.toDouble(), 'f', 8);
			res = yobit_make_trade(str_Rate, amount, "sell");
		}


	}
	return;
}

double QBizManager::GenAmount()
{
	QString nonce = QString("c6%1").arg(QDateTime::currentMSecsSinceEpoch()).mid(5, 9);
	double n = nonce.toInt() % 97 % 8 * 0.000001701;
	double n1 = nonce.toInt() % 57 % 10 * 0.0000005203;
	double n2 = nonce.toInt() % 37 % 9 * 0.00000008;
	double n3 = nonce.toInt() % 27 % 4 * 0.0000009005;

	return 0.00010009 + n + n1 + n2 + n3;
}




int QBizManager::Deal_OrderBook(const QStringList& buy_list, const  QStringList& sell_list)
{
	QString res;
	QString buy_price = buy_list.at(0).split(",").at(0);
	QString sell_price = sell_list.at(0).split(",").at(0);
	if (sell_price.toDouble() - buy_price.toDouble() <= 0.000000011)
	{
		for (int i = 0; i < 1; i++)
		{
			QString str = sell_list.at(i);
			QString price = str.split(",").at(0);
			QString amount = str.split(",").at(1);


			if (amount.toDouble() * price.toDouble() <= 0.006000)
			{
				if (!CancelAll())
					continue;
				QString str_Rate = QString::number(price.toDouble() + 0.00000001, 'f', 8);
				QString amount_buy_1 = QString::number((amount.toDouble() * price.toDouble()) / price.toDouble(), 'f', 8);
				p2pb2b_make_trade(str_Rate, amount_buy_1, "buy");

				QEventLoop loop;
				QTimer::singleShot(8800, &loop, SLOT(quit()));
				loop.exec();


				return 1;
			}
			else
			{
				if (!CancelAll())
					continue;
				QString str_Rate = QString::number(buy_price.toDouble() - 0.00000001, 'f', 8);
				QString amount = QString::number(GenAmount() / str_Rate.toDouble(), 'f', 8);
				res = p2pb2b_make_trade(str_Rate, amount, "sell");

				QEventLoop loop;
				QTimer::singleShot(8800, &loop, SLOT(quit()));
				loop.exec();
			}

			return 1;
		}
	}
	return 0;
	
}

QString QBizManager::GetBalance(const QString & bal, int price)
{
	return 0;
}


QBizManager::QBizManager()
{
	m_oenoen = 7;
	secret = "1fa3c243a71bbc33c152cdc7c9d0dcfa";
	init();
}

bool QBizManager::init()
{
	QString  source;
	if (!yobit_depth(source))
		return 0;


	QSettings m_settingsa(qApp->applicationDirPath() + "/yobit.ini", QSettings::IniFormat);
	m_oenoen = m_settingsa.value("yobit/cur").toDouble();

	m_oenoen =  2;

	m_settingsa.setValue("yobit/cur", QString::number(m_oenoen));

	GetBalance();

	return true;
}

bool QBizManager::GetBalance()
{
	QString res;

	for (int i = 0; i <= 10; i++)
	{
		QString res = yobit_getInfo();
		if (res.indexOf("true") != -1)
		{
			if (res.indexOf("Too many re") != -1)
				continue;
			int p = res.indexOf("BTC");
			if (p == -1)
				return false;

			int p1 = res.indexOf(",", p);
			m_BTC_balance = res.mid(p + 19, p1 - p - 20);
			return true;
		}

	}

	return false;

}



QBizManager::~QBizManager()
{
}

QString QBizManager::yobit_make_trade(const QString& price, const QString& amount, const QString& type, const QString& pair)
{
	QString nonce = QString("%1").arg(QDateTime::currentMSecsSinceEpoch()).mid(0, 10);
	QString Body = QString(R"({"market":"XTT_BTC","side":"%1","amount":"%2","price":"%3","request":"/api/v1/order/new","nonce":%4})").arg(type).arg(amount).arg(price).arg(nonce);
	QString payload = Body.toUtf8().toBase64();
	QString sign = QMessageAuthenticationCode::hash(payload.toLatin1(), secret.toLatin1(), QCryptographicHash::Sha512).toHex();
	QString source;
	QHttpManager::GetInstance().HttpPost("https://api.p2pb2b.io/api/v1/order/new", Body.toUtf8(), payload.toUtf8(), sign.toUtf8(), source);
	return  source;
}


QString QBizManager::p2pb2b_make_trade(const QString& price, const QString& amount, const QString& type)
{
	QString res;
	for (int i = 0; i <= 5; i++)
	{
		QString res = yobit_make_trade(price, amount, type);
		if (res.indexOf("true") != -1)
			continue;
	}
	
	return  res;
}



QString QBizManager::yobit_ActiveOrders_List(int pair)
{
	QString nonce = QString("%1").arg(QDateTime::currentMSecsSinceEpoch()).mid(0, 10);
	QString Body = QString(R"({"market":"XTT_BTC","offset":0,"limit":100,"request":"/api/v1/orders","nonce":%1})").arg(nonce);
	QString payload = Body.toUtf8().toBase64();
	QString sign = QMessageAuthenticationCode::hash(payload.toLatin1(), secret.toLatin1(), QCryptographicHash::Sha512).toHex();
	QString source;
	QHttpManager::GetInstance().HttpPost("https://api.p2pb2b.io/api/v1/orders", Body.toUtf8(), payload.toUtf8(), sign.toUtf8(), source);
	return  source;
}


QString QBizManager::yobit_CancelOrder(const QString & order)
{
	QString nonce = QString("%1").arg(QDateTime::currentMSecsSinceEpoch()).mid(0, 10);
	QString Body = QString(R"({"market":"XTT_BTC","orderId":%1,"request":"/api/v1/order/cancel","nonce":%2})").arg(order).arg(nonce);
	QString payload = Body.toUtf8().toBase64();
	QString sign = QMessageAuthenticationCode::hash(payload.toLatin1(), secret.toLatin1(), QCryptographicHash::Sha512).toHex();
	QString source;
	QHttpManager::GetInstance().HttpPost("https://api.p2pb2b.io/api/v1/order/cancel", Body.toUtf8(), payload.toUtf8(), sign.toUtf8(), source);
	return  source;
}

QString QBizManager::yobit_getInfo()
{


	QString nonce = QString("%1").arg(QDateTime::currentMSecsSinceEpoch()).mid(0, 10);


	QString Body = QString( R"({"request":"/api/v1/account/balances","nonce":%1})").arg(nonce);
	QString payload = Body.toUtf8().toBase64();


	QString sign = QMessageAuthenticationCode::hash(payload.toLatin1(), secret.toLatin1(), QCryptographicHash::Sha512).toHex();
	QString source;
	QHttpManager::GetInstance().HttpPost("https://p2pb2b.io/api/v1/account/balances", Body.toUtf8(), payload.toUtf8(), sign.toUtf8(), source);
	return  source;


	{
		QString nonce = QString("%1").arg(QDateTime::currentMSecsSinceEpoch()).mid(0, 9);

		QString Body = QString(R"({"currency":"XTT","request":"/api/v1/account/balance","nonce":%1})").arg(nonce);
		QString payload = Body.toUtf8().toBase64();

		QString sign = QMessageAuthenticationCode::hash(payload.toLatin1(), secret.toLatin1(), QCryptographicHash::Sha512).toHex();
		QString source;
		QHttpManager::GetInstance().HttpPost("https://p2pb2b.io/api/v1/account/balance", Body.toUtf8(), payload.toUtf8(), sign.toUtf8(), source);
		return  source; }

}



bool QBizManager::yobit_depth(QString & source)
{
	QHttpManager::GetInstance().HttpGet("https://api.p2pb2b.io/api/v1/public/depth/result?market=XTT_BTC&limit=100", source);	
	if (source.length() < 50 || source.indexOf("!DOCTYPE html") != -1 || source.indexOf("<!DOCTYPE HTML") != -1)
	{
		qDebug() << "yobit_depth" << source.mid(0, 10);
		return  0;
	}
	return  1;
}


QString QBizManager::yobit_trades()
{
	QString source;
	QHttpManager::GetInstance().HttpGet("https://yobit.net/api/3/trades/tvt_doge", source);
	return  source;
}

int QBizManager::doCancle(int type)
{
	QString orders = yobit_ActiveOrders_List(type);
	if (orders.indexOf("{\"success\":1}") != -1)
		return 0;


	QStringList orders_list = orders.mid(18).split("status");
	if (orders_list.size() == 0)
	{
		return 0;
	}

	for (int i = 0; i < orders_list.size() && orders_list.size() != 1; i++)
	{
		if (type == 0)
		{
			if (orders_list.at(i).indexOf("tvt_doge") == -1)
				continue;
		}
		QString OrderId = orders_list.at(i).mid(6, 16);
		QString res = yobit_CancelOrder(OrderId);
	}
	return orders_list.size();
}

int QBizManager::CancelAll()
{
	for (int i = 0; i <= 5; i++)
	{
		QString orders = yobit_ActiveOrders_List(0);
		if (orders.indexOf("{\"success\":1}") != -1)
			return 0;
		if (orders.indexOf("This action is unauthorized") != -1)
			return 0;

		if (orders.indexOf("Too many requests") != -1)
		{
			QEventLoop loop;
			QTimer::singleShot(800, &loop, SLOT(quit()));
			loop.exec();
			continue;
		}



		QStringList orders_list = orders.mid(3).split("orderId");
		if (orders_list.size() == 1)
		{
			return 1;
		}



		for (int i = 1; i < orders_list.size() && orders_list.size() != 1; i++)
		{
			QString price = orders_list.at(i).split(",").at(2).split(":").at(1);
			QString amount = orders_list.at(i).split(",").at(8).split(":").at(1);
			price = price.replace("\"", "");
			amount = amount.replace("\"", "");

		


			QString OrderId = orders_list.at(i).mid(2, 10);
			QString res = yobit_CancelOrder(OrderId);


		}

	}
	return false;
}



int QBizManager::CancelVol()
{
	for (int i = 0; i <= 5; i++)
	{
		QString orders = yobit_ActiveOrders_List(0);
		if (orders.indexOf("{\"success\":1}") != -1)
			return 0;
		if (orders.indexOf("This action is unauthorized") != -1)
			return 0;

		if (orders.indexOf("Too many requests") != -1)
		{
			QEventLoop loop;
			QTimer::singleShot(800, &loop, SLOT(quit()));
			loop.exec();
			continue;
		}



		QStringList orders_list = orders.mid(3).split("orderId");
		if (orders_list.size() == 1)
		{
			return 1;
		}

		if (orders_list.size() == 2)
		{
			QString price = orders_list.at(1).split(",").at(2).split(":").at(1);
			QString amount = orders_list.at(1).split(",").at(8).split(":").at(1);
			price = price.replace("\"", "");
			amount = amount.replace("\"", "");

			if (amount.toDouble() * price.toDouble() >= 0.0001000)
				return 1;
		}


		for (int i = 1; i < orders_list.size() && orders_list.size() != 1; i++)
		{
			QString price = orders_list.at(i).split(",").at(2).split(":").at(1);
			QString amount = orders_list.at(i).split(",").at(8).split(":").at(1);
			price = price.replace("\"", "");
			amount = amount.replace("\"", "");

			if (amount.toDouble() * price.toDouble() >= 0.0001300)
				continue;;


			QString OrderId = orders_list.at(i).mid(2, 10);
			QString res = yobit_CancelOrder(OrderId);


		}

	}
	return false;
}


QString QBizManager::GetRate(int int_rate)
{
	QString rate = QString::number(int_rate);
	QString str_Rate = "0.00000" + rate;
	if (rate.length() == 2)
		str_Rate = "0.000000" + rate;
	if (rate.length() == 3)
		str_Rate = "0.00000" + rate;
	if (rate.length() == 4)
		str_Rate = "0.0000" + rate;
	if (rate.length() == 5)
		str_Rate = "0.000" + rate;
	return str_Rate;
}

int QBizManager::GetPriceInt(const QString & price)
{
	QString buy_price = price.mid(2);
	int ret = buy_price.toInt();

	if (price.length() == 6)
		ret = ret * 10000;
	else if (price.length() == 7)
		ret = ret * 1000;
	else if (price.length() == 8)
		ret = ret * 100;
	else if (price.length() == 9)
		ret = ret * 10;

	return ret;
}

int QBizManager::GetPrice(const QString & source, QStringList& buy_list, QStringList& sell_list)
{
	int p = source.indexOf("asks");
	int p2 = source.indexOf("bids", p + 10);
	QString sell_price_str = source.mid(p + 8, p2 - p);
	QString buy_price_str = source.mid(p2 + 8);
	buy_price_str = buy_price_str.replace("\"","");
	sell_price_str = sell_price_str.replace("\"", "");
	buy_list = buy_price_str.split("],[");
	sell_list = sell_price_str.split("],[");
	return 0;
}

void  QBizManager::AddTradeVolume(const QStringList& buy_list, const  QStringList& sell_list, int vol)
{
	QString res;
	QString buy_price = buy_list.at(0).split(",").at(0);
	QString sell_price = sell_list.at(0).split(",").at(0);


	if (sell_price.toDouble() - buy_price.toDouble() <= 0.000000011)
	{

		return;
	}

	QString str_Rate = QString::number(buy_price.toDouble() + 0.00000001, 'f', 8);
	QString amount = QString::number(GenAmount() / str_Rate.toDouble(), 'f', 8);
	res = p2pb2b_make_trade(str_Rate, amount, "sell");
 qDebug() <<"1"<< res;
	QEventLoop loop;
	QTimer::singleShot(600, &loop, SLOT(quit()));
	loop.exec();
	res = p2pb2b_make_trade(str_Rate, amount, "buy");
 qDebug() << "2" << res;
	if(res.indexOf("Balance not enough") !=-1)
	{		
		QString res;
		QString buy_price = buy_list.at(0).split(",").at(0);
		QString sell_price = sell_list.at(0).split(",").at(0);

		QString str_Rate = QString::number(buy_price.toDouble() - 0.00000001, 'f', 8);
		QString amount = QString::number(GenAmount() / str_Rate.toDouble(), 'f', 8);
		res = p2pb2b_make_trade(str_Rate, amount, "sell");
		qDebug() << "3" << res;
	}
}

void  QBizManager::make_bids_eth(const QStringList& buy_list, const  QStringList& sell_list)
{
	QString res;
	QString buy_price = buy_list.at(0).split(",").at(0);
	QString sell_price = sell_list.at(0).split(",").at(0);

	int sp = (sell_price.toDouble() - buy_price.toDouble()- 0.00001003) / 0.00000001;
	int num = m_BTC_balance.toDouble() / 11 -2;


	
	{
		QString buy_str_Rate_1 = QString::number(buy_price.toDouble() - 0.00001001, 'f', 8);
		QString amount_buy_1 = QString::number((m_BTC_balance.toDouble() - GenAmount() * 1.2) * 0.95, 'f', 0);

		yobit_make_trade("0.001", amount_buy_1, "sell", "doge_eth");
		//res = yobit_make_trade(buy_str_Rate_1, amount_buy_1, "buy");
	}



}




int QBizManager::Amount_Eth( const QStringList& buy_list)
{
	// QStringList orders_list;
	QString orders = yobit_ActiveOrders_List(1);
	if (orders.indexOf("{\"success\":1}") != -1)
		return 0;


	orders.insert(23, "status\":0},");
	QStringList orders_list = orders.mid(3).split("status");
	if (orders_list.size() == 0)
	{
		return 0;
	}

	orders_list.removeFirst();
	orders_list.removeLast();

	double total = 0;
	QString buy_price = buy_list.at(0).split(",").at(0);
	QString buy_str_Rate_1 = QString::number(buy_price.toDouble() - 0.00000521, 'f', 8);

	QString nonce = QString("%1").arg(QDateTime::currentMSecsSinceEpoch()).mid(0, 10);
	for (int i = 0; i < orders_list.size() && orders_list.size() != 1; i++)
	{
		QString amount = orders_list.at(i).split(",").at(3).split(":").at(1);
		total += amount.toDouble();
	}
	return total;

}




void  QBizManager::make_bids_doge(const QStringList& buy_list, const  QStringList& sell_list)
{
	QString res;
	QString buy_price = buy_list.at(0).split(",").at(0);
	QString sell_price = sell_list.at(0).split(",").at(0);	
	{
		QString buy_str_Rate_1 = QString::number(buy_price.toDouble() + 0.00000003, 'f', 8);
		QString amount_buy_1 = QString::number((m_BTC_balance.toDouble() - GenAmount() * 1.2) * 0.99 / buy_str_Rate_1.toDouble(), 'f', 0);
		res = yobit_make_trade(buy_str_Rate_1, amount_buy_1, "buy");
	}
}




bool  QBizManager::make_bids_balance(const QStringList& buy_list, const  QStringList& sell_list)
{

	QString res;
	QString buy_price = buy_list.at(0).split(",").at(0);
	QString sell_price = sell_list.at(0).split(",").at(0);

	if (!GetBalance())
		return false;

	if (m_BTC_balance.toDouble() > 0.001)
	{
		if (!CancelVol())
			return false;;

		if (sell_price.toDouble() - buy_price.toDouble() > 0.00000001)
		{
			QString price = QString::number((buy_price.toDouble() ), 'f', 8);
			QString amount_buy_1 = QString::number((m_BTC_balance.toDouble() - 0.001) / price.toDouble(), 'f', 8);
			QString res = p2pb2b_make_trade(price, amount_buy_1, "buy");
			qDebug() << "make_bids_balance" << res;

			return true;
		}
		else
		{
			QString price = QString::number((buy_price.toDouble()), 'f', 8);
			QString amount_buy_1 = QString::number((m_BTC_balance.toDouble() - 0.001) / price.toDouble(), 'f', 8);
			QString res = p2pb2b_make_trade(price, amount_buy_1, "buy");
			qDebug() << "make_bids_balance" << res;

			return true;
		}

	}
	return false;;

}

