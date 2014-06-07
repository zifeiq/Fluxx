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

#define PLAYER_NUM 4

enum STATE{
    WAITING,
    PLAYING
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

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
    Register* reg;
    Mode* mode;

    QPushButton* start;

    QLabel playerName[4];
    int myNo;
    QString myName;
    QList<QLabel*> cardNum;
    QList<QLabel*> myCards;
    QList<QLabel*> myKeepers;
    QList<QLabel*> Keepers_1;
    QList<QLabel*> Keepers_2;
    QList<QLabel*> Keepers_3;
    QLabel* Action;
    QList<QLabel*> Rules;

    QVBoxLayout* waiting;
    QVBoxLayout* playing;
    QHBoxLayout* myArea;


    void regist();
    void awaitingStart();
    int state;
    bool host;

//    ClientMB* MailBox;

//    MailBox->receiveMsg(palyerName);

};

#endif // MAINWINDOW_H
