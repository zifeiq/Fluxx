#include "mainwindow.h"
#include <iostream>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
//    MailBox = new ClientMB;
    reg = new Register(this);
    connect(reg->ok,SIGNAL(clicked()),this,SLOT(reg_ok_clicked()));
    connect(reg->cancel,SIGNAL(clicked()),this,SLOT(reg_cancel_clicked()));
    reg->show();

    chooseMode = new Mode(this);
    connect(chooseMode->host,SIGNAL(clicked()),this,SLOT(mode_host_clicked()));
    connect(chooseMode->part,SIGNAL(clicked()),this,SLOT(mode_part_clicked()));

//    waiting = new QVBoxLayout(this);
//    waiting->addWidget(playerName[0]);
//    waiting->addWidget(playerName[1]);
//    waiting->addWidget(playerName[2]);
//    waiting->addWidget(playerName[3]);
//    centralWidget()->setLayout(waiting);
 //   connect(this,SIGNAL(gameStart()),this,SLOT(starting()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::reg_ok_clicked()
{
    myName = reg->name->text();
    reg->hide();
    chooseMode->show();
}

void MainWindow::reg_cancel_clicked()
{
    close();
}

void MainWindow::mode_host_clicked()
{
    //create a host
    //send request to host
    //establishing connection
    //    connectServer("127.0.0.1");
    game = new Game(true,myName,"127.0.0.1")
    start = new QPushButton("START");
    start->setEnabled(false);
    connect(start,SIGNAL(clicked()),this,SLOT(start_clicked()));
    regist();

}

void MainWindow::mode_part_clicked()
{
    bool ok = false;
    QString ip = QInputDialog::getText(this,"Fluxx","Please enter the IP address of your host",QLineEdit::Normal,QString::null,&ok);
    if(ok && !ip.isEmpty()){
        // checking the format
        // establishing connection
//        if(connect)
//            register();
//        else
        // error occurs
        game = new Game(false,myName,ip);
    }
    else
    {

    }
}

void MainWindow::regist()
{
    //sendMsg(myName);
    //QString t_name;
    //int t_no;
//    if(receiveMsg(t_no,t_name))
//    {
//        if(t_name == myName)
//            myNo = t_no;
//            playerName[myNo].setText(myName);
//    }
//    if(myNo != 0)  //不是主机
//    for(int i = 0; i < myNo; i++){
//        if(receiveMsg(t_no,t_name))
//          playerName[t_no].setText(t_name);
    //
//    }
    state = WAITING;
    this->show();
    // wating for other players
    for(int i = 1; myNo +i != 4; i++ ){
        //receiveMsg(t_no,t_name);
        //playerName[playerName+i].setText(t_name);
    }
    if(host){
        start->setEnabled(true);
        return;
    }
    //receive start msg
    awaitingStart();
}

void MainWindow::start_clicked(){
    //send start msg
    awaitingStart();
}

void MainWindow::awaitingStart(){
    //if(receive start msg)
        emit gameStart();
}

void MainWindow::starting(){
//    waiting->removeWidget(playerName[0]);
//    waiting->removeWidget(playerName[1]);
//    waiting->removeWidget(playerName[2]);
//    waiting->removeWidget(playerName[3]);
    for(int i = 0; i != PLAYER_NUM; i++){
        //排列玩家信息,包括初始手牌数
    }
    //get my cards

}
