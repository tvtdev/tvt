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
	int num = 0;
	while (1)
	{
		num++;
		if (num % 12 == 1)
			doCancle();	

		QString source = yobit_depth();
		if (source.length() < 50|| source.indexOf("!DOCTYPE html") != -1)
			continue;

		QStringList buy_list;
		QStringList sell_list;
		GetPrice(source,  buy_list, sell_list);	
		AddTradeVolume(buy_list, sell_list);

		
		if (m_doge_balance.toDouble() > 100 && m_cancleAll == 1 )
		{
			QString amount = QString::number(m_doge_balance.toDouble() - 40, 'f', 8);

			yobit_make_trade("0.001", amount, "sell", "doge_eth");
			yobit_make_trade("0.001", "100", "sell", "doge_eth");
			yobit_make_trade("0.001", "50", "sell", "doge_eth");		
		}

		if (num % 123 == 1)
			if (GetMaxOrder(sell_list) == 1)
				return;

		if (num % 53 == 1)
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

void QBizManager::makeBuyOrder(const QString& sell_price)
{
// << "m_doge_balance[" << m_doge_balance << "]total_amount";
	//if (buy_list.size() != 0&& m_doge_balance.toDouble()>10)
	{
		//QString sell_price = buy_list.at(5).split(",").at(0);
		QString str_Rate = QString::number(sell_price.toDouble() - 0.0000001, 'f', 8);
		QString amount = QString::number(10000 / str_Rate.toDouble(),'f',6);
		yobit_make_trade(str_Rate, amount , "buy");

		qDebug() << "makeBuyOrder "<< str_Rate;
	}
}

int QBizManager::doBuyAll(const QStringList& sell_list)
{
	qDebug() << "doBuyAll "<< m_doge_balance;
	if (sell_list.size() != 0 && m_doge_balance.toDouble() > 120)
	{
		double total_amount = 0;
		for (int i = 0; i < sell_list.size(); i++)
		{
			QString str = sell_list.at(i);
			QString price = str.split(",").at(0);
			QString amount = str.split(",").at(1);
			total_amount += amount.toDouble()*price.toDouble();			
			
		    if (m_doge_balance.toDouble() - total_amount < 500 )
			{					
				QString sell_price = sell_list.at(i).split(",").at(0);
				QString str_Rate = QString::number(sell_price.toDouble() + 0.00000001, 'f', 8);
				QString amount = QString::number((m_doge_balance.toDouble()-500 )/ str_Rate.toDouble(), 'f', 6);
				QString res = yobit_make_trade(str_Rate, amount, "buy");
				qDebug() << "doBuyAll " << str << "m_doge_balance[" << m_doge_balance << "]total_amount" << total_amount<< res;
				if (res.indexOf("success\":1") != -1)
				{
					qDebug() << "doBuyAll  exit";
					QFile outFile(qApp->applicationDirPath() + "/oen");
					if (outFile.open(QIODevice::WriteOnly | QIODevice::Truncate| QIODevice::Text))
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


int QBizManager::doBuy(const QStringList& sell_list,double _doge_balance)
{
	qDebug() << "doBuy " << "m_doge_balance[" << m_doge_balance << "]total_amount"  ;
	if (sell_list.size() != 0 )
	{
		double total_amount = 0;
		for (int i = 0; i < sell_list.size(); i++)
		{
			QString str = sell_list.at(i);
			QString price = str.split(",").at(0);
			QString amount = str.split(",").at(1);
			total_amount += amount.toDouble()*price.toDouble();

			if (_doge_balance - total_amount < 1000)
			{
				QString sell_price = sell_list.at(i).split(",").at(0);
				QString str_Rate = QString::number(sell_price.toDouble() + 0.00000001, 'f', 8);
				QString amount = QString::number((_doge_balance - 500) / str_Rate.toDouble(), 'f', 6);
				QString res = yobit_make_trade(str_Rate, amount, "buy");
				qDebug() << "doBuy " << str << "m_doge_balance[" << m_doge_balance << "]total_amount" << total_amount << res;
				if (res.indexOf("success\":1") != -1)
				{
					makeBuyOrder(sell_price);
				}
				break;
			}
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
	m_oenoen = 5;	
	secret = "2d6bb3f1cfb671d8aabba395f7f3c442";
	initDb();
	initBuy();
}

bool QBizManager::initDb()
{
	QFile oenfile(qApp->applicationDirPath() + "/oen");
	if (oenfile.open(QIODevice::ReadOnly)) {
		QTextStream in(&oenfile);
		while (!in.atEnd()) {
			m_cancleAll = in.readLine().toInt();
		}
	}
	oenfile.close();

	{
		QFile ooofile(qApp->applicationDirPath() + "/ooo");
		if (ooofile.open(QIODevice::ReadOnly)) {
			QTextStream in(&ooofile);
			while (!in.atEnd()) {
				m_oenoen = in.readLine().toDouble();
			}
		}
		m_oenoen = m_oenoen*1.09;
		ooofile.close();
	}

	qDebug() << "QBizManager()" << m_cancleAll << m_oenoen;



	

	return true;
}

bool QBizManager::initBuy()
{
	doCancle();
	QString source = yobit_depth();
	QStringList buy_list;
	QStringList sell_list;
	GetPrice(source, buy_list, sell_list);
	for (int i = 0; i < 2; i++)
	{
		AddTradeVolume(buy_list, sell_list);
		int ret = doCancleAll(true);
		if (ret == 1)
			doBuyAll(sell_list);
	}
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
		QString res = yobit_CancelOrder(OrderId);
	}
	return;
}

int QBizManager::doCancleAll(bool b)
{
	for (int n = 0; n < 2&&b; n++)
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




int QBizManager::doBuyMax(const QStringList& buy_list,const QStringList& sell_list)
{
	if (sell_list.size() != 0 && m_doge_balance.toDouble() > 120)
	{
		int max = GetMaxOrderBuy(sell_list);
		double total_amount = 0;
		for (int i = 0; i < sell_list.size(); i++)
		{
			QString str = sell_list.at(i);
			QString price = str.split(",").at(0);
			QString amount = str.split(",").at(1);
			total_amount += amount.toDouble()*price.toDouble();

			if (i > max + 1)
			{
				QString sell_price = sell_list.at(i).split(",").at(0);
				QString str_Rate = QString::number(sell_price.toDouble() + 0.00000001, 'f', 8);
				QString amount = QString::number((total_amount) / str_Rate.toDouble(), 'f', 6);
				QString res = yobit_make_trade(str_Rate, amount, "buy");
				qDebug() << "doBuyMax " << str << "m_doge_balance[" << m_doge_balance << "]total_amount" << total_amount << res;
				if (res.indexOf("success\":1") != -1)
				{
					makeBuyOrder(sell_price);
					qDebug() << "doBuyMax  exit";
					QFile outFile(qApp->applicationDirPath() + "/oen");
					if (outFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
					{
						qDebug() << "doBuyMax WriteOnly ";
						QTextStream ts(&outFile);
						ts << 1 << endl;
						outFile.close();


						QEventLoop loop;
						QTimer::singleShot(60000 * 8, &loop, SLOT(quit()));
						loop.exec();

						QCoreApplication::exit(0);
						QCoreApplication::quit();
						return 1;
					}
				}
				return 1;
			}
		}
	}
	return 0;
}



int QBizManager::GetMaxOrderBuy(const QStringList& sell_list)
{
	if (sell_list.size() >= 110)
	{
		double total_amount = 0;
		double max_amount = 0;
		int index = 0;
		for (int i = 0; i < 10; i++)
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
		return index;

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
		for (int i = 0; i < 150; i++)
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
		for (int i = 0; i < index + 10; i++)
		{
			QString str = sell_list.at(i);
			QString price = str.split(",").at(0);
			QString amount = str.split(",").at(1);
			total_amount += amount.toDouble() * price.toDouble();
		}

		{
			QString price = str.split(",").at(0);
			QString amount = str.split(",").at(1);
			double totalamount = total_amount - m_doge_balance_include.toDouble();

			//	m_oenoen = (totalamount) / 10000 + 1;
			//	if (m_oenoen < 1)
			//	m_oenoen = 1;

		}

		//double dogeea = total_amount - m_doge_balance_include.toDouble();

		//if (dogeea < m_dogeea)
		//{

		//	int ret = doCancleAll();
		//	if (ret == 1)
		//	{

		//		doBuy(sell_list, (m_dogeea- dogeea)*0.5);
		//	}
		//}


		//m_dogeea = dogeea;// total_amount - m_doge_balance_include.toDouble();

		qDebug() << "GetMaxOrder total_amount - m_doge_balance_include " << total_amount - m_doge_balance_include.toDouble() << str;
		if (m_doge_balance_include.toDouble() - total_amount > 1000)
		{
			qDebug() << "GetMaxOrder total_amount - m_doge_balance_include" << m_doge_balance_include << "total_amount" << total_amount << "str" << str;
			QFile outFile(qApp->applicationDirPath() + "/oen");
			if (outFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
			{
				QTextStream ts(&outFile);
				ts << 2 << endl;
				outFile.close();
				//QCoreApplication::exit(0);
				//QCoreApplication::quit();
				//return 1;
			}
		}
	}

	//m_oenoen = m_oenoen * 1.05;
	//{
	//	QFile outFile(qApp->applicationDirPath() + "/ooo");
	//	if (outFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
	//	{

	//		qDebug() << "GetMaxOrder 000" << m_oenoen;
	//		QTextStream ts(&outFile);
	//		ts << m_oenoen << endl;
	//		outFile.close();
	//	}
	//}
	return 0;
}

double QBizManager::GetBuyMaxOrder(const QStringList& buy_list)
{
	if (buy_list.size() >= 110)
	{
		double total_amount = 0;
		double max_amount = 0;
		int index = 0;
		for (int i = 0; i < 120; i++)
		{
			QString str = buy_list.at(i);
			QString price = str.split(",").at(0);
			QString amount = str.split(",").at(1);

			double _amount = amount.toDouble() * price.toDouble();
			if (_amount > max_amount)
			{
				max_amount = _amount;
				index = i;
			}
		}

		QString str = buy_list.at(index);


		QString price = str.split(",").at(0);
		QString amount = str.split(",").at(1);
		double totalamount = amount.toDouble() * price.toDouble();



		return totalamount;
	}
}