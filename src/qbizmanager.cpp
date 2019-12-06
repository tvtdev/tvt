﻿#include "qbizmanager.h"
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
	qsrand(time(NULL));
	QStringList buy_list;
	QStringList sell_list;
	int num = 0;
	while (1)
	{
		CancelHFTSell();
		if (!CancelHFT())			continue;

		GetBalance();
		QString  source;
		if (!yobit_depth(source))			continue;
		GetPrice(source, buy_list, sell_list);
	
		if ( m_doge_balance.toDouble() < 500.001)
		{
			qDebug() << "doBduyAll  1 fd exit" << m_doge_balance;
			CheckBuy_big(buy_list);

			int ret = make_bids_doge_sell(buy_list, sell_list);
			if (ret == 1)
				ret = make_bids_doge_buy(buy_list, sell_list);
		}
		else
		{
			int ret = make_bids_doge_sell(buy_list, sell_list);
			if (ret == 1)
			{
				ret = make_bids_doge_buy(buy_list, sell_list);
				if (ret == 1)
					make_bids_doge(buy_list, sell_list);
			}
		}	
	}
	return;
}

double QBizManager::GenAmount()
{
	QString nonce = QString("c6%1").arg(QDateTime::currentMSecsSinceEpoch()).mid(5, 9);
	double n = nonce.toInt() % 97 % 8 * 0.01701;
	double n1 = nonce.toInt() % 57 % 10 * 0.05203;
	double n2 = nonce.toInt() % 37 % 9 * 0.0008;
	double n3 = nonce.toInt() % 27 % 4 * 0.009005;
	double n4 = nonce.toInt() % 33 % 7 * 0.000006;
	double n5 = nonce.toInt() % 56 % 3 * 0.0000006;

	return 10.0019 + n + n1 + n2 + n3;
}



int QBizManager::Amount_Eth(const QStringList& buy_list)
{
	QString orders = yobit_ActiveOrders_List("doge_eth");
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



int QBizManager::CheckBuy_big(const QStringList& buy_list)
{
	if (buy_list.size() >= 110)
	{
		//for (int i = 0; i < 1; i++)
		//{
		//	QString str = buy_list.at(i);
		//	QString price = str.split(",").at(0);
		//	QString amount = str.split(",").at(1);

		//	if (amount.toDouble() * price.toDouble() > 100)
		//	{
		//		QString buy_str_Rate_1 = QString::number(price.toDouble() - 0.00000001, 'f', 8);
		//		QString amount_buy_1 = QString::number((amount.toDouble() * price.toDouble() - 10) / buy_str_Rate_1.toDouble(), 'f', 0);
		//		yobit_make_trade(buy_str_Rate_1, amount_buy_1, "sell");
		//	}
		//}


		for (int i = 0; i < 2; i++)
		{
			QString str = buy_list.at(i);
			QString price = str.split(",").at(0);
			QString amount = str.split(",").at(1);

			if (amount.toDouble() * price.toDouble() > 1000)
			{
				QString buy_str_Rate_1 = QString::number(price.toDouble() - 0.00000001, 'f', 8);
				QString amount_buy_1 = QString::number(500/ buy_str_Rate_1.toDouble(), 'f', 0);
				qDebug() << yobit_make_trade(buy_str_Rate_1, amount_buy_1, "sell");
			}
		}


		//for (int i = 0; i < 5; i++)
		//{
		//	QString str = buy_list.at(i);
		//	QString price = str.split(",").at(0);
		//	QString amount = str.split(",").at(1);

		//	if (amount.toDouble() * price.toDouble() > 1000)
		//	{
		//		QString buy_str_Rate_1 = QString::number(price.toDouble() - 0.00000001, 'f', 8);
		//		QString amount_buy_1 = QString::number(amount.toDouble() + 10, 'f', 0);
		//		yobit_make_trade(buy_str_Rate_1, amount_buy_1, "sell");
		//	}
		//}


		/*for (int i = 0; i < 15; i++)
		{
		QString str = buy_list.at(i);
		QString price = str.split(",").at(0);
		QString amount = str.split(",").at(1);
		if (amount.toDouble() * price.toDouble() > 5000)
		{
		QString buy_str_Rate_1 = QString::number(price.toDouble() - 0.00000001, 'f', 8);
		QString amount_buy_1 = QString::number(amount.toDouble() + 10, 'f', 0);
		yobit_make_trade(buy_str_Rate_1, amount_buy_1, "sell");
		}
		}*/
	}

	return 0;
}

int QBizManager::doBuyAll(const QStringList& sell_list)
{
	if (sell_list.size() != 0 && m_doge_balance.toDouble() > 120)
	{
		double total_amount = 0;
		for (int i = 0; i < sell_list.size(); i++)
		{
			QString str = sell_list.at(i);
			QString price = str.split(",").at(0);
			QString amount = str.split(",").at(1);
			total_amount += amount.toDouble()*price.toDouble();

			if (m_doge_balance.toDouble() - total_amount < 100)
			{
				QString sell_price = sell_list.at(i).split(",").at(0);
				QString str_Rate = QString::number(sell_price.toDouble() + 0.00000001, 'f', 8);
				QString amount = QString::number((m_doge_balance.toDouble() - 100) / str_Rate.toDouble(), 'f', 6);
				QString res = yobit_make_trade(str_Rate, amount, "buy");
			//	qDebug() << "doBuyAll " << str << "m_doge_balance[" << m_doge_balance << "]total_amount" << total_amount << res;
				if (res.indexOf("success\":1") != -1)
				{
					qDebug() << "doBuyAll  exit";
					QFile outFile(qApp->applicationDirPath() + "/oen");
					if (outFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
					{
						qDebug() << "doBuyAll WriteOnly ";
						QTextStream ts(&outFile);
						ts << 1 << endl;
						outFile.close();
						//QCoreApplication::exit(0);
						//QCoreApplication::quit();
						//return 1;
					}
				}
				break;
			}
		}
	}
	return 0;
}




int QBizManager::GetMaxOrder(const QStringList& sell_list, const QStringList& buy_list)
{
	if (sell_list.size() >= 110)
	{
		double total_amount = 0;
		double max_amount = 0;
		int index = 0;
		for (int i = 0; i < 50; i++)
		{
			QString str = sell_list.at(i);
			QString price = str.split(",").at(0);
			QString amount = str.split(",").at(1);

			double _amount = amount.toDouble() * price.toDouble();
			if (_amount > max_amount)
			{
				max_amount = _amount;
				index = i;
			}
		}

		QString str = sell_list.at(index);
		for (int i = 0; i < index + 1; i++)
		{
			QString str = sell_list.at(i);
			QString price = str.split(",").at(0);
			QString amount = str.split(",").at(1);
			total_amount += amount.toDouble() * price.toDouble();
		}

	
	}

	return 0;
}



int QBizManager::CheckBuyOrder( const QStringList& buy_list)
{
	if (buy_list.size() >= 110)
	{
		double total_amount = 0;
		for (int i = 0; i < 5; i++)
		{
			QString str = buy_list.at(i);
			QString price = str.split(",").at(0);
			QString amount = str.split(",").at(1);
			total_amount += amount.toDouble() * price.toDouble();			
		}

		m_oenoen = total_amount / 1000;
		if (total_amount <= 15000)
			m_oenoen = 1.28;


		if (total_amount <= 10000)
			m_oenoen = 0.1;

		{
			double total_amount = 0;
			for (int i = 0; i < 3; i++)
			{
				QString str = buy_list.at(i);
				QString price = str.split(",").at(0);
				QString amount = str.split(",").at(1);
				total_amount += amount.toDouble() * price.toDouble();
			}
			if (total_amount <= 15000)
				m_oenoen = 1.28;

			if (total_amount <= 10000)
				m_oenoen = 0.1;

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
	
	init();
}

bool QBizManager::init()
{
	QString  source;
	if (!yobit_depth(source))
		return 0;


	QSettings m_settingsa(qApp->applicationDirPath() + "/yobit.ini", QSettings::IniFormat);
	secret = m_settingsa.value("yobit/secret").toString();
	QString key = m_settingsa.value("yobit/key").toString();
	QHttpManager::GetInstance().SetKey(key);

	m_oenoen =  1.2;
	GetBalance();

	return true;
}

void QBizManager::GetBalance()
{
	QString res = yobit_getInfo();
	int p = res.indexOf("doge");
	if (p == -1)
		return;

	int p1 = res.indexOf(",", p);
	m_doge_balance = res.mid(p + 6, p1 - p - 6);

	{
		int p = res.indexOf("doge", p1);
		if (p == -1)
			return;
		int p1 = res.indexOf(",", p);
		m_doge_balance_include = res.mid(p + 6, p1 - p - 6);
	}
	return;
}


QBizManager::~QBizManager()
{
}

QString QBizManager::yobit_make_trade(const QString& price, const QString& amount, const QString& type, const QString& pair)
{
	QString nonce = QString("%1").arg(QDateTime::currentMSecsSinceEpoch()).mid(3, 9);
	QString str = "pair=" + pair + "&type=" + type + "&rate=" + price + "&amount=" + amount + "&method=Trade&nonce=" + nonce;
	QString sign = QMessageAuthenticationCode::hash(str.toLatin1(), secret.toLatin1(), QCryptographicHash::Sha512).toHex();

	QByteArray send;
	send.append(str);
	QString source;
	QHttpManager::GetInstance().HttpPost("https://yobit.net/tapi/", send, source, sign);
	return  source;
}

QString QBizManager::yobit_ActiveOrders_List(const QString & pair)
{
	QString nonce = QString("%1").arg(QDateTime::currentMSecsSinceEpoch()).mid(3, 9);
	QString str = QString("pair=%1&method=ActiveOrders&nonce=%2").arg(pair).arg(nonce);
	//if (pair == 1)
		//str = "pair=doge_eth&method=ActiveOrders&nonce=" + nonce;
	QString sign = QMessageAuthenticationCode::hash(str.toLatin1(), secret.toLatin1(), QCryptographicHash::Sha512).toHex(); //;

	QByteArray send;
	send.append(str);
	QString source;
	QHttpManager::GetInstance().HttpPost("https://yobit.io/tapi/", send, source, sign);
	//qDebug() << "yobit_ActiveOrders_List" << pair<<source;
	return  source;
}

QString QBizManager::yobit_CreateYobicode(const QString & amount)
{
	QString nonce = QString("%1").arg(QDateTime::currentMSecsSinceEpoch()).mid(3, 9);
	QString str = "currency=tvt&amount=" + amount + "&method=CreateCoupon&nonce=" + nonce;
	QString sign = QMessageAuthenticationCode::hash(str.toLatin1(), secret.toLatin1(), QCryptographicHash::Sha512).toHex(); //;

	QByteArray send;
	send.append(str);
	QString source;
	QHttpManager::GetInstance().HttpPost("https://yobit.io/tapi/", send, source, sign);
	return  source;
}

QString QBizManager::yobit_getInfo()
{
	QString nonce = QString("%1").arg(QDateTime::currentMSecsSinceEpoch()).mid(3, 9);
	QString str = "method=getInfo&nonce=" + nonce;
	QString sign = QMessageAuthenticationCode::hash(str.toLatin1(), secret.toLatin1(), QCryptographicHash::Sha512).toHex();

	QByteArray send;
	send.append(str);
	QString source;
	QHttpManager::GetInstance().HttpPost("https://yobit.net/tapi/", send, source, sign);
	return  source;
}

QString QBizManager::yobit_CancelOrder(const QString & order)
{
	QString nonce = QString("%1").arg(QDateTime::currentMSecsSinceEpoch()).mid(3, 9);
	QString str = "order_id=" + order + "&method=CancelOrder&nonce=" + nonce;
	QString sign = QMessageAuthenticationCode::hash(str.toLatin1(), secret.toLatin1(), QCryptographicHash::Sha512).toHex(); //;

	QByteArray send;
	send.append(str);
	QString source;
	QHttpManager::GetInstance().HttpPost("https://yobit.io/tapi/", send, source, sign);
	return  source;
}

bool QBizManager::yobit_depth(QString & source)
{
	QHttpManager::GetInstance().HttpGet("https://yobit.net/api/3/depth/tvt_doge", source);	
	if (source.length() < 50 || source.indexOf("!DOCTYPE html") != -1 || source.indexOf("<!DOCTYPE HTML") != -1)
	{
		qDebug() << "yobit_depth" << source.mid(0, 10);
		return  0;
	}
	//qDebug() << "yobit_depth" << source.mid(0, 20);
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
	QString orders = yobit_ActiveOrders_List("doge_eth");
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
		QString orders = yobit_ActiveOrders_List("doge_eth");
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
	QString amount = QString::number(GenAmount() * m_oenoen / str_Rate.toDouble(), 'f', 8);
	res = yobit_make_trade(str_Rate, amount, "sell");
	res = yobit_make_trade(str_Rate, amount, "buy");

	if (res.indexOf("error") != -1)
	{
		if (res.indexOf("Insufficient funds") != -1)
		{
			str_Rate = QString::number(buy_price.toDouble() - 0.00000009, 'f', 8);
			amount = QString::number(GenAmount() * m_oenoen*1.025 / str_Rate.toDouble(), 'f', 6);
			yobit_make_trade(str_Rate, amount, "sell");
		}
		return;
	}
	else
	{
		int p = res.indexOf("doge");
		if (p == -1)
			return;

		int p1 = res.indexOf(",", p);
		m_doge_balance = res.mid(p + 6, p1 - p - 6);
	

		{
			int p = res.indexOf("doge", p1);
			if (p == -1)
				return;

			int p1 = res.indexOf(",", p);
			m_doge_balance_include = res.mid(p + 6, p1 - p - 6);

		}
		if (m_doge_balance.toDouble() < GenAmount() * m_oenoen * 1.1 &&m_doge_balance.toDouble()>1)
		{
			str_Rate = QString::number(buy_price.toDouble() - 0.00000009, 'f', 8);
			amount = QString::number(15.8123456*m_oenoen / str_Rate.toDouble(), 'f', 6);
		}
	}
}

void  QBizManager::make_bids_eth(const QStringList& buy_list, const  QStringList& sell_list)
{
	QString res;
	QString buy_price = buy_list.at(0).split(",").at(0);
	QString sell_price = sell_list.at(0).split(",").at(0);

	int sp = (sell_price.toDouble() - buy_price.toDouble()- 0.00001003) / 0.00000001;
	int num = m_doge_balance.toDouble() / 11 -2;


	
	{
		QString amount_buy_1 = QString::number((m_doge_balance.toDouble() - GenAmount() * 1.2) * 0.95, 'f', 0);
		yobit_make_trade("0.001", amount_buy_1, "sell", "doge_eth");
	}



}




int QBizManager::CancelVol(const QStringList& sell_list)
{
	if (sell_list.isEmpty())
		return 0;

	QString buy_price = sell_list.at(0).split(",").at(0);
	

	QString orders = yobit_ActiveOrders_List(0);
	if (orders.indexOf("{\"success\":1}") != -1)
		return 0;


	QStringList orders_list = orders.mid(3).split("status");
	if (orders_list.size() == 0)
	{
		return 0;
	}

	for (int i = 0; i < orders_list.size() && orders_list.size() != 1; i++)
	{

		if (orders_list.at(i).indexOf("tvt_doge") == -1)
			continue;

		if (orders_list.at(i).indexOf("buy") != -1)
			continue;

		QString price = orders_list.at(i).split(",").at(4).split(":").at(1);
		QString amount = orders_list.at(i).split(",").at(3).split(":").at(1);



		QString OrderId = orders_list.at(i).mid(6, 16);
		QString res = yobit_CancelOrder(OrderId);
	}
	return orders_list.size();
}



int   QStringList_find_count(const QStringList& orders_list, QString str)
{
	int num = -1;
	for (int i = 0; i < orders_list.size(); i++)
	{
		if (orders_list.at(i).indexOf("tvt_doge") == -1)
			continue;

		if (orders_list.at(i).indexOf("buy") == -1)
			continue;


		int n = orders_list.at(i).indexOf(str);
		if (n != -1)
			num ++;
	}
	return num;
}



int QBizManager::CancelHFT()
{
	QString orders = yobit_ActiveOrders_List("tvt_doge");
	if (orders.indexOf("{\"success\":1}") != -1)			return 1;
	orders = orders.mid(15);
	orders = orders.left(orders.length() - 7);

	QStringList orders_list = orders.mid(3).split("status");
	if (orders_list.size() == 0) { qDebug() << orders.mid(0, 10); return 0; }

	QStringList bids_orders;
	for (int i = 0; i < orders_list.size(); i++)
	{
		QString amount = orders_list.at(i).split(",").at(3).split(":").at(1);

		if (amount.indexOf(".1313289") != -1)
		{
			bids_orders.push_front(orders_list.at(i));
		}
	}

	if (bids_orders.size() <= 50) { return 1; }
	
	if (bids_orders.length() > 50)
	{
		QString order_str = bids_orders.first();
		QString OrderId = order_str.mid(6, 16);
		QString res = yobit_CancelOrder(OrderId);
		if (res.indexOf("success") != -1)
		{
			qDebug() << "CancelHFT success";
			return 0;
		}
		if (res.indexOf("success\":0,") != -1)
		{
			qDebug() << "CancelHFT success\":0" <<  res;// << " " << res.mid(0, 13);
			return 0;
		}
	}
	return 0;	
}




int QBizManager::CancelHFTSell()
{
	while (1)
	{
		QString orders = yobit_ActiveOrders_List("tvt_doge");
		orders = orders.mid(15);
		if (orders.indexOf("{\"success\":1}") != -1)return 0;
		
		QStringList orders_list = orders.mid(3).split("status");
		if (orders_list.size() == 0)return 0;
			
		int num = 0;		
		for (int i = 0; i < orders_list.size() ; i++)
		{
			if (orders_list.at(i).indexOf("sell") == -1)
				continue;
			num++;
			QString OrderId = orders_list.at(i).mid(6, 16);
			QString res = yobit_CancelOrder(OrderId);
			num++;
		}
		if (num == 0)			return 0;
	}
}

int   QStringList_find(const QStringList& buy_list, QString str)
{
	for (int i = 0; i < buy_list.size(); i++)
	{
		QString stra = buy_list.at(i);
		int n = stra.indexOf(str);
		if (n != -1)
			return i;
	}
	return -1;
}


void  QBizManager::make_bids_doge(const QStringList& buy_list, const  QStringList& sell_list)
{
	QString res;
	QString buy_price = buy_list.at(0).split(",").at(0);
	QString sell_price = sell_list.at(0).split(",").at(0);
	
	QString tmp_price = QString::number((buy_price.toDouble() - 0.00000001), 'f', 8);
	for (int i = 1; i < 15; )
	{
		int n = qrand() % 15;
		QString index_price = QString::number((tmp_price.toDouble() - 0.00000001*n), 'f', 8);
		QString amount = QString::number(GenAmount() / index_price.toDouble(), 'f', 0) + ".1313289";
		res = put_yobit_make_trade(index_price, amount, "buy", buy_price); 
		if (res.indexOf("success") != -1)if (res.indexOf("return") != -1)
		{
			if (CancelHFT())
			{
				i++;
			}
		}
		if (res.indexOf("success\":0,") != -1)
		{
			qDebug() << "make_bids_doge success\":0" << index_price << res;// << " " << res.mid(0, 13);
			return;
		}
	}

}

QString  QBizManager::put_yobit_make_trade_base(const QString& price, const QString& type, QString buy_price,int count)
{
	QString amount_buy_1 = QString::number(GenAmount() * (count ) / price.toDouble(), 'f', 8);
	QString res = yobit_make_trade(price, amount_buy_1, "buy");
	//if (res.indexOf("Insufficient funds") != -1)
	//{
	//	QString str_Rate = QString::number(buy_price.toDouble() - 0.00000009, 'f', 8);
	//	QString tmp_amount = QString::number(GenAmount() * count*1.12 / str_Rate.toDouble(), 'f', 6);
	//	res = yobit_make_trade(str_Rate, tmp_amount, "sell");
	//	if (res.indexOf("success") != -1)if (res.indexOf("return") != -1)
	//	{
	//		res = yobit_make_trade(price, amount_buy_1, "buy");
	//	}
	//}
	return res;
}

QString  QBizManager::put_yobit_make_trade(const QString& price, const QString& amount, const QString& type, QString buy_price)
{
	QString res = yobit_make_trade(price, amount, "buy");
	//if (res.indexOf("Insufficient funds") != -1)
	//{
	//	QString str_Rate = QString::number(buy_price.toDouble() - 0.00000009, 'f', 8);
	//	QString tmp_amount = QString::number(GenAmount() * m_oenoen*1.025 / str_Rate.toDouble(), 'f', 6);
	//	res = yobit_make_trade(str_Rate, tmp_amount, "sell");
	//	if (res.indexOf("success") != -1)if (res.indexOf("return") != -1)
	//	{
	//		res = yobit_make_trade(price, amount, "buy");
	//	}
	//}
	return res;
}

int  QBizManager::make_bids_doge_sell(const QStringList& buy_list, const  QStringList& sell_list)
{
	int ret = 1;
	QString res;
	QString buy_price = buy_list.at(0).split(",").at(0);
	QString sell_price = sell_list.at(0).split(",").at(0);

	QString strfind;
	for (int i = 0; i < 130; i++)
	{
		QString str = buy_list.at(i);
		QString price = str.split(",").at(0);
		strfind += price;
	}

	int num = (sell_price.toDouble() - buy_price.toDouble() ) / 0.00000001 + 2;
	QString tmp_price = QString::number((sell_price.toDouble() - 0.00000012), 'f', 8);
	for (int i = 1; i < num-2; i++)
	{
		QString tmp_buy_price = QString::number((buy_price.toDouble() + 0.00000001*i), 'f', 8);

		if (tmp_buy_price >= tmp_price)
			continue;

		if (strfind.indexOf(tmp_buy_price) != -1)
			continue;

		int count = num - i;
		res = put_yobit_make_trade_base(tmp_buy_price,  "buy", buy_price, 1);		
		ret = 0;
	}


	return ret;
}


int  QBizManager::make_bids_doge_buy(const QStringList& buy_list, const  QStringList& sell_list)
{
	int ret = 1;
	QString res;
	QString buy_price = buy_list.at(0).split(",").at(0);
	QString sell_price = sell_list.at(0).split(",").at(0);

	QString strfind;
	for (int i = 0; i < 130; i++)
	{
		QString str = buy_list.at(i);
		QString price = str.split(",").at(0);
		strfind += price;
	}


	if (ret == 1)
	{
		QString tmp_price = QString::number((buy_price.toDouble() - 0.00000001), 'f', 8);
		for (int i = 1; i < 28; i++)
		{
			QString buy_str_Rate = QString::number((tmp_price.toDouble() - 0.00000001*i), 'f', 8);
			if (buy_str_Rate.mid(6).indexOf("00") != -1)
				continue;
			if (strfind.indexOf(buy_str_Rate) != -1)
				continue;
			QString amount_buy_1 = QString::number(GenAmount() / buy_str_Rate.toDouble(), 'f', 8);
			res = put_yobit_make_trade(buy_str_Rate, amount_buy_1, "buy", buy_price);
			ret = 0;
		}
	}

	return ret;
}
