#include "client.h"

Client::Client(QString name)
{
    myName = name;
}

void Client::addPlayer(QString name, QString ip)
{
    player[no] = name;
}

bool Client::connect(QString ip)
{
    if(msgBox.connectServer(q2s(ip)))
        return true;
    else
        return false;
}

bool Client::join()
{
    if(msgBox.createMsg(REGISTER,q2s(myName))){
        if(msgBox.getMsg(tmsg,myNo,myName))
            if(tmsg == NACK)
                ;//error handling
            else if(tmsg != ADD_PLAYER)
                msgBox.createMsg(NACK);
            else
                return true;
    }
    else
        ;//error handling
    return false;
}
