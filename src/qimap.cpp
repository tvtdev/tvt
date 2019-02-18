#include "qimap.h"

QImap::QImap(QObject *parent)
	: QObject(parent)
{
	connect(&m_socket, &QSslSocket::readyRead, this, &QImap::readReady, Qt::UniqueConnection);
}

QImap::~QImap()
{
}

void QImap::connectToServer(QString host, int port)
{
	m_socket.connectToHostEncrypted(host, port);
}

void QImap::capalibily()
{
	QString _cmd = "##%1## CAPABILITY\r\n";
	sendCmd(_cmd.arg(CMD::CMD_CAPALIBILY).toUtf8());
}

void QImap::login(QString userName, QString password)
{
	QString _cmd = "##%1## LOGIN %2 %3 \r\n";
	sendCmd(_cmd.arg(CMD::CMD_LOGIN).arg(userName).arg(password).toUtf8());
}

void QImap::queryEmail()
{
	QString _cmd = "##%1## LIST \"\" *\r\n";
	sendCmd(_cmd.arg(CMD::CMD_LIST).toUtf8());
}

void QImap::logout()
{
	QString _cmd = "##%1## LOGOUT\r\n";
	sendCmd(_cmd.arg(CMD::CMD_LOGOUT).toUtf8());
}

void QImap::status(QString box)
{
	QString _cmd = "##%1## STATUS %2 (MESSAGES UNSEEN RECENT)\r\n";
	sendCmd(_cmd.arg(CMD::CMD_STATUS).arg(box.toUpper()).toUtf8());
}

void QImap::select(QString box)
{
	QString _cmd = "##%1## SELECT %2\r\n";
	sendCmd(_cmd.arg(CMD::CMD_SELECT).arg(box.toUpper()).toUtf8());
}

void QImap::search(QString box)
{
	QString _cmd = "##%1## SEARCH %2\r\n";
	sendCmd(_cmd.arg(CMD::CMD_SEARCH).arg(box.toUpper()).toUtf8());
}

void QImap::examine(QString box)
{
	QString _cmd = "##%1## EXAMINE %2\r\n";
	sendCmd(_cmd.arg(CMD::CMD_EXAMINE).arg(box.toUpper()).toUtf8());
}

void QImap::fetch(QString sequence, QString dataItemName)
{
	QString _cmd = "##%1## FETCH %2 %3\r\n";
	sendCmd(_cmd.arg(CMD::CMD_FETCH).arg(sequence).arg(dataItemName).toUtf8());
}

void QImap::queryUidByIndex(int index)
{
	QString _cmd = "##%1## UID SEARCH %2\r\n";
	sendCmd(_cmd.arg(CMD::CMD_UIDSEARCH).arg(index).toUtf8());
}

void QImap::readReady()
{
	QString _data = m_socket.readAll();
	qDebug() << _data;

	if (m_type == 11)
	{
		m_mail.clear();
		m_mail.append(_data);
	}

	QRegExp _regexp(R"(##(\d+)##)");
	if (_regexp.indexIn(_data) > -1)
	{
		
		QStringList _capturedTexts = _regexp.capturedTexts();
		if (_capturedTexts.count() == 2)
		{
			CMD _cmd = (CMD)_regexp.capturedTexts()[1].toInt();
			qDebug() << _cmd;
			switch (_cmd)
			{
			case QImap::CMD_CAPALIBILY:
				emit signalCapalibily(_data);
				break;
			case QImap::CMD_LOGIN:
				emit signalLogin(_data);
				break;
			case QImap::CMD_LOGOUT:
				emit signalLogout();
				break;
			case QImap::CMD_LIST:
				emit signalList(_data);
				break;
			case QImap::CMD_STATUS:
				emit signalStatus(_data);
				break;
			case QImap::CMD_SELECT:
				emit signalSelect(_data);
				m_newid = _data;
				break;
			case QImap::CMD_SEARCH:
				emit signalSearch(_data);
				break;
			case QImap::CMD_EXAMINE:
				emit signalExamine(_data);
				break;
			case QImap::CMD_FETCH:
				emit signalFetch(_data);
				break;
			case QImap::CMD_UIDSEARCH:
				emit signalUidSearch(_data);
				break;
			default:
				qDebug() << "Unhandle cmd result:"<<_cmd;
				break;
			}
		}
		else
		{
			if (_data.indexOf("OK"))
			{
				emit signalConnected(_data);
			}
			else
			{
				emit signalUnkown(_data);
			}
		}
	}
}

void QImap::sendCmd(QByteArray msg)
{
	qDebug() << "Send cmd:"<<msg;
	m_socket.write(msg);
}
