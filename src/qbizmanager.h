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
    //enum RequestType {
    //    REQUEST_WALLET,
    //    REQUEST_CREATE_ORDER,
    //    REQUEST_QUERY_ORDER,
    //    REQUEST_CANCEL_ORDER,
    //    REQUEST_ORDER_BOOK,
    //};
    //Q_ENUM(RequestType)

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

	struct struct_trade
	{
		QString high;
		QString low;
		QString close;
		QString volume;
		QString time;
	};
	
public:
	void doTransfer(const QString & source);
	void GetPostion(const QString & source);
	void GetVolume(const QString & source);

private:
	int Sell_Amount_Up();
	int Sell_Amount_Down();
	int Buy_Amount_Up();
	int Buy_Amount_Down();

private:
	bool Down_High_Check();
	bool Down_Check();
	bool Down_Check_Red();
	bool Down_Check_Green_Front();

	bool Up_Low_Check();
	bool Up_Check();
	bool Up_Check_Red();
	int Up_Fan(QString p);
	int Down_Fan(QString p);

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

	bool parse_bucketed(const QString & source, QStringList& trade_list);

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

	QString m_apiId = "dUQ0H4eqzj-2GpLEa7iZIvlv";

	//QString m_apiId = "a-eJ9WVKgS7eaJ19qox7KW3W";
	//QString amount_1;

	//QString price_2;
	//QString amount_2;

	//QString price_3;
	//QString amount_3;

	//QString price_4;
	//QString amount_4;

	//QString price_5;
	//QString amount_5;

	//QString price_6;
	//QString amount_6;

	//QString price_7;
	//QString amount_7;

	//QString price_8;
	//QString amount_8;

	//QString price_9;
	//QString amount_9;

	//QString price_10;
	//QString amount_10;

	QDateTime my_now;// = QDateTime::currentDateTime();
	QTimer m_pingTimer;
	int numm = 0;


	QTimer m_TradeTimer_order;
	QTimer m_TradeTimer;
	struct_trade m_trade;
	struct_trade my_trade;
	QStringList  trade_list;

	QDateTime dfaf;

	int oneord;
	int oneordfdsf;

	
};

#endif // BITMEXWEBSOCKETCLIENT_H
