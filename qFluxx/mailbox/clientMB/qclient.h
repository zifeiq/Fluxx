#ifndef QCLIENT_H
#define QCLIENT_H

#include <QObject>
#include <QtNetwork>
#include <QString>
#include <QList>
#include <vector>
#include <string>
#include <iostream>
#include "../../cardLib_code/cardLib.h"
#include "ClientMB.h"
#include "../../config.h"

class QClient : public QObject, public ClientMB
{
    Q_OBJECT
public:
    explicit QClient(QObject *parent=0);
    ~QClient();
    bool connectServer(QString server_ip);
    QTcpSocket *tcpSocket;

signals:
    void msgReady();

private slots:
  //  void dsplError(QAbstractSocket::SocketError);

protected:
	virtual std::string recvMsg();
    virtual bool sendMsg(std::string s);

private:
    QList<QString> msg;

};


#endif // QCLIENT_H
