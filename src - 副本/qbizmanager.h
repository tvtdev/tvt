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
	QString GetRate(int rate);

	QString yobit_make_trade(const QString& price, const QString& amount, const QString& type,const QString & pair="tvt_doge");
	QString yobit_ActiveOrders_List(int pair=0);
	QString yobit_CancelOrder(const QString  & order);
	QString yobit_CreateYobicode(const QString  & a);

	
	QString yobit_depth();
	QString yobit_trades();
	
	int GetPriceInt(const QString & source);
	QString GetBalance(const QString & bal,int price);
	void doTransfer();
	void doCancle(int type =0);
	int doCancleAll();


	int GetPrice(const QString & source,  QStringList& buy_list, QStringList& sell_list);
	double GenAmount();
	int CheckBuyList(const QStringList& buy_list);

	int CheckBigOrder(const QStringList& buy_list);

	void doBuyAll(const QStringList& buy_list);
	void doBuy(const QStringList& buy_list);
	void doSell(const QStringList& buy_list);
	void selldididi(const QStringList& buy_list);
	void AddTradeVolume(const QStringList& buy_list, const  QStringList& sell_list,int vol=1);

	void Get_cf_clearance(QString coo, QString & res);
private:
	QString secret ;
	QString m_doge_balance;
	QString m_doge_balance_include;
	QStringList m_buyList;
    
	double m_oenoen;
	int   m_cancleAll;
};



#endif // QBIZMANAGER_H

