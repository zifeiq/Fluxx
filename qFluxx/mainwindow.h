#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QList>
#include <QLabel>
#include <QLayout>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>

#include <string>
#include <vector>
using std::string;
using std::vector;

//#include "mailBox.h"
#include "register.h"
#include "mode.h"
#include "waiting.h"
//#include "client.h"
#include "./cardLib_code/cardLib.h"
#include "./mailbox/clientMB/clientMB.h"
#include "config.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Register* reg;      //注册用户名界面
    Mode* chooseMode;   //选择建立主机/加入游戏
    Waiting* waiting;   //等待游戏开始

    //playing
    QGraphicsScene sKeepers;
    QGraphicsScene sHands;
    QGraphicsView vHands;
    QList<QGraphicsPixmapItem*> myHands;
    QList<QList<QGraphicsPixmapItem*>> keepers;
    QGraphicsPixmapItem* icons[PLAYER_NUM];

    QPushButton* confirm;


signals:
    void serverConnected();
    void roomFull();
    void gameStart();

private slots:
    void reg_ok_clicked();
    void reg_cancel_clicked();
    void mode_host_clicked();
    void mode_part_clicked();

    void start_clicked();
    void initGame();

    void sendChoice(MsgType type);

private:
    Server* server;
//    Client* player;

    ClientMB msgBox;
    int handsNum[PLAYER_NUM];
    QList<QString> playerName;
    QString myName;
    int myNo;

    //用于接收消息的临时变量
    MsgType tmsg;
    int tno;
    int tinfo;
    string tname;
    vector<const Card*> tcards;

    //QString与string间转换
    QString s2q(const string& s) { return QString(QString::fromLocal8Bit(s.c_str())); }
    string q2s(const QString& s) { return string((const char *)s.toLocal8Bit()); }

//    void awaitOthers();            //waiting for other players
//    void awaitStart();      //waiting for gamestart msg
//    void startGame();

//    void enableHands();//unfinished

    void processMsg();//等待接收消息的函数,游戏结束前每个动作处理完均要回到该函数
    void addPlayer();
    void roundBegin();
    void cardPlayed();
    void cardDropped();
//    void cardStoled();
    void gameOver();

    //responce command
    void playCard();
    void dropCard();
    void dropKeeper();
    void dropRule();
    void choosePlayer();
    void chooseKeeper();
    void exchangeKeeper();
    void chooseGoal();

    //updates
    void cardnumUpdate();
    void ruleUpdate();
    void keeperUpdate();
    void cardUpdate();

    //
};

#endif // MAINWINDOW_H
