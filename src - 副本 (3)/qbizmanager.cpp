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
	static int num = 0;
	while (1)
	{
		num++;
		doCancle();	
		QString source = yobit_depth();
		if (source.length() < 50)
			continue;

		QStringList buy_list;
		QStringList sell_list;
		GetPrice(source,  buy_list, sell_list);
	
		AddTradeVolume(buy_list, sell_list);
	
		if (m_cancleAll == 2)
		{
			int ret = doCancleAll();
			if (ret == 1)
			{
				doBuyAll(sell_list);
				QFile outFile(qApp->applicationDirPath() + "/oen");
				outFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
				QTextStream ts(&outFile);
				ts << "1" << endl;
				outFile.close();
				QCoreApplication::exit(0);
				return;
			}
		}

		if (m_doge_balance.toDouble() > 100 && m_cancleAll == 1 )
		{
			QString amount = QString::number(m_doge_balance.toDouble() - 80, 'f', 8);
			yobit_make_trade("0.001", amount, "sell", "doge_eth");
			yobit_make_trade("0.001", "100", "sell", "doge_eth");
			yobit_make_trade("0.001", "50", "sell", "doge_eth");		
		}

		if(num % 23 == 1)
			GetMaxOrder(sell_list);	

		if (num % 253 == 1)
			doCancleAll();
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

void QBizManager::doBuy(const QStringList& sell_list)
{
	if (sell_list.size() != 0&& m_doge_balance.toDouble()>10)
	{
		QString sell_price = sell_list.at(0).split(",").at(0);
		int oen = m_doge_balance.toDouble() / 500 + 1;
		QString str_Rate = QString::number(sell_price.toDouble() + 0.00000001, 'f', 8);
		QString amount = QString::number((GenAmount()*oen ) / str_Rate.toDouble(),'f',6);
		yobit_make_trade(str_Rate, amount , "buy");
		

	}
}

void QBizManager::doBuyAll(const QStringList& sell_list)
{
	if (sell_list.size() != 0 )
	{
		double total_amount = 0;
		for (int i = 0; i < sell_list.size(); i++)
		{
			QString str = sell_list.at(i);
			QString price = str.split(",").at(0);
			QString amount = str.split(",").at(1);
			total_amount += amount.toDouble()*price.toDouble();
			
			
		    if (m_doge_balance.toDouble() - total_amount < 100 )
			{				
					qDebug() << "doBuyAll " << str << m_doge_balance << total_amount;
					QString sell_price = sell_list.at(i).split(",").at(0);
					QString str_Rate = QString::number(sell_price.toDouble() + 0.00000001, 'f', 8);
					QString amount = QString::number((m_doge_balance.toDouble()-100 )/ str_Rate.toDouble(), 'f', 6);
					yobit_make_trade(str_Rate, amount, "buy");
					break;				
			}
		}
	}
}


void QBizManager::doSell(const QStringList& buy_list)
{
	if (buy_list.size() != 0)
	{
		double total_amount=0;

		{
			int  index = 0;;
			for (int i = 0; i < 15; i++)
			{
				QString str = buy_list.at(i);
				QString price = str.split(",").at(0);
				QString amount = str.split(",").at(1);
				total_amount += amount.toDouble()*price.toDouble();
			}
		}
		QString str = buy_list.at(0);		
		QString price = str.split(",").at(0);	

		QString str_Rate = QString::number(price.toDouble() - 0.00000001, 'f', 8);
		QString amount = QString::number((23.1) / str_Rate.toDouble(), 'f', 8);

		QString res = yobit_make_trade(price, amount, "sell");
		int p = res.indexOf("doge");
		if (p == -1)
			return;

		int p1 = res.indexOf(",", p);
		m_doge_balance = res.mid(p + 6, p1 - p - 6);
		
	}
}

void QBizManager::selldididi(const QStringList& buy_list)
{
	if (buy_list.size() != 0)
	{
		QString str = buy_list.at(0);
		QString str_Rate =  str.split(",").at(0); 

		QString amounto = str.split(",").at(1);
		int oen = amounto.toDouble()*str_Rate.toDouble()/1000+1;
		QString amount = QString::number(oen * 10.98765432 / str_Rate.toDouble(), 'f', 8);
		yobit_make_trade(str_Rate, amount, "sell");
	}
}

int QBizManager::CheckBuyList(const QStringList& buy_list)
{

	return 0;
}

int QBizManager::CheckBigOrder(const QStringList& sell_list)
{
	if (sell_list.size() != 0)
	{
		 
	//	while (1)
		{	

			double total_amount = 0;
			int n = 0;
			for (int i = 0; i < sell_list.size(); i++)
			{
				QString str = sell_list.at(i);
				QString price = str.split(",").at(0);
				QString amount = str.split(",").at(1);

				total_amount += amount.toDouble()*price.toDouble();

				n++;
				if (amount.toDouble()*price.toDouble() > m_am && n<50)
				{
					if (m_doge_balance_include.toDouble() - total_amount > 5000  )
					{
						if (m_doge_balance_include.toDouble() > 327905.94862544 && m_doge_balance.toDouble() > 71000)
						{
							qDebug() << "CheckBigOrder 2" << m_doge_balance_include << total_amount		 << str;
							QFile outFile(qApp->applicationDirPath() + "/oen");
							outFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
							QTextStream ts(&outFile);
							ts << 2 << endl;
							outFile.close();
							QCoreApplication::exit(0);
							return 0;
						}	
					}	
				}
			}

			if (m_am <= 10000)
			{
				m_am = 21500;
				return 0;
			}

			if (n == sell_list.size())
			{
				m_am -= 1000;
				if (m_doge_balance_include.toDouble() > 55000)
				{
					qDebug() << "CheckBigOrder" << m_doge_balance_include << total_amount;
					QFile outFile(qApp->applicationDirPath() + "/oen");
					outFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
					QTextStream ts(&outFile);
					ts << 2 << endl;
					outFile.close();
					QCoreApplication::exit(0);
					return 0;
				}
			}
		}
	}
	return 0;
}


int QBizManager::GetMaxOrder(const QStringList& sell_list)
{
	if (sell_list.size() != 0)
	{
		double total_amount = 0;
		double max_amount = 0;
		int index = 0;
		for (int i = 0; i < 5; i++)
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

		if (m_doge_balance_include.toDouble() - total_amount > 110)
		{			
			qDebug() << "GetMaxOrder 2" << m_doge_balance_include << total_amount << str;
			QFile outFile(qApp->applicationDirPath() + "/oen");
			outFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
			QTextStream ts(&outFile);
			ts << 2 << endl;
			outFile.close();
			QCoreApplication::exit(0);
			return 0;
			
		}
	}
}

QString QBizManager::GetBalance(const QString & bal, int price)
{
	return 0;
}


QBizManager::QBizManager()
{
	m_oenoen = 2;	
	secret = "4cf8ffde8b741a09f34b0035ea9cf210";
	initDb();
	m_am = 21500;

	QFile oenfile(qApp->applicationDirPath() + "/oen");
	if (oenfile.open(QIODevice::ReadOnly)) {
		QTextStream in(&oenfile);
		while (!in.atEnd()) {		
			m_cancleAll = in.readLine().toInt();
		}
	}

	m_cancleAll = 1;
	oenfile.close();
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
    QString sign = QMessageAuthenticationCode::hash(str.toLatin1(), secret.toLatin1(), QCryptographicHash::Sha512).toHex(); 
    QByteArray send;
    send.append(str);
    QString source;
    QHttpManager::GetInstance().HttpPost("https://yobit.io/tapi/", send, source, sign);
    return  source;
}

QString QBizManager::yobit_depth()
{
    QString source;
    QHttpManager::GetInstance().HttpGet("https://yobit.net/api/3/depth/tvt_doge",  source);
    return  source;
}

QString QBizManager::yobit_trades()
{
	QString source;
	QHttpManager::GetInstance().HttpGet("https://yobit.net/api/3/trades/tvt_doge", source);
	return  source;
}


void QBizManager::doCancle(int type)
{
	QString orders = yobit_ActiveOrders_List(type);

	if (orders.indexOf("{\"success\":1}") !=-1)
		return ;

	if (orders.indexOf("!DOCTYPE html") != -1|| orders.size()==0)
		QCoreApplication::exit(0);

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
	if (orders_list.size() == 0)
	{
		return;
	}

	for (int n = 0; n < 3; n++)
	{

		for (int i = 0; i < orders_list.size()&& orders_list.size()!=1; i++)
		{
			if (type == 0)
			{
				if (orders_list.at(i).indexOf("tvt_doge") == -1)
					continue;
			}


			QString OrderId = orders_list.at(i).mid(6,16);	
			for (int n = 0; n < 3; n++)
			{
				QString res = yobit_CancelOrder(OrderId);
				if (res.indexOf("invalid nonce") == -1)
				{
					break ;
				}
			}
		}

	}

	return;
}

int QBizManager::doCancleAll()
{
	QString orders = yobit_ActiveOrders_List(1);
	if (orders.indexOf("{\"success\":1}") != -1)
		return 1;
	qDebug() << "doCancleAll" << orders;

	if (orders.indexOf("invalid nonce") != -1)
	{
		return 0;
	}

	if (orders.indexOf("!DOCTYPE html") != -1 || orders.size() == 0)
	{
		QFile oenfile(qApp->applicationDirPath() + "/order");
		if (oenfile.open(QIODevice::ReadOnly)) {
			QTextStream in(&oenfile);
			while (!in.atEnd()) {
				orders = in.readAll();
			}
		}
		oenfile.close();
	}
	else
	{
		QFile outFile(qApp->applicationDirPath() + "/order");
		outFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
		QTextStream ts(&outFile);
		ts << orders << endl;
		outFile.close();
	}
	
	QStringList orders_list = orders.mid(18).split("status");
	if (orders_list.size() <= 2 )
	{
		return 1;
	}

	for (int i = 0; i < orders_list.size() && orders_list.size() != 1; i++)
	{
		QString OrderId = orders_list.at(i).mid(6, 16);
		QString res = yobit_CancelOrder(OrderId);	
		qDebug() << "doCancleAll" << res;
	}

	orders = yobit_ActiveOrders_List(1);
	if (orders.indexOf("{\"success\":1}") != -1)
		return 1;
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



