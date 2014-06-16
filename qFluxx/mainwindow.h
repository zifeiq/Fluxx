#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QString>
#include <QList>
#include <QLabel>
#include <QLayout>
#include <QGridLayout>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTextEdit>
#include <string>
#include <vector>
using std::string;
using std::vector;

#include "./cardLib_code/cardLib.h"
//#include "./mailbox/clientMB/clientMB.h"

#include "register.h"
#include "mode.h"
#include "waiting.h"
#include "qcard.h"
#include "qavatar.h"
#include "./mailbox/clientMB/qclient.h"
#include "aithread.h"
#include "serverthread.h"

#include "config.h"

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Register* reg;      
    Mode* chooseMode;   
    Waiting* waiting;   

    QGraphicsScene* myHands;
    QGraphicsView* vHands;
    QGraphicsScene* keepers;
    QList<QRectF> rKeepers;
    QList<QGraphicsView*> vKeepers;   
    QList<QAvatar*> icons;
    QList<QLabel*> cardNum;    
    QGraphicsScene* rules;
    QGraphicsScene* goals;
    QGraphicsView* vRules;
    QGraphicsView* vGoals;
    QLabel* presentDspl;
    QLabel* presentCard;
    QTextEdit* log;
    QList<QLabel*> droppedCards;

    QPushButton* confirm;

    QVBoxLayout* layout;

    QHBoxLayout* myArea;
    QHBoxLayout* competitors;
    QHBoxLayout* table;
    QList<QHBoxLayout*> oneArea;

    QList<QLabel*> names;
    QList<QVBoxLayout*> lProfiles;

signals:
    void serverConnected();
    void roomFull();
    void gameStart();

private slots:

	void processMsg();

    void reg_ok_clicked();
    void reg_cancel_clicked();
    void mode_host_clicked();
    void mode_part_clicked();

    void createAI();

    void initGame();
    void sendChoice();

//    void reKeepers(QList<QRectF> rect);
//    void reRules();
//    void reGoal();


    void clearPresent();
    void chooseConstraint();
//    void chooseConstraint(bool exchange);

    void addPlayer();
    void roundBegin();
    void cardPlayed();
    void cardDropped();

    void gameOver();

    void cardnumUpdate();
    void ruleUpdate();
    void keeperUpdate();
    void cardUpdate();

	void dropCard();
    void dropKeeper();
    void dropRule();

private:

//	QTcpSocket *tcpSocket; 
    QClient* msgBox;

    int handsNum[CLIENT_MAX];
    QList<QString> playerName;
    QString myName;
    int myNo;
    int presentPlayer;

    bool randomStart;
    MsgType tmsg;
    int tno;
    int tinfo;
    string tname;
    vector<const Card*> tcards;
    static const QString str_dspl;
    static const QString str_num;

    QList<AIthread*> ai;
    ServerThread* server;
    
    QString s2q(const string& s) { return QString(QString::fromLocal8Bit(s.c_str())); }
    string q2s(const QString& s) { return string((const char *)s.toLocal8Bit()); }
    
    int toNo(const Card* card) { return (int)card->getType()*100+card->getNum();}
    const Card* toCard(QGraphicsItem* item);

    //void processMsg();

    void playCard();

    void choosePlayer();
    void chooseKeeper();
    void exchangeKeeper();
    void chooseGoal();    
    void enable(QGraphicsScene* cards, int i);
    void enable(int no = 1);
    void enable(bool exchange);
    void disable();
    void reHands();    
    void error();
};

#endif 
