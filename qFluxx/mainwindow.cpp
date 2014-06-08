#include "mainwindow.h"
#include <iostream>
#include <QInputDialog>
#include <QMessageBox>
#include <QPixmap>

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
        case ADD_PLAYER:    addPlayer();break;
        case GAME_START:    initGame();break;
        case ROUND_BEGIN:   roundBegin();break;


        //commands MODIFICATION
        case PLAY_C:        playCard();break;
        case DROP_CARD_C:   dropCard();break;
        case DROP_KEEPER_C: dropKeeper();break;
        case DROP_RULE_C:   dropRule();break;
        case CHOOSE_PLAYER_C:choosePlayer();break;
        case CHOOSE_KEEPER_C:chooseKeeper();break;
        case EXCHANGE_KEEPER_C:exchangeKeeper();break;
        case CHOOSE_GOAL_C: chooseGoal();break;

        //updates
        case CARD_NUM:      cardnumUpdate();break;
        case RULE:          ruleUpdate();break;
        case KEEPER_UPDATE: keeperUpdate();break;
        case CARD_UPDATE:   cardUpdate();break;

        //information
        case CARD_PLAYED:   cardPlayed();break;
        case CARD_DROPED:   cardDropped();break;
//        case CARD_STOLEN:   cardStoled();break;
        case GAME_OVER:     gameOver();break;

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
    if(server && playerName.size() == PLAYER_NUM)
        emit roomFull();
    processMsg();
}

void MainWindow::initGame(){
    //waiting for modification
    waiting->hide();
    this->show();
    if(msgBox.getMsg(GAME_START,tcards)){
        for(int i = 0; i != PLAYER_NUM; i++)
            handsNum[i] = tcards.size();
        sHands.setSceneRect(0,480,650,120);
        vHands.setScene(sHands);
        for(int i = 0; i != tcards.size(); i++){
            QGraphicsPixmapItem* temp = new QGraphicsPixmapItem(QPixmap(s2q(tcards[i]->getAddr())));
            myHands.push_back(temp);
            sHands.addItem(temp);
            temp->setPos(15+95*i,0);
        }

    }
    else
        ;//error handling
    processMsg();
}

void MainWindow::roundBegin(){
    if(msgBox.getMsg(ROUND_BEGIN,tno,tinfo)){
        bool random = tinfo?true:false;
        if(tno == myNo){
            if(msgBox.getMsg(CARD_UPDATE,tcards,tinfo)){
                if(random){
                    msgBox.createMsg(PLAY_I,tcards[0]);
                    for(int i = 1; i != tcards.size(); i++)
                        myHands.push_back(tcards[i]);
                    ;//GUI
                }
                else{
                    for(int i = 0; i != tcards.size(); i++)
                        myHands.push_back(tcards[i]);
                    ;//GUI
                }
            }
            else
                ;//error handling
        }
        else{
            if(msgBox.getMsg(CARD_NUM,tno,tinfo)){
                handsNum[tno] = tinfo;
                ;//GUI
            }
            else
                ;//error handling
        }
    }
    else
        ;//error handling
    processMsg();
}

void MainWindow::cardPlayed(){
    if(msgBox.getMsg(CARD_PLAYED,tno,tcards)){
        handsNum[tno]--;
        ;//GUI
    }
    else
        ;//error handling
    processMsg();
}
//waiting for modification
void MainWindow::playCard(){
    //enableHands();
    connect(confirm,SIGNAL(clicked()),this,SLOT(sendChoice(PLAY_I)));
}

void MainWindow::dropCard(){
    //enableHands();
    connect(confirm,SIGNAL(clicked()),this,SLOT(sendChoice(DROP_CARD_I)));
}

void MainWindow::dropKeeper(){
    //enableKeeper();
    connect(confirm,SIGNAL(clicked()),this,SLOT(sendChoice(DROP_KEEPER_I)));
}

void MainWindow::dropRule(){
    //enableRules();
    connect(confirm,SIGNAL(clicked()),this,SLOT(sendChoice(DROP_RULE_I)));
}

void MainWindow::choosePlayer(){
    //enablePlayers();
    connect(confirm,SIGNAL(clicked()),this,SLOT(sendChoice(CHOOSE_PLAYER_I)));
}

void MainWindow::chooseKeeper(){
    //enableKeepers();
    connect(confirm,SIGNAL(clicked()),this,SLOT(sendChoice(CHOOSE_KEEPER_I)));
}

void MainWindow::exchangeKeeper(){
}

void MainWindow::chooseGoal(){

}
// END OF MODIFICATION

//updates
void MainWindow::cardnumUpdate(){
    if(msgBox.getMsg(CARD_NUM,tno,tinfo)){
        handsNum[tno] = tinfo;
        //GUI
    }
    else
        ;//error handling
    processMsg();
}

void MainWindow::ruleUpdate(){
    if(msgBox.getMsg(RULE,tcards)){
        for(int i = 0; i != tcards.size(); i++){
            switch(tcards[i]->_type){
            case Card::BASIC_RULE:
            case Card::NEW_RULE:
                //更新当前规则
            case Card::GOAL:
                //更新当前目标
            }
        }
    }
    else
        ;//error handling
    processMsg();
}

void MainWindow::keeperUpdate(){
    if(msgBox.getMsg(KEEPER_UPDATE,tno,tcards,tinfo)){
        //update keepers
        //keepers usually deleted with notification
    }
    else
        ;//error handling
    processMsg();
}

void MainWindow::cardUpdate(){
    if(msgBox.getMsg(CARD_UPDATE,tcards,tinfo)){
        switch(tinfo){
        case 0:
            myHands.clear();
            for(int i = 0; i != tcards.size(); i++){
                myHands.push_back(tcards[i]);
                //adding hands
            }
            break;
        case 1:
            for(int i = 0; i != tcards.size(); i++){
                //
                myHands.push_back(tcards[i]);
            }
            break;
        case 2:
            ;//find and delete
            ;//usually with notification
            break;
        }
    }
    else
        ;//error handling
    processMsg();
}

void MainWindow::cardDropped(){
    if(msgBox.getMsg(CARD_DROPED,tno,tcards)){
        if(tno == myNo){
            ;//find and delete my hands
            ;//GUI
        }
        else{
            handsNum[tno] -= tcards.size();
            ;//GUI
        }
    }
    else
        ;//error handling
    processMsg();
}

//void MainWindow::cardStoled(){

//}

void MainWindow::gameOver(){
    if(msgBox.getMsg(GAME_OVER,tno)){
        ;//game over handling
    }
    else
        ;//error handling
}

// the following are slots function
void MainWindow::sendChoice(MsgType type){
    if(type == CHOOSE_PLAYER_I){
        int temp;
        ;//get the no you choose
        msgBox.createMsg(CHOOSE_PLAYER_I,temp);
    }
    else{
        vector<const Card*> temp;
        ;//get cards you select
        msgBox.createMsg(type,temp);
    }

    ;//get and send information about hand to be played
    disconnect(confirm,SIGNAL(clicked()),this,SLOT(sendChoice(MsgType)));
    processMsg();
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

//void MainWindow::start_clicked()
//{
//    if(msgBox.createMsg(ACK))
//        awaitStart();
//    else
//        ;//error handling
//}

//void MainWindow::awaitStart(){
//    if(msgBox.getMsg(tmsg,tcards)){
//        if(tmsg == GAME_START)
//            emit gameStart();//game start
//        else
//            ;//error handling
//    }
//    else
//        ;//error handling

//}


