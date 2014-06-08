#include "mainwindow.h"
#include <iostream>
#include <QInputDialog>
#include <QMessageBox>

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

    server = NULL;

    connect(this,SIGNAL(gameStart()),this,SLOT(initGame())
}

MainWindow::~MainWindow()
{
}

void MainWindow::reg_ok_clicked()
{
    myName = reg->name->text();
//    player = new Client(myName);
    reg->hide();
    chooseMode->show();
}

void MainWindow::reg_cancel_clicked()
{
    close();
}

void MainWindow::mode_host_clicked()
{
    //    connectServer("127.0.0.1");
    server = new Server;
    //听说这里应该建立子进程
    msgBox.connectServer(q2s("127.0.0.1"));
    waiting = new Waiting(true);
    waiting->show();
//    awaitOthers();
    if(!msgBox.createMsg(REGISTER,q2s(myName)))
        ;//error handling
//    emit serverConnected();
    processMsg();
}

void MainWindow::mode_part_clicked()
{
    while(1){
        bool ok = false;
        //请输入IP
        QString ip = QInputDialog::getText(this,"Fluxx","Please enter the IP address of your host",QLineEdit::Normal,QString::null,&ok);
        //按下OK且IP不为空
        if(ok && !ip.isEmpty()){
            if(!msgBox.connectServer(q2s(ip)))//连接失败
                QMessageBox::warning(NULL, "Oops~", "Cannot connect to the server, please try another IP address.", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            else  {                 //连接成功
                chooseMode->hide();
                waiting = new Waiting(false);
                waiting->show();
//                awaitOthers();
                if(!msgBox.createMsg(REGISTER,q2s(myName)))
                    ;//error handling
                processMsg();
                return;
        }
        else
            break;
        }
    }
}

void MainWindow::processMsg(){
        if(!msgBox.getMsg(tmsg))
            ;//error handling
        switch(tmsg){
        case ADD_PLAYER:    addPlayer();

        }
}

void MainWindow::addPlayer(){
    if(msgBox.getMsg(ADD_PLAYER,tno,tname)){
        if(tname == q2s(myName)){
            myNo = tno;
            for(int i = 0; i != myNo; i++){
                if(msgBox.getMsg(ADD_PLAYER,tno,tname)){
                    playerName.push_back(s2q(tname));
                    waiting->addPlayer(s2q(tname));
                }
                else
                    ;//error handling
            }
            playerName[myNo] = s2q(myName);
            waiting->addPlayer(myName);
        }
        else{
            player[tno] = s2q(tname);
            waiting->addPlayer(s2q(tname));
        }
    }
    else
        ;//error handling
    if(!server && playerName.size() == PLAYER_NUM)
        emit roomFull();
}

//void MainWindow::awaitOthers()
//{
//    connect(this,SIGNAL(roomFull()),waiting,SLOT(enableStart()));
//    //receiving information of myself
//    if(msgBox.createMsg(REGISTER,q2s(myName))){
//        if(msgBox.getMsg(tmsg,myNo,tname)){
//            if(tmsg == ADD_PLAYER)
//                playerName[myNo] = s2q(tname);
//            else{
//                msgBox.createMsg(NACK);
//                ;//error handling
//            }
//        }
//        else
//            ;//error handling
//        if(msgBox.getMsg(tmsg)){

//        }
//        else
//            ;//error handling
//    }
//    else
//        ;//error handling

//    //receiving information of other players entered before
//    //and adding the namelist to GUI
//    for(int i = 0; i != myNo; i++){
//        if(msgBox.getMsg(tmsg,relatedPlayer,tname)){
//            if(tmsg == ADD_PLAYER){
//                waiting->addPlayer(tname);
//                playerName[relatedPlayer] = s2q(tname);
//            }
//            else{
//                msgBox.createMsg(NACK);
//                ;//error handling
//            }
//        }
//        else
//            ;//error handling
//    }
//    waiting->addPlayer(myName);

//    //waiting for the room to be full
//    if(myNo == PLAYER_NUM-1)
//        emit roomFull();
//    else
//      for(int i = 0; myNo+i != PLAYER_NUM-1; i++){
//          if(msgBox.getMsg(ADD_PLAYER,relatedPlayer,tname)){
//              if(tmsg == ADD_PLAYER){
//                  ing->addPlayer(tname);
//                  playerName[relatedPlayer] = s2q(tname);
//              }
//              else{
//                  msgBox.createMsg(NACK);
//                  ;//error handling
//              }
//          }
//          else
//              ;//error handling
//      }
//      emit roomFull();
//      if(!server)
//          awaitStart();
//}

void MainWindow::start_clicked()
{
    if(msgBox.createMsg(ACK))
        awaitStart();
    else
        ;//error handling
}

void MainWindow::awaitStart(){
    if(msgBox.getMsg(tmsg,tcards)){
        if(tmsg == GAME_START)
            emit gameStart();//game start
        else
            ;//error handling
    }
    else
        ;//error handling

}

void MainWindow::initGame(){

}
