#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QList>
#include <QLabel>
#include <QLayout>
#include <QGridLayout>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVariant>
#include <QMetaType>

#include <string>
#include <vector>
using std::string;
using std::vector;

#include "./cardLib_code/cardLib.h"
#include "./mailbox/clientMB/clientMB.h"
//#include "mailBox.h"

#include "register.h"
#include "mode.h"
#include "waiting.h"
#include "qcard.h"
#include "qavatar.h"
//#include "client.h"

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
    //my hands
    QGraphicsScene* myHands;
//    QList<QCard*> hands;
    QGraphicsView* vHands;

    //keepers
    QGraphicsScene* keepers;
//    QList<QGraphicsItemGroup*> gKeepers;
    QList<const QRectF>* rKeepers;
    QList<QGraphicsView*> vKeepers;

    //对手区域,icons & card no.
    QList<QAvatar*> icons;
    QList<QLabel*> cardNum;

    //rules&goal
    QGraphicsScene* rules;
    QGraphicsScene* goals;
    QGraphicsView* vRules;
    QGraphicsView* vGoals;
    QLabel* presentDspl;
    QLabel presentCard;
    QList<QLabel*> droppedCards;

    QPushButton* confirm;

    QVBoxLayout* layout;

    QHBoxLayout* myArea;
    QHBoxLayout* competitors;
    QHBoxLayout* table;

    QList<QHBoxLayout*> oneArea;
//    QList<QGridLayout*> lKeepers;

    QList<QLabel*> names;
    QList<QVBoxLayout*> lProfiles;

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

    void reHands();
    void reKeepers(QList<QRectF> rect);
    void reRules();
    void reGoal();

    void clearPresent();    //clear the present card
    void chooseConstraint(int no);
    void chooseConstraint(bool exchange);

private:
    Server* server;
//    Client* player;

    ClientMB msgBox;
    int handsNum[PLAYER_NUM];
    QList<QString> playerName;
    QString myName;
    int myNo;
    int presentPlayer;

    //用于接收消息的临时变量
    MsgType tmsg;
    int tno;
    int tinfo;
    string tname;
    vector<const Card*> tcards;

    const QString str_dspl = "The present player is:\n\t%1";

    //QString与string间转换
    QString s2q(const string& s) { return QString(QString::fromLocal8Bit(s.c_str())); }
    string q2s(const QString& s) { return string((const char *)s.toLocal8Bit()); }
    //Card* & no.
    int toNo(const Card* card) { return (int)card->getType()*100+card->getNum();}

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

    //enalble cards
    void enable(QGraphicsScene* cards, int i);
//    void enable(bool keeper_ex);
    void enable(int no = 1);
    void enable(bool exchange);
    //
    void error();
};

#endif // MAINWINDOW_H
