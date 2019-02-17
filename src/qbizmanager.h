#ifndef QBIZMANAGER_H
#define QBIZMANAGER_H
#include <QObject>
#include "qhttpmanager.h"
#include <QTimer>
#include <QMap>
#include <QHash>
#include "qimap.h"


class QBizManager : public QObject
{
 Q_OBJECT
public:
    QBizManager(QObject *parent);

     ~QBizManager();
public:

     static   QBizManager&							GetInstance(){static QBizManager cbm;return cbm;}
     bool initDb();
  

	 void appendCookie(const QString& topic);

	
	
	 
	 void sendmx();
	 void testmail();


	 bool create(const QString& topic, const QString& subject);
	 bool SendCoin(QString typ);

	 QString GetEmailCode();
	
	 
        void do_cf_clearance();
        void Get_cf_clearance(QString coo, QString & res);
 
public:


	QStringList m_StringList;

	QStringList m_cookieList;
	int m_cookienum;

	int m_startnonce;
	int m_signature_r;
	int m_gas_add;

	int m_curr_gwei_int;

	QString m_gas;
	QImap * _QImap;

};



#endif // QBIZMANAGER_H
