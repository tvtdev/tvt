#ifndef QBIZMANAGER_H
#define QBIZMANAGER_H
#include <QObject>
#include "qhttpmanager.h"
#include <QTimer>
#include <QMap>
#include <QHash>
#include <QSet>




class QBizManager : public QObject
{
public:
	QBizManager();

	~QBizManager();
public:
	bool initDb();
	bool initBuy();
	QString GetRate(int rate);

	QString yobit_make_trade(const QString& price, const QString& amount, const QString& type,const QString & pair="tvt_doge");
	QString yobit_ActiveOrders_List(int pair=0);
	QString yobit_CancelOrder(const QString  & order);
	QString yobit_CreateYobicode(const QString  & a);

	QString yobit_getInfo();	
	QString yobit_depth();
	QString yobit_trades();
	
	int GetPriceInt(const QString & source);
	void GetBalance();
	void doTransfer();

	void CancleSmall();
	int doCancleAll(bool b = false);
	void doCancle(QString st = "", int type = 0);
	//int doCancleAll(bool b = false);

	

	void CreateOrderPos(int);
	int zitamakeOrder(const QStringList& buy_list);

	//int signalOrder(const QStringList& buy_list);
	//int readsignal(const QStringList& buy_list);

	int GetPrice(const QString & source,  QStringList& buy_list, QStringList& sell_list);
	double GenAmount();

	int GetMaxOrder(const QStringList& buy_list);
	int GetMaxOrderBuy(const QStringList& sell_list);


	int doBuyAll(const QStringList& buy_list);
	void AddTradeVolume(const QStringList& buy_list, const  QStringList& sell_list,int vol=1);

	int makeOrder(const QStringList& buy_list, int pos);
	int premakeOrder(const QStringList& buy_list, int pos);


	int checkOrder();
	int setTextOrder(const QStringList& buy_list);
	int getText(const QStringList& buy_list);
	int delTextOrder();
private:
	QString secret ;
	QString m_doge_balance;
	QString m_doge_balance_include;
	QStringList m_buyList;
    
	double m_oenoen;
	int   m_cancleAll;

};



#endif // QBIZMANAGER_H

