#pragma once

#include <QObject>
#include <QSslSocket>
#include <QRegExp>
class QImap : public QObject
{
	Q_OBJECT
		enum CMD
	{
		CMD_CAPALIBILY = 10000,
		CMD_LOGIN,
		CMD_LOGOUT,
		CMD_LIST,
		CMD_STATUS,
		CMD_SELECT,
		CMD_SEARCH,		
		CMD_EXAMINE,
		CMD_FETCH,
		CMD_UIDSEARCH,
	};
signals:
	void signalCapalibily(QString content);
	void signalLogin(QString content);
	void signalLogout();
	void signalList(QString content);
	void signalStatus(QString content);
	void signalSelect(QString content);
	void signalSearch(QString content);
	void signalExamine(QString content);
	void signalFetch(QString content);
	void signalUidSearch(QString content);
	void signalConnected(QString content);
	void signalUnkown(QString content);
public:
	QImap(QObject *parent);
	~QImap();
	void connectToServer(QString host, int port);
	void capalibily();
	void login(QString userName,QString password);
	void queryEmail();
	void logout();
	void status(QString box);
	void select(QString box);
	void search(QString box);
	void examine(QString box);
	void fetch(QString sequence, QString dataItemName);
	void queryUidByIndex(int index);

	QString Getselect()
	{
		return m_newid
			;
	}
	QString Getmail(){
		return m_mail;
	}

	void setemail(int type)
	{
		m_type = type;

	}
private slots:
	void readReady();
private:
	void sendCmd(QByteArray msg);
	QSslSocket m_socket;

	QString m_newid;
	QString m_mail;
	int m_type;

};
