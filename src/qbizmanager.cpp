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
	QString last_prict = 0;
	int num = 0;
	while (1)
	{
		int buynum = GetBuyNumCancelOrder();
		GetBalance();

		QString  source;
		if (!yobit_depth(source))
			continue;

		QStringList buy_list;
		QStringList sell_list;
		GetPrice(source, buy_list, sell_list);

		if ( m_doge_balance.toDouble() > m_my_amount)
			BuyOrder(buy_list);	

		if (buynum == 0)
		{
			qDebug() << "buynum == 0 " << m_my_amount;
			m_my_amount = m_my_amount * 1.05;
			BuyOrder(buy_list);
			QSettings m_settingsa(qApp->applicationDirPath() + "/yobit.ini", QSettings::IniFormat);
			m_settingsa.setValue("yobit/my_amount", QString::number(m_my_amount));
		}

		{
			QString res;
			QString sell_price = sell_list.at(0).split(",").at(0);
			QString str_Rate = QString::number(sell_price.toDouble() - 0.00000002, 'f', 8);
			QString amount = QString::number(50 / str_Rate.toDouble(), 'f', 8);
			res = yobit_make_trade(str_Rate, amount, "sell");
		}
		
		
	}
	return;
}

double QBizManager::GenAmount()
{
	QString nonce = QString("c6%1").arg(QDateTime::currentMSecsSinceEpoch()).mid(5, 9);
	double n = nonce.toInt() % 97 % 8 * 0.1701;
	double n1 = nonce.toInt() % 57 % 10 * 0.5203;
	double n2 = nonce.toInt() % 37 % 9 * 0.0008;
	double n3 = nonce.toInt() % 27 % 4 * 0.09005;
	double n4 = nonce.toInt() % 33 % 7 * 0.000006;
	double n5 = nonce.toInt() % 56 % 3 * 0.0000006;

	return 10.019 + n + n1 + n2 + n3;
}


int QBizManager::doBuyAll(const QStringList& sell_list)
{
	qDebug() << "doBuyAll " << m_doge_balance;
	if (sell_list.size() != 0 && m_doge_balance.toDouble() > 120)
	{
		double total_amount = 0;
		for (int i = 0; i < sell_list.size(); i++)
		{
			QString str = sell_list.at(i);
			QString price = str.split(",").at(0);
			QString amount = str.split(",").at(1);
			total_amount += amount.toDouble()*price.toDouble();

			if (m_doge_balance.toDouble() - total_amount < 500)
			{
				QString sell_price = sell_list.at(i).split(",").at(0);
				QString str_Rate = QString::number(sell_price.toDouble() + 0.00000001, 'f', 8);
				QString amount = QString::number((m_doge_balance.toDouble() - 500) / str_Rate.toDouble(), 'f', 6);
				QString res = yobit_make_trade(str_Rate, amount, "buy");
				qDebug() << "doBuyAll " << str << "m_doge_balance[" << m_doge_balance << "]total_amount" << total_amount << res;
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
						/*		QCoreApplication::exit(0);
						QCoreApplication::quit();
						return 1;*/
					}
				}
				break;
			}
		}
	}
	return 0;
}



int QBizManager::GetMaxOrder(const QStringList& sell_list)
{
	if (sell_list.size() >= 110)
	{
		double total_amount = 0;
		double max_amount = 0;
		int index = 0;
		for (int i = 0; i < sell_list.size() && i<50; i++)
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
		if (m_doge_balance_include.toDouble() - total_amount > 100)
		{			
			qDebug() << "GetMaxOrder m_doge_balance_include exit" << m_doge_balance_include <<"total_amount"<< total_amount <<"str"<< str;
			QFile outFile(qApp->applicationDirPath() + "/oen");
			if (outFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
			{
				QTextStream ts(&outFile);
				ts << 2 << endl;
				outFile.close();
				QCoreApplication::exit(0);
				QCoreApplication::quit();
				return 1;
			}			
		}
	}
	return 0;
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
	return ;
}


QBizManager::QBizManager()
{
	m_cur = 0;
	m_oenoen = 2;	
	secret = "55179f3eb963454c05aebb1a351a0e35";
	initDb();
}

bool QBizManager::initDb()
{
	QSettings m_settingsa(qApp->applicationDirPath() + "/yobit.ini", QSettings::IniFormat);
	m_cur = m_settingsa.value("yobit/cur").toInt();
	m_doge_balance_include_befor = m_settingsa.value("yobit/balance").toString();
	m_my_amount = m_settingsa.value("yobit/my_amount").toDouble();


	//m_order_amount = m_settingsa.value("yobit/amount").toInt();

    return true;
}

bool QBizManager::initBuy()
{	
	
	return true;
}

QBizManager::~QBizManager()
{
}

QString QBizManager::yobit_make_trade(const QString& price, const QString& amount, const QString& type,const QString& pair)
{
	QString nonce = QString("%1").arg(QDateTime::currentMSecsSinceEpoch()).mid(3, 9);
	QString str = "pair="+pair+"&type=" + type + "&rate=" + price + "&amount=" + amount + "&method=Trade&nonce=" + nonce;
	QString sign= QMessageAuthenticationCode::hash(str.toLatin1(), secret.toLatin1(), QCryptographicHash::Sha512).toHex();

	QByteArray send;
	send.append(str);
	QString source;
	QHttpManager::GetInstance().HttpPost("https://yobit.net/tapi/", send, source, sign);
	return  source;
}

QString QBizManager::yobit_ActiveOrders_List(int pair)
{
    QString nonce = QString("%1").arg(QDateTime::currentMSecsSinceEpoch()).mid(3,9);
    QString str = "pair=tvt_doge&method=ActiveOrders&nonce=" + nonce;
	if(pair ==1)
		str = "pair=doge_eth&method=ActiveOrders&nonce=" + nonce;
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
	QString str = "currency=tvt&amount="+ amount +"&method=CreateCoupon&nonce=" + nonce;
	QString sign = QMessageAuthenticationCode::hash(str.toLatin1(), secret.toLatin1(), QCryptographicHash::Sha512).toHex(); //;

	QByteArray send;
	send.append(str);
	QString source;
	QHttpManager::GetInstance().HttpPost("https://yobit.io/tapi/", send, source, sign);
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
	qDebug() << "yobit_depth" << source.mid(0, 100);
	if (source.length() < 50 || source.indexOf("!DOCTYPE html") != -1)
		return  0;
	return  1;

}

QString QBizManager::yobit_trades()
{
	QString source;
	QHttpManager::GetInstance().HttpGet("https://yobit.net/api/3/trades/tvt_doge", source);
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


int QBizManager::checkOrder()
{
	QString orders = yobit_ActiveOrders_List(0);
	if (orders.indexOf("{\"success\":1}") != -1)
		return 0;


	if (orders.indexOf("invalid nonce") != -1)
	{
		return 0;
	}

	QStringList orders_list = orders.mid(18).split("status");
	if (orders_list.size() == 0)
	{
		return 0;
	}

	for (int i = 0; i < orders_list.size() && orders_list.size() != 1; i++)
	{
		QString OrderId = orders_list.at(i).mid(6, 16);
		QString res = yobit_CancelOrder(OrderId);
	}
	return 1;
}



void QBizManager::doCancle(QString st ,int type)
{
	QString orders = yobit_ActiveOrders_List(type);
	if (orders.indexOf("{\"success\":1}") !=-1)
		return ;

	if (orders.indexOf("!DOCTYPE html") != -1|| orders.size()==0)
		QCoreApplication::exit(0);

	if (orders.indexOf("invalid nonce") != -1)
	{
		return;
	}

	QStringList orders_list = orders.mid(18).split("status");
	if (orders_list.size() == 0)
	{
		return;
	}

	for (int i = 0; i < orders_list.size()&& orders_list.size()!=1; i++)
	{
		if (type == 0)
		{
			if (orders_list.at(i).indexOf("tvt_doge") == -1)
				continue;
		}
		QString OrderId = orders_list.at(i).mid(6,16);	

		int pos = orders_list.at(i).indexOf("rate");
		int pos1 = orders_list.at(i).indexOf(",",pos+7);

		QString price = orders_list.at(i).mid(pos+6,pos1-pos-6);

		if (price < st)
			continue;

		QString res = yobit_CancelOrder(OrderId);
	}
	return;
}



void QBizManager::CancelOrder()
{
	QString orders = yobit_ActiveOrders_List(0);
	if (orders.indexOf("{\"success\":1}") != -1)
		return;

	if (orders.indexOf("!DOCTYPE html") != -1 || orders.size() == 0)
		QCoreApplication::exit(0);

	if (orders.indexOf("invalid nonce") != -1)
	{
		return;
	}

	QStringList orders_list = orders.mid(18).split("status");
	if (orders_list.size() == 0)
	{
		return;
	}



	int bu = 1;
	for (int i = 0; i < orders_list.size() && orders_list.size() != 1; i++)
	{
		if (orders_list.at(i).indexOf("tvt_doge") == -1)
			continue;

		if (orders_list.at(i).indexOf("sell") != -1)
			continue;

		QString OrderId = orders_list.at(i).mid(6, 16);

		int pos = orders_list.at(i).indexOf("rate");
		int pos1 = orders_list.at(i).indexOf(",", pos + 7);
		QString price = orders_list.at(i).mid(pos + 6, pos1 - pos - 6);

		QString amount;
		{
			int pos = orders_list.at(i).indexOf("amount");
			int pos1 = orders_list.at(i).indexOf(",", pos + 7);
			amount = orders_list.at(i).mid(pos + 8, pos1 - pos - 8);
		}

		//double af = price.toDouble() * amount.toDouble();
		//if (af < 80)
		//{

		//	m_order_amount = m_order_amount * 1.5;


		//	QSettings m_settingsa(qApp->applicationDirPath() + "/yobit.ini", QSettings::IniFormat);		
		//	m_order_amount = m_settingsa.value("yobit/amount").toInt();
		//	m_settingsa.setValue("yobit/amount", m_doge_balance_include);		
		//}

	}



	int  buy_order_num = 0;
	for (int i = 0; i < orders_list.size() && orders_list.size() != 1; i++)
	{
		if (orders_list.at(i).indexOf("tvt_doge") == -1)
			continue;

		if (orders_list.at(i).indexOf("sell") == -1)
		{
			buy_order_num++;
				continue;
		}
			;

		QString OrderId = orders_list.at(i).mid(6, 16);

		int pos = orders_list.at(i).indexOf("rate");
		int pos1 = orders_list.at(i).indexOf(",", pos + 7);
		QString price = orders_list.at(i).mid(pos + 6, pos1 - pos - 6);

		QString amount;
		{
			int pos = orders_list.at(i).indexOf("amount");
			int pos1 = orders_list.at(i).indexOf(",", pos + 7);
			amount = orders_list.at(i).mid(pos + 8, pos1 - pos - 8);
		}
		QString res = yobit_CancelOrder(OrderId);
	}

	//if (buy_order_num == 0)
	//{

	//	QSettings m_settingsa(qApp->applicationDirPath() + "/yobit.ini", QSettings::IniFormat);

	//	m_cur = 0;
	//	m_settingsa.setValue("yobit/balance", m_doge_balance_include);
	//	m_settingsa.setValue("yobit/cur", QString::number(m_cur));
	//}
	////if (buy_order_num == 1)
	////{

	////	QSettings m_settingsa(qApp->applicationDirPath() + "/yobit.ini", QSettings::IniFormat);

	////	m_cur += 1;
	////	m_settingsa.setValue("yobit/balance", m_doge_balance_include);
	////	m_settingsa.setValue("yobit/cur", QString::number(m_cur));
	////}


	return;
}

void QBizManager::CancleSmall()
{
	QString orders = yobit_ActiveOrders_List(0);
	if (orders.indexOf("{\"success\":1}") != -1)
		return;

	if (orders.indexOf("!DOCTYPE html") != -1 || orders.size() == 0)
		QCoreApplication::exit(0);

	if (orders.indexOf("invalid nonce") != -1)
	{
		return;
	}

	QStringList orders_list = orders.mid(18).split("status");
	if (orders_list.size() == 0)
	{
		return;
	}

	for (int i = 0; i < orders_list.size() && orders_list.size() != 1; i++)
	{
		
		if (orders_list.at(i).indexOf("tvt_doge") == -1)
			continue;
	
		QString OrderId = orders_list.at(i).mid(6, 16);

		int pos = orders_list.at(i).indexOf("rate");
		int pos1 = orders_list.at(i).indexOf(",", pos + 7);
		QString price = orders_list.at(i).mid(pos + 6, pos1 - pos - 6);

		QString amount;
		{
			int pos = orders_list.at(i).indexOf("amount");
			int pos1 = orders_list.at(i).indexOf(",", pos + 7);
			amount = orders_list.at(i).mid(pos + 8, pos1 - pos - 8);
		}



		QString res = yobit_CancelOrder(OrderId);
	}
	return;
}


int QBizManager::delTextOrder()
{
	QString orders = yobit_ActiveOrders_List(0);
	if (orders.indexOf("{\"success\":1}") != -1)
		return 0;

	if (orders.indexOf("!DOCTYPE html") != -1 || orders.size() == 0)
		QCoreApplication::exit(0);

	if (orders.indexOf("invalid nonce") != -1)
	{
		return 0;
	}

	QStringList orders_list = orders.mid(18).split("status");
	if (orders_list.size() == 0)
	{
		return 0;
	}

	for (int i = 0; i < orders_list.size() && orders_list.size() != 1; i++)
	{
		
		if (orders_list.at(i).indexOf("tvt_doge") == -1)
				continue;
		
		QString OrderId = orders_list.at(i).mid(6, 16);

		QString res = yobit_CancelOrder(OrderId);
	}
	return 0;
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

int QBizManager::GetPrice(const QString & source ,QStringList& buy_list, QStringList& sell_list)
{
    int p = source.indexOf("asks");
    int p2 = source.indexOf("bids", p + 10);
    QString sell_price_str = source.mid(p + 8, p2 - p);
    QString buy_price_str = source.mid(p2 + 8);
    buy_list = buy_price_str.split("],[");
    sell_list = sell_price_str.split("],[");
    return 0;
}

void  QBizManager::AddTradeVolume(const QStringList& buy_list, const  QStringList& sell_list,int vol)
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
			amount = QString::number(15.8123456*m_oenoen / str_Rate.toDouble(), 'f', 6);
			yobit_make_trade(str_Rate, amount, "sell");
		}
		return;
	}
	else
	{
		int p = res.indexOf("doge");
		if (p == -1)
			return ;

		int p1 = res.indexOf(",", p);
		m_doge_balance = res.mid(p + 6, p1 - p - 6);
		if (vol == 2)
			return;

		{
			int p = res.indexOf("doge",p1);
			if (p == -1)
				return;

			int p1 = res.indexOf(",", p);
			m_doge_balance_include = res.mid(p + 6, p1 - p - 6);

		}
		if(m_doge_balance.toDouble() < 50 * m_oenoen &&m_doge_balance.toDouble()>1)
		{
			str_Rate = QString::number(buy_price.toDouble() - 0.00000009, 'f', 8);
			amount = QString::number(15.8123456*m_oenoen / str_Rate.toDouble(), 'f', 6);
		}  
	}
}




int QBizManager::GetMaxOrderBuy(const QStringList& buy_list)
{
	if (buy_list.size() >= 110)
	{
		double total_amount = 0;
		double max_amount = 0;
		int index = 0;
		for (int i = 0; i < buy_list.size() ; i++)
		{
			QString str = buy_list.at(i);
			QString price = str.split(",").at(0);
			QString amount = str.split(",").at(1);


			total_amount += amount.toDouble() * price.toDouble();

			if (total_amount >= 20000&&i>5)
				return i;			
		}		
	}
	return 0;
}


int QBizManager::premakeOrder(const QStringList& buy_list, int pos)
{
	QString res;

	{
		QString buy_price = buy_list.at(pos).split(",").at(0);
		QString str_Rate = QString::number(buy_price.toDouble() + 0.00000001, 'f', 8);
		QString amount = QString::number(m_doge_balance.toDouble()*0.02 / str_Rate.toDouble(), 'f', 8);
		res = yobit_make_trade(str_Rate, amount, "buy");
	}


	return 0;
}


int QBizManager::makeOrder(const QStringList& buy_list,int pos)
{
	QString res;

	{
		QString buy_price = buy_list.at(pos + 1).split(",").at(0);
		QString str_Rate = QString::number(buy_price.toDouble() - 0.00000001, 'f', 8);
		QString amount = QString::number(m_doge_balance.toDouble()*0.02 / str_Rate.toDouble(), 'f', 8);
		res = yobit_make_trade(str_Rate, amount, "buy");
	}


	{
		QString buy_price = buy_list.at(pos+2).split(",").at(0);
		QString str_Rate = QString::number(buy_price.toDouble() - 0.00000001, 'f', 8);
		QString amount = QString::number(m_doge_balance.toDouble()*0.15 / str_Rate.toDouble(), 'f', 8);
		res = yobit_make_trade(str_Rate, amount, "buy");
	}

	{
		QString buy_price = buy_list.at(pos+3).split(",").at(0);
		QString str_Rate = QString::number(buy_price.toDouble() - 0.00000001, 'f', 8);
		QString amount = QString::number(m_doge_balance.toDouble()*0.25 / str_Rate.toDouble(), 'f', 8);
		res = yobit_make_trade(str_Rate, amount, "buy");
	}

	{
		QString buy_price = buy_list.at(pos+5).split(",").at(0);
		QString str_Rate = QString::number(buy_price.toDouble() - 0.00000001, 'f', 8);
		QString amount = QString::number(m_doge_balance.toDouble()*0.5 / str_Rate.toDouble(), 'f', 8);
		res = yobit_make_trade(str_Rate, amount, "buy");
	}

 	return 0;
}

int QBizManager::BuyOrder(const QStringList& sell_list)
{
	QString buy_price = sell_list.at(1).split(",").at(0);
	QString str_Rate = QString::number(buy_price.toDouble() + 0.00000003 , 'f', 8);
	QString amount = QString::number((m_doge_balance.toDouble()*0.95-50) / str_Rate.toDouble(), 'f', 8);
	QString res = yobit_make_trade(str_Rate, amount, "buy");
	return 0;
}

int QBizManager::getText(const QStringList& buy_list)
{
	if (buy_list.size() >= 110)
	{
		for (int i = 60; i < buy_list.size(); i++)
		{
			QString str = buy_list.at(i);
			QString price = str.split(",").at(0);
			QString amount = str.split(",").at(1);
			if (amount.indexOf("23211") != -1)
				return 1;
		}
	}
	return 0;
}

int QBizManager::setTextOrder(const QStringList& buy_list)
{
	QString buy_price = buy_list.at(66).split(",").at(0);
	QString str_Rate = QString::number(buy_price.toDouble() - 0.00000021, 'f', 8);
	QString amount = QString::number(11.232111 / str_Rate.toDouble(), 'f', 0)+".232111";
	yobit_make_trade(str_Rate, amount, "buy");
	return 0;
}

void QBizManager::CreateOrderPos(int pos) 
{
	////doCancle();
	//QString source = yobit_depth();
	//
	//QStringList buy_list;
	//QStringList sell_list;
	//GetPrice(source, buy_list, sell_list);

	//AddTradeVolume(buy_list, sell_list);


	//premakeOrder(buy_list, pos);
	//makeOrder(buy_list, pos);
}




int QBizManager::GetBuyNumCancelOrder()
{
	QString orders = yobit_ActiveOrders_List(0);
	if (orders.indexOf("{\"success\":1}") != -1)
		return 0;

	if (orders.indexOf("!DOCTYPE html") != -1 || orders.size() == 0)
		QCoreApplication::exit(0);

	if (orders.indexOf("invalid nonce") != -1)
	{
		return 0;
	}

	QStringList orders_list = orders.mid(18).split("status");
	if (orders_list.size() == 0)
	{
		return 0;
	}
	
	int  buy_order_num = 0;
	for (int i = 0; i < orders_list.size() && orders_list.size() != 1; i++)
	{
		if (orders_list.at(i).indexOf("tvt_doge") == -1)
			continue;

		if (orders_list.at(i).indexOf("buy") != -1)
		{
			buy_order_num++;
			continue;
		}
		QString OrderId = orders_list.at(i).mid(6, 16);
		yobit_CancelOrder(OrderId);
	}
	return buy_order_num;
}