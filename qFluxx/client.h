#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <vector>
#include <string>

#include "./cardLib_code/cardLib.h"
#include "mailbox/clientMB/clientMB.h"
#include "config.h"

using std::vector;
using std::string;

class Client
{
public:
    Client(QString name);
    void addPlayer(int no, QString name);

    bool connect(QString ip);
    bool join();
    ClientMB msgBox;

private:
    vector<const Card*> myHands;
    vector<vector<const Card*>> keepers;
    int handsNum[PLAYER_NUM];

    QString playerName[PLAYER_NUM];
    QString myName;
    int myNo;

    //用于接收消息的临时变量
    MsgType tmsg;
    int relatedPlayer;
    string tname;
    int relatedInfo;
    vector<const Card*> tcards;

    //QString与string间转换
    QString s2q(const string& s) { return QString(QString::fromLocal8Bit(s.c_str())); }
    string q2s(const QString& s) { return string((const char *)s.toLocal8Bit()); }
};

#endif // CLIENT_H
