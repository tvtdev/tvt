﻿#ifndef QBIZMANAGER_H
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
	bool init();
	//bool initBuy();
	QString GetRate(int rate);

	QString yobit_make_trade(const QString& price, const QString& amount, const QString& type,const QString & pair="tvt_doge");
	QString yobit_ActiveOrders_List(const QString & pair);
	QString yobit_CancelOrder(const QString  & order);
	QString yobit_CreateYobicode(const QString  & a);

	QString yobit_getInfo();
	bool yobit_depth(QString & source);
	QString yobit_trades();
	
	int GetPriceInt(const QString & source);
	QString GetBalance(const QString & bal,int price);
	void doTransfer();
	int doCancle(int type =0);
	int doCancleAll(bool b= false);

	int CancelVol(const QStringList& sell_list);

	int CancelHFT();
	int CancelHFTSell();
	
	void GetBalance();

	int GetPrice(const QString & source,  QStringList& buy_list, QStringList& sell_list);
	double GenAmount();
	int GetMaxOrder(const QStringList& sell_list, const QStringList& buy_list);
	int CheckBuyOrder( const QStringList& buy_list);

	int doBuyMax(const QStringList& buy_list, const QStringList& sell_list);
	int GetMaxOrderBuy(const QStringList& buy_list);
	double GetBuyMaxOrder(const QStringList& buy_list);
	

	int doBuy(const QStringList& buy_list,double );
	int doBuyAll(const QStringList& buy_list);
	void makeBuyOrder(const QString& buy_list);

	void AddTradeVolume(const QStringList& buy_list, const  QStringList& sell_list,int vol=1);
	int Amount_Eth(const QStringList& buy_list);

	void make_bids_eth(const QStringList& buy_list, const  QStringList& sell_list);

	int make_bids_doge_sell(const QStringList& buy_list, const  QStringList& sell_list);
	int make_bids_doge_buy(const QStringList& buy_list, const  QStringList& sell_list);

	void make_bids_doge(const QStringList& buy_list, const  QStringList& sell_list);

//	void  make_bids_doge_buy(const QStringList& buy_list, const  QStringList& sell_list);
	int CheckBuy_big(const QStringList& buy_list);

	QString put_yobit_make_trade_base(const QString& price,  const QString& type, QString buy_price,int);
	QString put_yobit_make_trade(const QString& price, const QString& amount, const QString& type, QString buy_price);
private:
	QString secret ;
	QString m_doge_balance;
	QString m_doge_balance_include;
	QStringList m_buyList;
    
	double m_oenoen;
	int   m_cancleAll;

	double m_dogeea;

};



#endif // QBIZMANAGER_H

