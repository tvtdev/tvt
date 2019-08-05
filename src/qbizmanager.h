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
#define FUNCTION_QUERY_ORDER "/order"
#define FUNCTION_CANCEL_ORDER "/order"
#define FUNCTION_ORDER_BOOK "/orderBook/L2"

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

public:
	void doTransfer();

private:
	int Sell_Amount_Up();
	int Sell_Amount_Down();
	int Buy_Amount_Up();
	int Buy_Amount_Down();

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
    void queryWalletInfo(QString coinType = "XBt");
    void createOrder(QUrlQuery param);
    void queryAllOrder(QUrlQuery param);
    void cancelOrder(QString orderId,QString clOrderId,QString comment);
	
	bool bitmex_depth(QString &,QString coinType = "XBT", QString depth = "2");


	int GetPrice(const QString & source, QStringList& buy_list, QStringList& sell_list);



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
	//QString price_1;
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
};

#endif // BITMEXWEBSOCKETCLIENT_H
