#ifndef QCLIENT_H
#define QCLIENT_H

#include <QObject>
#include <QtNetwork>
#include <QString>
#include <QList>
#include <vector>
#include <string>
#include <iostream>
#include "./cardLib_code/cardLib.h"
#include "ClientMB.h"
#include "config.h"

class QClient : public QObject, public ClientMB
{
    Q_OBJECT
public:
    explicit QClient(QObject *parent=0);
    ~QClient();
    void connectServer(QString server_ip);
    QTcpSocket *tcpSocket;

signals:
    void msgReady();

private slots:
    void dsplError(QAbstractSocket::SocketError);

private:
    QList<QString> msg;
    std::string recvMsg();
    void sendMsg(std::string s);
};


#endif // QCLIENT_H
