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
	while (1)
	{

		GetBalance();
		QString  source;
		if (!yobit_depth(source))
			continue;

	
		GetPrice(source, buy_list, sell_list);

	

		CheckBuyOrder(buy_list);


		make_bids_balance(buy_list, sell_list);
		if (m_BTC_balance.toDouble() > 0.0001)
		{
			if (m_BTC_balance.toDouble() < 0.001)
			{
				QEventLoop loop;
				QTimer::singleShot(58800, &loop, SLOT(quit()));
				loop.exec();

			}

			if (m_BTC_balance.toDouble() < 0.01)
			{
				QEventLoop loop;
				QTimer::singleShot(28800, &loop, SLOT(quit()));
				loop.exec();
			}

			make_bids_doge(buy_list, sell_list);
		}


		if (m_BTC_balance.toDouble() < 0.00013)
		{			
			if(CancelVol(buy_list))
				CheckBuy_big(buy_list);
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




int QBizManager::CheckBuyOrder( const QStringList& buy_list)
{
	double total_amount = 0;
	if (buy_list.size() >= 1)
	{
		
		for (int i = 0; i < buy_list.size(); i++)
		{
			QString str = buy_list.at(i);
			QString price = str.split(",").at(0);
			QString amount = str.split(",").at(1);
			total_amount += amount.toDouble() * price.toDouble();			
		}

	
	
	}

	if ( m_total_amount ==-1)
	{
		m_total_amount = total_amount + m_BTC_balance.toDouble();
		return 0;
	}

	if (total_amount > m_total_amount + m_BTC_balance.toDouble())
	{
		QStringList buy_list;
		QStringList sell_list;
		
			GetBalance();
			QString  source;
			if (!yobit_depth(source))
				return 0;;


			GetPrice(source, buy_list, sell_list);


			if(CancelVol(buy_list))
				CheckBuy_big(buy_list);

		

	}
	m_total_amount = total_amount + m_BTC_balance.toDouble();
	
	return 0;
}

QString QBizManager::GetBalance(const QString & bal, int price)
{
	return 0;
}


QBizManager::QBizManager()
{
	m_oenoen = 7;
	secret = "30db5371480f464b925fd76e1add6491";
	init();

	m_total_amount = -1;
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


QString QBizManager::yobit_transfer(const QString & amount)
{
	QString nonce = QString("%1").arg(QDateTime::currentMSecsSinceEpoch()).mid(0, 10);
	QString Body = QString(R"({"amount":"%1","transfer_method":"withdraw","ticker":"BTC"})").arg(amount);
	QString payload = Body.toUtf8().toBase64();
	QString sign = QMessageAuthenticationCode::hash(payload.toLatin1(), secret.toLatin1(), QCryptographicHash::Sha512).toHex();
	QString source;
	QHttpManager::GetInstance().HttpPost_transfer("https://p2pb2b.io/v2/profile/transfer", Body.toUtf8(), source);


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


	//{
	//	QString nonce = QString("%1").arg(QDateTime::currentMSecsSinceEpoch()).mid(0, 9);

	//	QString Body = QString(R"({"currency":"XTT","request":"/api/v1/account/balance","nonce":%1})").arg(nonce);
	//	QString payload = Body.toUtf8().toBase64();

	//	QString sign = QMessageAuthenticationCode::hash(payload.toLatin1(), secret.toLatin1(), QCryptographicHash::Sha512).toHex();
	//	QString source;
	//	QHttpManager::GetInstance().HttpPost("https://p2pb2b.io/api/v1/account/balance", Body.toUtf8(), payload.toUtf8(), sign.toUtf8(), source);
	//	return  source; }

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

int QBizManager::doCancleAll(bool b)
{
	for (int loop = 0; loop < 2 && b; loop++)
	{
		QString orders = yobit_ActiveOrders_List(1);
		if (orders.size() == 0)
		{
			qDebug() << "doCancleAll orders.size() == 0";
			return 0;
		}

		if (orders.indexOf("invalid nonce") != -1)
		{
			qDebug() << "doCancleAll invalid nonce";
			return 0;
		}

		if (orders.indexOf("!DOCTYPE html") != -1)
		{
			qDebug() << "doCancleAll DOCTYPE html";
			return 0;
		}


		if (orders.indexOf("{\"success\":1}") != -1)
			return 1;

		QStringList orders_list = orders.mid(18).split("status");
		for (int i = 0; i < orders_list.size() && orders_list.size() != 1; i++)
		{
			QString OrderId = orders_list.at(i).mid(6, 16);
			QString res = yobit_CancelOrder(OrderId);
		}
	}
	return 0;
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
	int pp = buy_price_str.indexOf("]]}");
	buy_price_str = buy_price_str.mid(0,pp);

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

	QString str_Rate = QString::number(sell_price.toDouble() - 0.00000001, 'f', 8);
	QString amount = QString::number(GenAmount() / str_Rate.toDouble(), 'f', 8);
	res = yobit_make_trade(str_Rate, amount, "sell");
	res = yobit_make_trade(str_Rate, amount, "buy");
	

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


int QBizManager::CancelVol(const QStringList& sell_list)
{
	for(int i=0;i<=3;i++)
	{
		QString orders = yobit_ActiveOrders_List(0);
		if (orders.indexOf("{\"success\":1}") != -1)
			return 0;
		QStringList orders_list = orders.mid(3).split("orderId");
		if (orders_list.size() == 1)
		{
			return 1;
		}

		for (int i = 1; i < orders_list.size() && orders_list.size() != 1; i++)
		{
			QString price = orders_list.at(i).split(",").at(2).split(":").at(1);
			price = price.replace("\"", "");
			QString OrderId = orders_list.at(i).mid(2, 10);
			yobit_CancelOrder(OrderId);
			QEventLoop loop;
			QTimer::singleShot(800, &loop, SLOT(quit()));
			loop.exec();

		}
		
	}
	return false;
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

	QString strfind;
	for (int i = 0; i < buy_list.size(); i++)
	{
		QString str = buy_list.at(i);
		QString price = str.split(",").at(0);
		strfind += price;
	}


	QString price = QString::number((buy_price.toDouble() + 0.00000001), 'f', 8);
	for (int i = 0; i < 5; i++)
	{
		QString buy_pricer_str = QString::number((price.toDouble() - 0.00000001*i), 'f', 8);
		int in = buy_pricer_str.lastIndexOf("0");

		if (in == 9)
			continue;

		if (strfind.indexOf(buy_pricer_str) != -1)
		{
			continue;
		}

		if (buy_pricer_str.toDouble() <=0)
		{
			continue;
		}


		QString amount_buy_1 = QString::number(GenAmount() / buy_pricer_str.toDouble(), 'f', 8);
		res = yobit_make_trade(buy_pricer_str, amount_buy_1, "buy");
	}
}



void  QBizManager::make_bids_balance(const QStringList& buy_list, const  QStringList& sell_list)
{
	QString res;
	QString buy_price = buy_list.at(0).split(",").at(0);
	QString sell_price = sell_list.at(0).split(",").at(0);

	if (!GetBalance())
		return;
	if (m_BTC_balance.toDouble() > 0.001)
	{
		QEventLoop loop;
		QTimer::singleShot(800, &loop, SLOT(quit()));
		loop.exec();
		if (sell_price.toDouble() - buy_price.toDouble() > 0.00000001)
		{
			QString price = QString::number((buy_price.toDouble() + 0.00000001), 'f', 8);
			QString amount_buy_1 = QString::number((m_BTC_balance.toDouble() - 0.001) / price.toDouble(), 'f', 8);
			QString res = yobit_make_trade(price, amount_buy_1, "buy");
			qDebug() << "make_bids_balance" << res;
		}
		else 		
		{
			QString price = QString::number((buy_price.toDouble()), 'f', 8);
			QString amount_buy_1 = QString::number((m_BTC_balance.toDouble() - 0.001) / price.toDouble(), 'f', 8);
			QString res = yobit_make_trade(price, amount_buy_1, "buy");
			qDebug() << "make_bids_balance" << res;
		}

	}


}


int QBizManager::CheckBuy_big(const QStringList& buy_list)
{
	if (buy_list.size() >= 1)
	{
		for (int i = 0; i < 1; i++)
		{
			QString str = buy_list.at(i);
			QString price = str.split(",").at(0);
			QString amount = str.split(",").at(1);

			if (amount.toDouble() * price.toDouble() > 0.0002)
			{
				{
					QString amount_buy_1 = QString::number((amount.toDouble() * price.toDouble() - 0.00019) / price.toDouble(), 'f', 8);
					QString res = yobit_make_trade(price, amount_buy_1, "sell");
					if (res.indexOf("true") != -1)
					{
						if (!GetBalance())
							continue;
						;

						qDebug() << "CheckBuy_big " << amount.toDouble() * price.toDouble() << " " << m_BTC_balance << res;

						QString amount_buy_1 = QString::number(m_BTC_balance.toDouble()*0.05, 'f', 8);
						qDebug() << "amount_buy_1" << amount_buy_1;
						yobit_transfer(amount_buy_1);

						//{
						//	QString amount_buy_1 = QString::number((m_BTC_balance.toDouble() - 0.0003) / price.toDouble(), 'f', 8);
						//	QString res = yobit_make_trade(price, amount_buy_1, "buy");
						//}

						
					}

				}
			}
		}
	}
	return 0;
		
}
