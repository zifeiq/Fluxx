#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QList>
#include <QLabel>
#include <QLayout>

//#include "mailBox.h"
#include "register.h"
#include "mode.h"
#include "game.h"
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

signals:
    void gameStart();
//protected:
//    void mousePressEvent(QMouseEvent *event);

private slots:
    void reg_ok_clicked();
    void reg_cancel_clicked();
    void mode_host_clicked();
    void mode_part_clicked();
    void start_clicked();
    void starting();

private:
    Game* game;
    QString myName;
//    QPushButton* start;

//    QLabel playerName[4];
//    int myNo;
//    QString myName;
//    QList<QLabel*> cardNum;
//    QList<QLabel*> myCards;
//    QList<QLabel*> myKeepers;
//    QList<QLabel*> Keepers_1;
//    QList<QLabel*> Keepers_2;
//    QList<QLabel*> Keepers_3;
//    QLabel* Action;
//    QList<QLabel*> Rules;

//    QVBoxLayout* waiting;
//    QVBoxLayout* playing;
//    QHBoxLayout* myArea;


//    void regist();
//    void awaitingStart();
//    int state;
//    bool host;

//    ClientMB* MailBox;

//    MailBox->receiveMsg(palyerName);

};

#endif // MAINWINDOW_H
