
#ifndef QBIZMANAGER_H
#define QBIZMANAGER_H

#include <QObject>
#include <QDebug>
#include <QMessageAuthenticationCode>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>
#include <QDateTime>
#include <QUrlQuery>
#include <QEventLoop>
#include <QtWebSockets/QWebSocket>
#define FUNCTION_WALLET "/user/wallet"
#define FUNCTION_CREATE_ORDER "/order"
#define FUNCTION_closePosition_ORDER "/order/closePosition"
#define FUNCTION_QUERY_ORDER "/order"
#define FUNCTION_CANCEL_ORDER "/order"
#define FUNCTION_ORDER_BOOK "/orderBook/L2"
#define FUNCTION_QUERY_POSITION "/position"
#define FUNCTION_ISOLATE_POSITION "/position/isolate"
#define FUNCTION_LEVERAGE_POSITION "/position/leverage"
#define FUNCTION_RISKLIMIT_POSITION "/position/riskLimit"
#define FUNCTION_TRANSFERMARGIN_POSITION "/position/transferMargin"
#define WEBSOCKET_OP_AUTH "authKeyExpires"
class QBizManager : public QObject
{
    Q_OBJECT
public:
    QBizManager();
	~QBizManager();
   
	struct price_amount
	{
		QString price;
		QString amount;


	};

	struct postion
	{
		QString currentQty;
		QString unrealisedRoePcnt;
		QString lastPrice;
	};

	struct struct_tradeBin
	{
		QString high;
		QString low;
		QString close;
		QString volume;
		QString time;
	};


	struct struct_trade
	{
		QString price;
		QString size;
		QString side;
		QString timestamp;
	};

	
public:
	void doTransfer(const QString & source);
	void GetPostion(const QString & source);
	void GetVolume(const QString & source);
	void doTrade(const QString & source);

	int Side();
private:
	int Sell_Amount_Up();
	int Sell_Amount_Down();
	int Buy_Amount_Up();
	int Buy_Amount_Down();

private:
	bool Down_High_Check();
	bool Down_Check();
	bool Down_Check_5();
	bool Down_Check_volume_5();
	bool Down_Check_volume_1h();
	bool Down_Check_1h();
	bool Down_Check_1day();
	bool Down_Check_Red();
 	bool Down_Check_Red_5();
	int Down_Check_Red_1h();
	bool Down_Check_Green_Front();
	bool Down_High_Same();

	bool Down_Add();

	bool Up_Low_Check();
	bool Up_Check();
	bool Up_Check_5();
	bool Up_Check_1h();
	bool Up_Check_1day();
	bool Up_Check_volume_5();
	bool Up_Check_volume_1h();
	bool Up_Check_Green_5();
	bool Up_Check_Green_1h();
	bool Up_Check_Red();
	bool Up_Check_Red_Front();
	int Up(QString p);
	int Down(QString p);

signals:
    void walletInfoResult(QByteArray data);
    void orderCreateResult(QByteArray data);
    void allOrderQueryResult(QByteArray data);
    void orderCancelResult(QByteArray data);
    void orderBookResult(QByteArray data);
public slots:
    void connected();
    void closed();
    void textMessageReceived(const QString &message);
	void trade();
	void trade_ordre();
private:
    void queryWalletInfo(QString coinType = "XBt");
    void createOrder(QUrlQuery param);
	void closePosition(QUrlQuery param);
    void queryAllOrder(QUrlQuery param);
    void cancelOrder(QString orderId,QString clOrderId,QString comment);
	
	bool bitmex_depth(QString &,QString coinType = "XBT", QString depth = "2");
	bool bitmex_bucketed(QString &);
	bool bitmex_bucketed_5(QString &);
	bool bitmex_bucketed_1h(QString &);
	bool bitmex_bucketed_1day(QString &);

	bool bitmex_USD_Index(QString &);

	bool parse_bucketed(const QString & source, QStringList& trade_list);
	bool parse_USDT(const QString & source, QStringList& trade_list);
	

	int GetPrice(const QString & source, QStringList& buy_list, QStringList& sell_list);

	//void oneAmount();
	//void queryAllPosition(QUrlQuery param);
	void isolatePosition(QUrlQuery param);
	void leveragePosition(QUrlQuery param);
	void riskLimitPosition(QUrlQuery param);
	
private:
	bool m_signed;// = false;
	int m_currentTotalRequests;// = 0;
	int m_maxRequests;// = 60;
    QTimer m_maxRequestsTimer;
	QString m_websocketHost;// = "wss://testnet.bitmex.com/realtime";

    QWebSocket m_webSocket;
    //QNetworkAccessManager m_manager;

	QList<price_amount> price_amount_list_buy;

	QList<price_amount> price_amount_list_sell;

	QList<price_amount> price_amount_list_temp;

	QString m_price_buy;
	QString m_price_sell;

	double m_price_amount_buy;
	double m_price_amount_sell;
	postion my_postion;

	QString m_apiId = "pw45KAY8_wF3iIMKipm_b2Et";


	QDateTime my_now;// = QDateTime::currentDateTime();
	QTimer m_pingTimer;
	int numm = 0;


	QTimer m_TradeTimer_order;
	QTimer m_TradeTimer;
	struct_tradeBin m_tradeBin1m;
	struct_tradeBin my_trade;
	QStringList  trade_list;
	QStringList  m_trade_list_5;
	QStringList  m_trade_list_1h;
	QStringList  m_trade_list_1day;
	QList<struct_trade>  m_trade;
	QDateTime dfaf;

	int oneord;
	int oneordfdsf;

	QString text;

	QString m_price;

	int nummt;

	int nufasft;
	QString m_pricedfafd;
	int nummafads;

	int sideret;;
};

#endif // BITMEXWEBSOCKETCLIENT_H
