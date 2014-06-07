#include "client.h"

Client::Client(QString name, QString ip)
{
    myName = name;
    msgBox.connectServer(q2s(ip));
    msgBox.createMsg(REGISTER,q2s(name));
    myNo = no;
    playerName[myNo] = myName;
}

void Client::addPlayer(QString name, QString ip)
{
    player[no] = name;
}
