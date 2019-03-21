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
	int loop = 0;
	while (1)
	{
		doCancle();
		QString source = yobit_depth();

		if (source.length() < 50)
			continue;

		{
			QStringList buy_list;
			QStringList sell_list;
			int  int_buy_price = 0;
			int  int_sell_price = 0;
			int price_trade = GetPrice(source, int_buy_price, int_sell_price, buy_list, sell_list);
	

CheckBuyList(buy_list);
		AddTradeVolume(buy_list, sell_list);


			if (m_doge_balance < 1 || m_doge_balance>1000000000)
				continue;

			//GetBalance(res, int_buy_price);

			if(m_doge_balance.toDouble()> 17)				doBuy(sell_list);

			if (loop % 57 == 3)
			{
				selldididi(buy_list);
			}
				
		}
		loop++;

	}
	return;
}

void QBizManager::doBuy(const QStringList& buy_list)
{
	if (buy_list.size() != 0&& m_doge_balance.toDouble()>10)
	{
		double total_amount;
		int  index = 0;;
		for (int i = 0; i <= buy_list.size(); i++)
		{
			QString str = buy_list.at(i);	
			QString price = str.split(",").at(0);
			QString amount = str.split(",").at(1);
			total_amount += amount.toDouble()*price.toDouble();	
			if (total_amount > 15)//m_doge_balance.toDouble())
			{
				index = i;
				break;
			}
		}

		QString str = buy_list.at(index);
		QString price = str.split(",").at(0);
	//	QString amount = QString::number((0.92*m_doge_balance.toDouble() -50) / price.toDouble(),'f',6);
	
QString amount = QString::number((13.08) / price.toDouble(),'f',6);

QString res =	yobit_make_trade(price, amount , "buy");
		
		
int p = res.indexOf("doge");

if (p == -1)
return 0;
int p1 = res.indexOf(",", p);
QString rate = res.mid(p + 6, p1 - p - 6);
//if (rate.toDouble()> 1600) {

QString amount_str = QString::number(rate.toDouble()*0.5, 'f', 8);
yobit_make_trade("0.00001400", "11", "sell", "doge_eth");
//}
		
		
	}
}

void QBizManager::doSell(const QStringList& buy_list)
{
	if (buy_list.size() != 0)
	{		
		QString str = buy_list.at(0);		
		QString amount = str.split(",").at(1);
		QString price = str.split(",").at(0);	
		yobit_make_trade(price, amount, "sell");
	}
}

void QBizManager::selldididi(const QStringList& buy_list)
{
	if (buy_list.size() != 0)
	{
		QString str = buy_list.at(0);
		QString str_Rate =  str.split(",").at(0); 
		QString amount = QString::number(10.1 / str_Rate.toDouble(), 'f', 8);
		yobit_make_trade(str_Rate, amount, "sell");
	}
}

int QBizManager::CheckBuyList(const QStringList& buy_list)
{
	if (m_buyList.size() != 0)
	{
		for (int i = 0; i <= 0; i++)
		{
			QString mstr = m_buyList.at(i);
			QString str = buy_list.at(i);
			if (mstr.split(",").at(0) < str.split(",").at(0))
			{
				qDebug() << "CheckBuyList ," ;

				QString amount = str.split(",").at(1);
				QString price = str.split(",").at(0);

				double  order_amount = amount.toDouble()*0.05;
				double min_amount = 11.71 / price.toDouble();
				if(order_amount <= min_amount)
					order_amount = min_amount;
				QString order_amount_str = QString::number(order_amount, 'f', 8);
		QString res =		yobit_make_trade(price, order_amount_str, "sell");		
			









	continue;
			}
		}
	}	
	m_buyList = buy_list;
	return 0;
}

QString QBizManager::GetBalance(const QString & bal, int price)
{
	int p = bal.indexOf("doge");
	if (p == -1)
		return 0;

	int p1 = bal.indexOf(",", p);
	QString rate = bal.mid(p + 6, p1 - p - 6);

	m_doge_balance = rate;
	if (rate.toDouble()> 18600) {
		yobit_make_trade("0.00035", "50737590", "buy");
	}

	if (rate.toDouble()> 1600) {
		//yobit_make_trade("0.00001400", "17", "sell", "doge_eth");
	}

	if (rate.toDouble() < 250 && rate.toDouble() > 200)
	{

		QString str_Rate = GetRate(price - 2000);
		yobit_make_trade(str_Rate, "35271", "sell");
	}

	if (rate.toDouble() < 200 && rate.toDouble() > 170)
	{
		QString str_Rate = GetRate(price - 2000);
		yobit_make_trade(str_Rate, "58921", "sell");
	}

	if (rate.toDouble() < 150 && rate.toDouble() > 120)
	{
		QString str_Rate = GetRate(price - 2000);
		yobit_make_trade(str_Rate, "78921", "sell");
	}


	if (rate.toDouble() < 120 && rate.toDouble() > 110)
	{
		QString str_Rate = GetRate(price - 2000);
		yobit_make_trade(str_Rate, "73921", "sell");
	}

	if (rate.toDouble() < 80)
	{
		QString str_Rate = GetRate(price - 2000);
		yobit_make_trade(str_Rate, "98921", "sell");
	}

	return 0;
}


QBizManager::QBizManager()
{
	secret = "6196bdad7bbd64a2a81ea6839769a64d";
	initDb();
}

bool QBizManager::initDb()
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

QString QBizManager::yobit_ActiveOrders_List()
{
    QString nonce = QString("%1").arg(QDateTime::currentMSecsSinceEpoch()).mid(3,9);
    QString str = "pair=tvt_doge&method=ActiveOrders&nonce=" + nonce;
    QString sign = QMessageAuthenticationCode::hash(str.toLatin1(), secret.toLatin1(), QCryptographicHash::Sha512).toHex(); //;

    QByteArray send;
    send.append(str);
    QString source;
    QHttpManager::GetInstance().HttpPost("https://yobit.io/tapi/", send, source, sign);
 //   qDebug() << "yobit_ActiveOrders_List" << source;
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
   // qDebug() << "yobit_CancelOrder" << source;
    return  order;
}

QString QBizManager::yobit_depth()
{
    QString source;
    QHttpManager::GetInstance().HttpGet("https://yobit.net/api/3/depth/tvt_doge",  source);
    return  source;
}


void QBizManager::doCancle()
{
	QString orders = yobit_ActiveOrders_List();
	while (true)
	{
		if (orders.indexOf("invalid nonce") == -1)
		{
			break;
		}
		orders = yobit_ActiveOrders_List();
	}

	if (orders.indexOf("invalid nonce") != -1)
	{
		return;
	}
	QStringList orders_list = orders.mid(18).split("status");
	for (int i = 0; i < orders_list.size()&& orders_list.size()!=1; i++)
	{
		QString OrderId = orders_list.at(i).mid(6,16);	
		for (int i = 0; i < 3; i++)
		{
			QString res = yobit_CancelOrder(OrderId);
			if (res.indexOf("invalid nonce") == -1)
			{
				break ;
			}
		}
	}
	return;
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

int QBizManager::GetPrice(const QString & source,int & int_buy_price,int & int_sell_price, QStringList& buy_list, QStringList& sell_list)
{
    int p = source.indexOf("asks");
    int p2 = source.indexOf("bids", p + 10);
    QString sell_price_str = source.mid(p + 8, p2 - p);
    QString buy_price_str = source.mid(p2 + 8);
    buy_list = buy_price_str.split("],[");
    sell_list = sell_price_str.split("],[");

    p = buy_list.at(0).indexOf(",");
    QString buy_price = buy_list.at(0).mid(0, p);
    int_buy_price = GetPriceInt(buy_price);
    p = sell_list.at(0).indexOf(",");
    QString sell_price = sell_list.at(0).mid(0, p);
    int_sell_price = GetPriceInt(sell_price);

    int div = int_sell_price - int_buy_price;
    int ret = int_sell_price - 1;
    ret -= div * 0.1;
	
	for (int i = 0; i < 5; i++)
	{
		QString str = buy_list.at(i).split(",").at(1);
		if (str.toDouble() > 50000000)
		{
			QString price = buy_list.at(i).split(",").at(0);
//			yobit_make_trade(price, str, "sell");
		}
	}
    return ret;
}




void  QBizManager::AddTradeVolume(const QStringList& buy_list, const  QStringList& sell_list)
{
	QString res;
	QString buy_price = buy_list.at(0).split(",").at(0);
	QString sell_price = sell_list.at(0).split(",").at(0);

	QString str_Rate = QString::number(sell_price.toDouble() - 0.00000001, 'f', 8);
	QString amount = QString::number(11 / str_Rate.toDouble(), 'f', 8);
	res = yobit_make_trade(str_Rate, amount, "sell");
        res = yobit_make_trade(str_Rate, amount, "buy");
	if (res.indexOf("error") != -1)
	{
		if (res.indexOf("Insufficient funds") != -1)
		{
			str_Rate = QString::number(buy_price.toDouble() - (sell_price.toDouble() - buy_price.toDouble())*0.2, 'f', 8);
			amount = QString::number(10.01 / str_Rate.toDouble(), 'f', 6);
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

                if(m_doge_balance.toDouble() < 10)
{

str_Rate = QString::number(buy_price.toDouble() - (sell_price.toDouble() - buy_price.toDouble())*0.2, 'f', 8);

amount = QString::number(20.01 / str_Rate.toDouble(), 'f', 6);


qDebug()<<"sffgg"<<str_Rate<<amount<<m_doge_balance<<res;
yobit_make_trade(str_Rate, amount, "sell");


}

	
}
}

int QBizManager::GenAmount()
{
	QString nonce = QString("c6%1").arg(QDateTime::currentMSecsSinceEpoch()).mid(5, 9);
	int pt = nonce.toInt() % 97;
	double n1 = nonce.toInt() % 97 % 8 * 0.0016;
	double n2 = nonce.toInt() % 57 % 10 * 0.00001;
	double n3 = nonce.toInt() % 37 % 9 * 0.000001;

	//double d_vol = (0.001601884 + n1 + n2 + n3) / str_Rate.toDouble();
	return 0;
}

