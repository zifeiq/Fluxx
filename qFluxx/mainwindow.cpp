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

    connect(this,SIGNAL(gameStart()),this,SLOT(initGame()));

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
        error();
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
                    error();
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
            error();
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
                    error();
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
        error();
    if(server && playerName.size() == PLAYER_NUM)
        emit roomFull();
    processMsg();
}

void MainWindow::initGame(){
    //waiting for modification
    waiting->hide();
    this->show();
    if(msgBox.getMsg(GAME_START,tcards)){
        //profiles & cardnum
        for(int i = 0; i != PLAYER_NUM; i++){
            //hands number
            handsNum[i] = tcards.size();
            QLabel* tnum = new QLabel("Hands Number:"+QString::number(handsNum[i]));
            cardNum.push_back(tnum);
            QAvatar* ticon = new QAvatar(i,playerName[i],this);
            icons.push_back(ticon);
            QLabel* temp = new QLabel(playerName[i],this);
            names.push_back(temp);
            QVBoxLayout* tprofile = new QVBoxLayout(this);
            tprofile->addItem(temp);
            tprofile->addItem(ticon);
            if(i != myNo)
                tprofile->addItem(tnum);
            lProfiles.push_back(tprofile);
        }
        lProfiles[myNo]->addItem(confirm);

        //my hands
        myHands = new QGraphicsScene(this);
        connect(myHands,SIGNAL(changed(QList<QRectF>)),this,SLOT(rearrange()));
        for(int i = 0; i != tcards.size(); i++){
            QCard* temp = new QCard(tcards[i],HAND);
            myHands->addItem(temp);
            temp->setPos(85*i,0);
        }
        vHands = new QGraphicsView(myHands);

        //keepers
        keepers = new QGraphicsScene(this);
        connect(keepers,SIGNAL(changed(QList<QRectF>)),this,SLOT(reKeepers(QList<RectF>)));
        for(int i = 0; i != PLAYER_NUM; i++){
//            QGraphicsScene* tscene = new QGraphicsScene(this);
//            connect(tscene,SIGNAL(changed(QList<QRectF>)),this,SLOT(reKeepers()));
//            keepers.push_back(tscene);
            QGraphicsView* tview = new QGraphicsView(this);
            tview->setScene(tscene);
            tview->setSceneRect(0,200*i,140,140);
            vKeepers.push_back(tview);
            rKeepers->push_back(QRectF(0,200*i,140,140));
        }

        //layouts
        //my area
        myArea = new QHBoxLayout(this);
        myArea->addItem(vKeepers[myNo]);
        myArea->addItem(vHands);
        myArea->addLayout(lProfiles[myNo]);

        //competitors
        for(int i = 1; (myNo+i)%PLAYER_NUM != myNo; i++){
            QHBoxLayout* tarea = new QHBoxLayout(this);
            tarea->addLayout(lProfiles[(i+myNo)%PLAYER_NUM]);
            tarea->addItem(vKeepers[(i+myNo)%PLAYER_NUM]);
            competitors->addLayout(tarea);
        }

        //table
        table = new QHBoxLayout(this);
        presentDspl = new QLabel(str_dspl.arg(" "));
        rules = new QGraphicsScene;
        connect(rules,SIGNAL(changed(QList<QRectF>)),this,SLOT(reRules()));
        QCard* trule = new QCard(CardLib::getLib().getCard(0),qcardType::RULE);
        rules->addItem(trule);
//        trule->setPos(0,0);
        truel = new QCard(CardLib::getLib().getCard(1),qcardType::RULE);
        rules->addItem(trule);
//        trule->setPos(80,0);
        vRules = new QGraphicsView(rules);
        goals = new QGraphicsScene;
        connect(goals,SIGNAL(changed(QList<QRectF>)),this,SLOT(reGoal()));
        vGoals = new QGraphicsView(goals);
        table->addItem(presentDspl);
        table->addSpacing(100);

        layout = new QVBoxLayout(this);
        layout->addLayout(competitors);
        layout->addLayout(table);
        layout->addLayout(myArea);
    }
    else
        error();
    processMsg();
}

void MainWindow::roundBegin(){
    if(msgBox.getMsg(ROUND_BEGIN,tno,tinfo)){
        bool random = tinfo?true:false;
        if(tno == myNo){
            presentDspl->setText(str_dspl.arg("YOU"));
            if(msgBox.getMsg(CARD_UPDATE,tcards,tinfo)){
                if(random){
                    msgBox.createMsg(PLAY_I,tcards[0]);
                    for(int i = 1; i != tcards.size(); i++){
                        myHands->addItem(new QCard(tcards[i],qcardType::HAND));
                    }
                }
                else{
                    for(int i = 0; i != tcards.size(); i++)
                        myHands->addItem(new QCard(tcards[i]),qcardType::HAND);
                }
            }
            else
                error();
        }
        else{
            presentDspl->setText(str_dspl.arg(playerName[tno]));
            if(msgBox.getMsg(CARD_NUM,tno,tinfo)){
                handsNum[tno] = tinfo;
                cardNum[tno]->setText(QString::number(tinfo));//GUI
            }
            else
                error();
        }
    }
    else
        error();
    processMsg();
}

void MainWindow::cardPlayed(){
    if(msgBox.getMsg(CARD_PLAYED,tno,tcards)){
        handsNum[tno]--;
        if(tno != myNo)
            cardNum[tno]->setText(QString::number(handsNum[tno]));
        else{
            QList<QGraphicsItem*> hands = myHands->items();//GUI
            for(int i = 0; i != hands.size(); i++){
                if(hands[i]->data(0) == toNo(tcards[0])){
                    myHands->removeItem(hands[i]);
                    delete hands[i];
                }
            }
        }
        presentCard.setPixmap(QPixmap(QString::number(toNo(tcards[0]))));
        QTimer::singleShot(2000,this,SLOT(clearPresent()));
    }
    else
        error();
    processMsg();
}

// response to command
void MainWindow::playCard(){
    enable(myHands,1);
    connect(confirm,SIGNAL(clicked()),this,SLOT(sendChoice(PLAY_I)));
}

void MainWindow::dropCard(){
    if(!msgBox.getMsg(DROP_CARD_C,tinfo))
        error();
    enable(myHands,tinfo);
    connect(confirm,SIGNAL(clicked()),this,SLOT(sendChoice(DROP_CARD_I)));
}

void MainWindow::dropKeeper(){
    if(!msgBox.getMsg(DROP_KEEPER_C,tinfo))
        error();
    enable(keepers[myNo],tinfo);
    connect(confirm,SIGNAL(clicked()),this,SLOT(sendChoice(DROP_KEEPER_I)));
}

void MainWindow::dropRule(){
    if(!msgBox.getMsg(DROP_RULE_C,tinfo))
        error();
    enable(rules,tinfo);
    connect(confirm,SIGNAL(clicked()),this,SLOT(sendChoice(DROP_RULE_I)));
}

void MainWindow::choosePlayer(){
    enable();
    connect(confirm,SIGNAL(clicked()),this,SLOT(sendChoice(CHOOSE_PLAYER_I)));
}

void MainWindow::chooseKeeper(){
    enable(false);
    connect(confirm,SIGNAL(clicked()),this,SLOT(sendChoice(CHOOSE_KEEPER_I)));
}

void MainWindow::exchangeKeeper(){
    enable(true);
    connect(confirm,SIGNAL(clicked()),this,SLOT(sendChoice(EXCHANGE_KEEPER_I)));
}

void MainWindow::chooseGoal(){
    enable(goals,1);
    connect(confirm,SIGNAL(clicked()),this,SLOT(sendChoice(CHOOSE_GOAL_I)));
}
// END OF MODIFICATION

//updates
void MainWindow::cardnumUpdate(){
    if(msgBox.getMsg(CARD_NUM,tno,tinfo)){
        handsNum[tno] = tinfo;
        cardNum[tno]->setText(QString::number(tinfo));
    }
    else
        error();
    processMsg();
}

void MainWindow::ruleUpdate(){
    QList<QGraphicsItem> items(rules->items());
    for(int i = 0; i != items.size(); i++){
        rules->removeItem(items[i]);
        delete items[i];
    }
    items = goals->items();
    for(int i = 0; i != items.size(); i++){
        goals->removeItem(items[i]);
        delete items[i];
    }
    if(msgBox.getMsg(RULE,tcards)){
        for(int i = 0; i != tcards.size(); i++){
            switch(tcards[i]->_type){
            case Card::BASIC_RULE:
            case Card::NEW_RULE:
                rules->addItem(new QCard(tcards[i],qcardType::RULE));
                break;
            case Card::GOAL:                      
                goals->addItem(new QCard(tcards[i],qcardType::GOAL));
                break;
                //更新当前目标
            default:    break;
            }
        }
    }
    else
        error();
    processMsg();
}

void MainWindow::keeperUpdate(){
    if(msgBox.getMsg(KEEPER_UPDATE,tno,tcards,tinfo)){
        //update keepers
        //keepers usually deleted with notification
        QList<QGraphicsItem*> items = keepers->items(QRectF(0,200*tno,140,140));
        switch(tinfo){
        case 0:
            for(int i = 0; i != items.size(); i++){
                keepers->removeItem(items[i]);
                delete items[i];
            }
            for(int i = 0; i != tcards.size(); i++){
                QCard* temp = new QCard(tcards[i],qcardType::KEEPER);
                temp->setPixmap("s"+QString::number(temp->data(0).toInt()));
                keepers->addItem(temp);
//                temp->setPos(140*i/4,35*i%4);
            }
            ;//notifications
            break;
        case 1:
            for(int i = 0; i != tcards.size(); i++){
                QCard* temp = new QCard(tcards[i],qcardType::KEEPER);
                temp->setPixmap("s"+QString::number(temp->data(0).toInt()));
                keepers->addItem(temp);
//                temp->setPos(140*(items.size()+i)/4,35*(items.size())+i);
            }
            break;
        case 2:
            for(int i = 0; i != tcards.size(); i++){
                for(int j = 0; j != items.size(); i++)
                    if(toNo(tcards[i]) == items[j]->data(0).toInt()){
                        keepers->removeItem(items[j]);
                        delete items[j];
                    }
            }
            ;//notifications
            break;
        default:    break;
        }
    }
    else
        error();
    processMsg();
}

void MainWindow::cardUpdate(){
    if(msgBox.getMsg(CARD_UPDATE,tcards,tinfo)){
        switch(tinfo){
        case 0:
            QList<QGraphicsItem> items(myHands->items());
            for(int i = 0; i != items.size(); i++){
                myHands->removeItem(items[i]);
                delete items[i];
            }
            for(int i = 0; i != tcards.size(); i++){
                myHands->addItem(new QCard(tcards[i],HAND));
            }
            break;
        case 1:
            for(int i = 0; i != tcards.size(); i++){
                //
                myHands->addItem(new QCard(tcards[i],HAND));
            }
            break;
        case 2:
            ;//find and delete
            ;//usually with notification
            QList<QGraphicsItem> items(myHands->items());
            for(int i = 0; i != tcards.size(); i++){
                for(int j = 0; j != items.size(); j++){
                    if(items[j].data(0) == toNo(tcards[i])){
                        myHands->removeItem(items[j]);
                        delete items[j];
                    }
                }
            }
            break;
        }
    }
    else
        error();
    processMsg();
}

void MainWindow::cardDropped(){
    if(msgBox.getMsg(CARD_DROPED,tno,tcards)){
        if(tno == myNo){
            QList<QGraphicsItem> items(myHands->items());
            for(int i = 0; i != tcards.size(); i++){
                for(int j = 0; j != items.size(); j++){
                    if(items[j].data(0) == toNo(tcards[i])){
                        myHands->removeItem(items[j]);
                        delete items[j];
                    }
                }
                QLabel* temp = new QLabel(QPixmap(QString::number(toNo(tcards[i]))));
                temp->setGeometry(400-40*tcards.size()+80*i,240,80,120);
                droppedCards.push_back(temp);
            }
            QTimer::singleShot(2000,this,SLOT(clearPresent()));
        }
        else{
            //display
            for(int i = 0; i != tcards.size(); i++){
                QLabel* temp = new QLabel(QPixmap(QString::number(toNo(tcards[i]))));
                temp->setGeometry(400-40*tcards.size()+80*i,240,80,120);
                droppedCards.push_back(temp);
            }
            QTimer::singleShot(2000,this,SLOT(clearPresent()));
            handsNum[tno] -= tcards.size();
            cardNum[tno]->setText(QString::number(handsNum[tno]));
        }
    }
    else
        error();
    processMsg();
}

//void MainWindow::cardStoled(){

//}

void MainWindow::gameOver(){
    if(msgBox.getMsg(GAME_OVER,tno)){
        if(tno == myNo){
            QMessageBox::information(NULL, "Game Over", "Congratulations! You win!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        }
        else{
            QMessageBox::information(NULL, "Game Over", "Player "+playerName[tno]+" wins!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        }

    }
    else
        error();
}

void MainWindow::enable(int no){
    for(int i = 0; i != icons.size(); i++){
        if(i != myNo){
          icons[i]->setSelectable(true);
          connect(icons[i],SIGNAL(clicked()),this,SLOT(chooseConstraint(no)));
        }
    }
}

void MainWindow::enable(QGraphicsScene *cards, int i){
    QList<QGraphicsItem*> items(cards->items());
    for(int i = 0; i != items.size(); i++){
        items[i]->setFlag(QGraphicsItem::ItemIsSelectable);
    }
    connect(cards,SIGNAL(selectionChanged()),this,SLOT(chooseConstraint(i)));
}

void MainWindow::enable(bool exchange)
{
    if(true){
        QList<QGraphicsItem*> items = keepers->items();
        for(int i = 0; i != items.size(); i++){
            items[i]->setFlag(QGraphicsItem::ItemIsSelectable);
            connect(items[i],SIGNAL(clicked()),this,SLOT(chooseConstraint(exchange)));
        }
    }
    else{
        for(int i = 0; i != PLAYER_NUM; i++){
            if(i == myNo)
                continue;
            QList<QGraphicsItem*> items = keepers->items(rKeepers[i]);
            for(int j = 0; j != items.size(); i++){
                connect(items[j],SIGNAL(clicked()),this,SLOT(chooseConstraint(exchange)));
                items[j]->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
    }
}

// the following are slots function
void MainWindow::sendChoice(MsgType type){
    switch(type){
    case CHOOSE_PLAYER_I:
        for(int i = 0; i != icons.size(); i++)
            if(icons[i]->isSelected()){
                tno = i;
                break;
            }
        if(!msgBox.createMsg(CHOOSE_PLAYER_I,to))
            error();
        break;
    case PLAY_I:
    case DROP_CARD_I:
        QList<QGraphicsItem*> sel(myHands->selectedItems());
        for(int i = 0; i != sel.size(); i++)
            tcards.push_back(toCard(sel[i]));
        if(!msgBox.createMsg(type,tcards))
            error;
        break;
    case DROP_KEEPER_I:
    case CHOOSE_KEEPER_I:
        QList<QGraphicsItem*> sel(keepers->selectedItems());
        for(int i = 0; i != sel.size(); i++)
            tcards.push_back(toCard(sel[i]));
        if(!msgBox.createMsg(type,tcards))
            error;
        break;
    case DROP_RULE_I:
        QList<QGraphicsItem*> sel(rules->selectedItems());
        for(int i = 0; i != sel.size(); i++)
            tcards.push_back(toCard(sel[i]));
        if(!msgBox.createMsg(type,tcards))
            error;
        break;
    case CHOOSE_GOAL_I:
        QList<QGraphicsItem*> sel(goals->selectedItems());
        for(int i = 0; i != sel.size(); i++)
            tcards.push_back(toCard(sel[i]));
        if(!msgBox.createMsg(type,tcards))
            error;
        break;
    default:    break;
    }
    disconnect(confirm,SIGNAL(clicked()),this,SLOT(sendChoice(MsgType)));
    confirm->setEnabled(false);
    processMsg();
}

void MainWindow::reHands()
{
    QGraphicsScene* theScene = dynamic_cast<QGraphicsScene*>(sender());
    QList<QGraphicsItem*> items = theScene->items();
    if(items.size()>6){
        for(int i = 0; i != items.size(); i++)
            items[i]->setPos(425.0/(items.size()-1)*i,0);
    }
    else
        for(int i = 0; i != items.size(); i++)
            items[i]->setPos(85*i,0);
}

void MainWindow::reKeepers(QList<QRectF> rect)
{
    QGraphicsScene* theScene = dynamic_cast<QGraphicsScene*>(sender());
    int no = rect[0].bottom()/200;
    QList<QGraphicsItem*> items = theScene->items(0,no*200,140,140);
    for(int i = 0; i != items.size()/4; i++)
        for(int j = 0; j != 4 && (4*i+j) != items.size(); j++)
            items[i]->setPos(140*i,35*j+no*200);
}

void MainWindow::reRules()
{
    QGraphicsScene* theScene = dynamic_cast<QGraphicsScene*>(sender());
    QList<QGraphicsItem*> items = theScene->items();
    for(int i = 0; i != items.size(); i++)
        items[i]->setPos(80*i,0);
}

void MainWindow::reGoal()
{
    QGraphicsScene* theScene = dynamic_cast<QGraphicsScene*>(sender());
    QList<QGraphicsItem*> items = theScene->items();
    for(int i = 0; i != items.size(); i++)
        items[i]->setPos(80*i,0);
}

void MainWindow::clearPresent()
{
    presentCard.clear();
    for(int i = 0; i != droppedCards.size(); i++){
        delete droppedCards[i];
    }
}

void MainWindow::chooseConstraint(int no)
{
    if(dynamic_cast<QAvatar*>(sender())){
        int counter = 0;
        for(int i = 0; i != icons.size(); i++)
            if(icons[i]->isSelected())
                counter++;
        if(counter == no){
            for(int i = 0; i != icons.size(); i++)
                if(!icons[i]->isSelected())
                    icons[i]->setSelectable(false);
        }
        else{
            for(int i = 0; i != icons.size(); i++)
                if(!icons[i]->isSelectable())
                    icons[i]->setSelectable(true);
        }
    }
    else if(QGraphicsScene* scene = dynamic_cast<QGraphicsScene*>(sender())){
        QList<QGraphicsItem*> items(scene->items());
        if(items[0]->data(1) == qcardType::KEEPER)
            items = scene->items(rkeepers[myNo]);
        if(scene->selectedItems().size() == no){
            for(int i = 0; i != items.size(); i++)
                if(!items[i]->isSelected())
                    items[i]->setFlag(QGraphicsItem::ItemIsSelectable,false);
        }
        else{
            for(int i = 0; i != items.size(); i++)
                items[i]->setFlag(QGraphicsItem::ItemIsSelectable);
        }
    }
}

void MainWindow::chooseConstraint(bool exchange)
{
    if(exchange){
        QList<QGraphicsItem*> sel = keepers->selectedItems();
        switch(sel.size()){
        case 0:     enable(true);break;
        case 1:
            confirm->setEnabled(false);
            if(rKeepers[myNo].contains(sel[0]->boundingRect())){
                QList<QGraphicsItem*> items(keepers->items(rKeepers[myNo]));
                for(int i = 0; i != items.size(); i++)
                    if(!items[i]->isSelected())
                        items[i]->setFlag(QGraphicsItem::ItemIsSelectable,false);
            }
            else{
                QList<QGraphicsItem*> items(keepers->items());
                for(int i = 0; i != items.size(); i++)
                    if(!items[i]->isSelected() && !rKeepers[myNo].contains(items[i]->boundingRect()))
                        items[i]->setFlag(QGraphicsItem::ItemIsSelectable,false);
            }
            break;
        case 2:
          confirm->setEnabled(true);
          QList<QGraphicsItem*> items(keepers->items());
          for(int i = 0; i != items.size(); i++)
              if(!items[i]->isSelected())
                  items[i]->setFlag(QGraphicsItem::ItemIsSelectable,false);
          break;
        }
    }
}

void MainWindow::error(){
    QMessageBox::warning(NULL, "Oops~", "Something wrong! ", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    close();
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
//                error();
//            }
//        }
//        else
//            error();
//        if(msgBox.getMsg(tmsg)){

//        }
//        else
//            error();
//    }
//    else
//        error();

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
//                error();
//            }
//        }
//        else
//            error();
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
//                  error();
//              }
//          }
//          else
//              error();
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
//        error();
//}

//void MainWindow::awaitStart(){
//    if(msgBox.getMsg(tmsg,tcards)){
//        if(tmsg == GAME_START)
//            emit gameStart();//game start
//        else
//            error();
//    }
//    else
//        error();

//}


