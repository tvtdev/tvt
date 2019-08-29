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
 Q_OBJECT
public:
    QBizManager( );

     ~QBizManager();
public:
     bool initDb();
  

	 bool bitcointalkPosttest_seqnum(const QString& topic, const QString& subject, const QString& sc, const QString& num_replies, const QString& seqnum, const QString& board);
	 void doPost();
	 void appendCookie(const QString& topic);
	 bool CheckTime();
	 bool Checkboard();

	 QString bttname;

private:
    QSet<QString> m_s;
    int tnum;



    QString accessKey;//= "abf17cf7-c12b-4075-9652-3590f4c9b6c5";
    QString secretKey;//= "fdfbc556-3f5d-477c-b8d1-9c4248930878";


    QString apiKey;// = "3644892";
    QString secret_key;//= "654EEF232C3A0C168B0219B919FFBE8F";

    QString m_amount;
    QString m_qujian;

public:
    bool  bok;
    bool  bhuobi;

	QStringList m_StringList;

	QStringList m_cookieList;
	int m_cookienum;


};



#endif // QBIZMANAGER_H
