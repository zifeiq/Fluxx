#include "mainwindow.h"
#include <iostream>
#include <QInputDialog>
#include <QMessageBox>
#include <QPixmap>

const QString MainWindow::str_dspl = "The present player is:\n\t";
const QString MainWindow::str_num = "Hands Number: ";

MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent)
{
//    str_dspl = "The present player is:\n\t";
//        str_num
    server = NULL;

    msgBox = new QClient;
    reg = new Register(this);
    connect(reg->ok,SIGNAL(clicked()),this,SLOT(reg_ok_clicked()));
    connect(reg->cancel,SIGNAL(clicked()),this,SLOT(reg_cancel_clicked()));
    reg->show();

    chooseMode = new Mode(this);
    connect(chooseMode->host,SIGNAL(clicked()),this,SLOT(mode_host_clicked()));
    connect(chooseMode->part,SIGNAL(clicked()),this,SLOT(mode_part_clicked()));
    connect(this,SIGNAL(gameStart()),this,SLOT(initGame()));

//	tcpSocket = new QTcpSocket(this);
    connect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(processMsg()));

    presentCard = new QLabel(this);
	presentCard->setFixedSize(CARD_W,CARD_L);
 //   setFixedSize(1024,768);
	setFixedSize(WINDOW_W,WINDOW_L);
    log = new QTextEdit;
    log->setReadOnly(true);
	log->setGeometry(0,0,200,120);
}

MainWindow::~MainWindow()
{
//    delete reg;
//	if(chooseMode)
//    delete chooseMode;
//	if(waiting)
//    delete waiting;
//    delete myHands;
//    delete vHands;
//    delete keepers;
//    for(int i = 0; i != vKeepers.size(); i++)
//        delete vKeepers[i];
//    for(int i = 0; i != icons.size(); i++)
//        delete icons[i];
//    for(int i = 0; i != cardNum.size(); i++)
//        delete cardNum[i];
//    delete rules;
//    delete goals;
//    delete vRules;
//    delete vGoals;
//    delete presentDspl;
//    delete presentCard;
//    for(int i = 0; i != droppedCards.size(); i++)
//        delete droppedCards[i];
//    delete confirm;
//    delete layout;
////    delete myArea;
////    delete competitors;
////    delete table;
//    for(int i = 0; i != oneArea.size(); i++)
//        delete oneArea[i];
//    for(int i = 0; i != names.size(); i++)
//        delete names[i];
//    for(int i = 0; i != lProfiles.size(); i++)
//        delete lProfiles[i];
//    delete msgBox;
//    for(int i = 0; i != tcards.size(); i++)
//        delete tcards[i];

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
    server = new ServerThread;
    server->start();
    msgBox->connectServer("127.0.0.1");
	//tcpSocket->abort();
	//tcpSocket->connectToHost("127.0.0.1",PORT);	
    waiting = new Waiting(true);
    connect(waiting->ai_s,SIGNAL(clicked()),this,SLOT(createAI()));
    connect(waiting->ai_h,SIGNAL(clicked()),this,SLOT(createAI()));
	chooseMode->hide();
    waiting->show();
    connect(this,SIGNAL(roomFull()),waiting,SLOT(enableStart()));

    if(!msgBox->createMsg(REGISTER,q2s(myName)))
        error();

    //processMsg();
}

void MainWindow::mode_part_clicked()
{
      bool ok = false;
      QString ip = QInputDialog::getText(this,"Fluxx","Please enter the IP address of your host",QLineEdit::Normal,QString::null,&ok);
        
      if(ok && !ip.isEmpty()){
//           if(!msgBox->connectServer(ip))
/*                QMessageBox::warning(NULL, "Oops~", "Cannot connect to the server, please try another IP address.", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            else  {       */   
				//tcpSocket->abort(); //
				//tcpSocket->connectToHost(ip,PORT); 
          while(1){
              if(!msgBox->connectServer(ip)){
                  QMessageBox::warning(NULL, "Oops~", "Cannot connect to the server, please try another IP address.", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
              }
              else break;
          }
      }
      else{
          return;
      }
      chooseMode->hide();
      waiting = new Waiting(false);
      waiting->show();
      waiting->resize(400,300);
      if(!msgBox->createMsg(REGISTER,q2s(myName)))
          error();
}

void MainWindow::createAI(){
    if(dynamic_cast<QPushButton*>(sender())->text() == SIMPLE_AI){
        AIthread* tai = new AIthread(true);
        ai.push_back(tai);
        tai->start();
    }
    else{
        AIthread* tai = new AIthread(false);
        ai.push_back(tai);
        tai->start();
    }
}


const Card* MainWindow::toCard(QGraphicsItem* item)
{
    for(int i = 0; i != CardLib::getLib().getCardNum(); i++){
        if(CardLib::getLib().getCard(i)->getType()*100+CardLib::getLib().getCard(i)->getNum() == item->data(0).toInt())
            return CardLib::getLib().getCard(i);
    }
	return CardLib::getLib().getCard(0);
}


void MainWindow::processMsg(){
		
        if(!msgBox->getMsg(tmsg))
            error();
        disconnect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(processMsg()));
        switch(tmsg){
        case ADD_PLAYER:   connect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(addPlayer()));break;
        case GAME_START:   connect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(initGame()));break;
        case ROUND_BEGIN:  connect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(roundBegin()));break;


        
        case PLAY_C:        playCard();break;
        case DROP_CARD_C:   connect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(dropCard()));break;
        case DROP_KEEPER_C: connect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(dropKeeper()));break;
        case DROP_RULE_C:   connect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(dropRule()));break;
        case CHOOSE_PLAYER_C:choosePlayer();break;
        case CHOOSE_KEEPER_C:chooseKeeper();break;
        case EXCHANGE_KEEPER_C:exchangeKeeper();break;
        case CHOOSE_GOAL_C: chooseGoal();break;

        
        case CARD_NUM:      connect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(cardnumUpdate()));break;
        case RULE:          connect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(ruleUpdate()));break;
        case KEEPER_UPDATE: connect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(keeperUpdate()));break;
        case CARD_UPDATE:   connect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(cardUpdate()));break;

        
        case CARD_PLAYED:   connect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(cardPlayed()));break;
        case CARD_DROPED:   connect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(cardDropped()));break;

        case GAME_OVER:     connect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(gameOver()));break;

        }
}

void MainWindow::addPlayer(){
    if(msgBox->getMsg(ADD_PLAYER,tno,tname)){
		playerName.push_back(s2q(tname));
        waiting->addPlayer(tno,s2q(tname));
        if(tname == q2s(myName))
            myNo = tno;
        //    for(int i = 0; i != myNo; i++){
        //        if(msgBox->getMsg(ADD_PLAYER,tno,tname)){
        //            playerName.push_back(s2q(tname));
        //            waiting->addPlayer(s2q(tname));
        //        }
        //        else
        //            error();
        //    }
        //    playerName[myNo] = myName;
        //    waiting->addPlayer(myName);
        //}
        //else{

        //    waiting->addPlayer(s2q(tname));
        //}
    }
    else
        error();
    if(/*server &&*/ playerName.size() == CLIENT_MAX)
        emit roomFull();
    connect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(processMsg()));
    disconnect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(addPlayer()));
//    processMsg();
}

void MainWindow::initGame(){
    
    waiting->hide();
    this->show();
    log->append("Game initializing...");
    if(msgBox->getMsg(GAME_START,tcards)){
        
        for(int i = 0; i != CLIENT_MAX; i++){
			handsNum[i] = tcards.size();
			QLabel* tnum = new QLabel(str_num+QString::number(handsNum[i]),this);
            if(i != myNo){
				tnum->show();
			}
            cardNum.push_back(tnum);
            QAvatar* ticon = new QAvatar(i,playerName[i],this);
            icons.push_back(ticon);
            ticon->show();
            QLabel* temp = new QLabel(QString::number(i)+" "+playerName[i],this);
            names.push_back(temp);
            temp->show();
            QVBoxLayout* tprofile = new QVBoxLayout;
            tprofile->setDirection(QBoxLayout::TopToBottom);
            tprofile->addWidget(temp);
            tprofile->addWidget(ticon);
            if(i != myNo)
                tprofile->addWidget(tnum);
            lProfiles.push_back(tprofile);
        }
        confirm = new QPushButton("confirm",this);
        lProfiles[myNo]->addWidget(confirm);

        
        myHands = new QGraphicsScene(this);
        for(int i = 0; i != tcards.size(); i++){
            QCard* temp = new QCard(tcards[i],QHAND);
            myHands->addItem(temp);
            temp->setPos((CARD_W+HAND_S)*i,0);
        }
        vHands = new QGraphicsView(myHands);
        vHands->setSceneRect(0,-10,HAND_MAX*(CARD_W+HAND_S),HAND_L);

        
        keepers = new QGraphicsScene(this);
        for(int i = 0; i != CLIENT_MAX; i++){
//            QGraphicsScene* tscene = new QGraphicsScene(this);
//            connect(tscene,SIGNAL(changed(QList<QRectF>)),this,SLOT(reKeepers()));

            QGraphicsView* tview = new QGraphicsView(this);
            tview->setScene(keepers);
            tview->setSceneRect(0,KEEPER_VL*i-20,KEEPER_VW,KEEPER_VL);
            vKeepers.push_back(tview);
            rKeepers.push_back(QRectF(0,KEEPER_VL*i,KEEPER_VW,KEEPER_VL));
        }
        
        myArea = new QHBoxLayout;
        myArea->addWidget(vKeepers[myNo]);
        myArea->addWidget(vHands);
        myArea->addLayout(lProfiles[myNo]);

        
        competitors = new QHBoxLayout;
        for(int i = 1; (myNo+i)%CLIENT_MAX != myNo; i++){
            QHBoxLayout* tarea = new QHBoxLayout;
            tarea->addLayout(lProfiles[(i+myNo)%CLIENT_MAX]);
            tarea->addWidget(vKeepers[(i+myNo)%CLIENT_MAX]);
            competitors->addLayout(tarea);
        }

        
        table = new QHBoxLayout;
        presentDspl = new QLabel(str_dspl+" ",this);
//        presentDspl->show();
        rules = new QGraphicsScene;

        QCard* trule = new QCard(CardLib::getLib().getCard(0),QRULE);
        rules->addItem(trule);

        trule = new QCard(CardLib::getLib().getCard(1),QRULE);
        rules->addItem(trule);
        trule->setPos(CARD_W,0);

        vRules = new QGraphicsView(rules);
        vRules->setSceneRect(0,0,RULE_MAX*CARD_W,CARD_L);
        goals = new QGraphicsScene;
        vGoals = new QGraphicsView(goals);
        vGoals->setSceneRect(0,0,CARD_W,CARD_L);


        table->addWidget(log);
//        table->addSpacing(100);
        table->addWidget(presentCard);
//        table->addSpacing(120);
        table->addWidget(vRules);
        table->addWidget(vGoals);

        layout = new QVBoxLayout(this);
        layout->addLayout(competitors);
//        layout->addStretch(200);
        layout->addLayout(table);
//        layout->addStretch(200);
        layout->addLayout(myArea);

        //setLayout(layout);
    }
    else
        error();
//    processMsg();
    disconnect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(initGame()));
    connect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(processMsg()));
}

void MainWindow::roundBegin(){
    connect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(processMsg()));
    if(msgBox->getMsg(ROUND_BEGIN,tno,tinfo)){
        randomStart = tinfo?true:false;
        if(tno == myNo){
            presentDspl->setText(str_dspl+"YOU");
            log->append("It's your round now!");
        }
        else{
            presentDspl->setText(str_dspl+QString::number(tno)+" "+playerName[tno]);
            log->append("It's player " + playerName[tno] + "'s round now.");
        }
    }
    else
        error();
//    processMsg();
    disconnect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(roundBegin()));

}

void MainWindow::cardPlayed(){
    connect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(processMsg()));
    if(msgBox->getMsg(CARD_PLAYED,tcards,tno)){
        handsNum[tno]--;
		QString type;
		switch(tcards[0]->getType()){
			case Card::NEW_RULE:	type = "NEW RULE";break;
			case Card::KEEPER:		type = "KEEPER";break;
			case Card::ACTION:		type = "ACTION CARD";break;
			case Card::GOAL:		type = "GOAL";break;
			default:break;
			}
        if(tno != myNo){
            cardNum[tno]->setText(str_num+QString::number(handsNum[tno]));
			log->append("Player "+playerName[tno]+" has played a "+ type+": " +s2q(tcards[0]->getName()));
        }
        else{
            log->append("You have played a "+ type+": " +s2q(tcards[0]->getName()));
            QList<QGraphicsItem*> hands = myHands->items();
            for(int i = 0; i != hands.size(); i++){
                if(hands[i]->data(0) == toNo(tcards[0])){
                    myHands->removeItem(hands[i]);
                    delete hands[i];
                    break;
                }
            }
			reHands();
        }
        presentCard->setPixmap(QPixmap(":/cards/"+s2q(tcards[0]->getAddr())).scaled(CARD_W,CARD_L));
//        presentCard->setGeometry(360,240,CARD_W,CARD_L);
//        presentCard->set
        presentCard->show();
        QTimer::singleShot(2000,this,SLOT(clearPresent()));
    }
    else
        error();
//    processMsg();
    disconnect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(cardPlayed()));

}


void MainWindow::playCard(){
    log->append("Pleas play a card!");
    enable(myHands,1);
    confirm->setEnabled(true);
    connect(confirm,SIGNAL(clicked()),this,SLOT(sendChoice()));
//    disconnect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(playCard()));
}

void MainWindow::dropCard(){
    if(!msgBox->getMsg(DROP_CARD_C,tinfo))
        error();
    log->append("Please drop "+QString::number(tinfo)+" cards.");
    enable(myHands,tinfo);
    confirm->setEnabled(true);
    connect(confirm,SIGNAL(clicked()),this,SLOT(sendChoice()));
    disconnect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(dropCard()));
}

void MainWindow::dropKeeper(){
    if(!msgBox->getMsg(DROP_KEEPER_C,tinfo))
        error();
	log->append("Please drop " + QString::number(tinfo)+" keepers.");
    enable(keepers,tinfo);
    confirm->setEnabled(true);
    connect(confirm,SIGNAL(clicked()),this,SLOT(sendChoice()));
    disconnect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(dropKeeper()));
}

void MainWindow::dropRule(){
    if(!msgBox->getMsg(DROP_RULE_C,tinfo))
        error();
	log->append("Please drop " + QString::number(tinfo)+" rules.");
    enable(rules,tinfo);
    confirm->setEnabled(true);
    connect(confirm,SIGNAL(clicked()),this,SLOT(sendChoice()));
    disconnect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(dropRule()));
}

void MainWindow::choosePlayer(){
    enable();
	log->append("Please choose a player.");
    confirm->setEnabled(true);
    connect(confirm,SIGNAL(clicked()),this,SLOT(sendChoice()));
//    disconnect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(choosePlayer()));
}

void MainWindow::chooseKeeper(){
	log->append("Please choose a keeper.");
    enable(false);
    confirm->setEnabled(true);
    connect(confirm,SIGNAL(clicked()),this,SLOT(sendChoice()));
//    disconnect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(chooseKeeper()));
}

void MainWindow::exchangeKeeper(){
	log->append("Please choose a keeper of others and a keeper of yours.");
    enable(true);
    confirm->setEnabled(true);
    connect(confirm,SIGNAL(clicked()),this,SLOT(sendChoice()));
//    disconnect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(exchangeKeeper()));
}

void MainWindow::chooseGoal(){
	log->append("Please choose a goal.");
    enable(goals,1);
    confirm->setEnabled(true);
    connect(confirm,SIGNAL(clicked()),this,SLOT(sendChoice()));
//    disconnect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(chooseGoal()));
}



void MainWindow::cardnumUpdate(){

    if(msgBox->getMsg(CARD_NUM,tno,tinfo)){
		if(tinfo > handsNum[tno])
			log->append("Player " + playerName[tno]+" has drawn " + QString::number(tinfo-handsNum[tno]) + " hands");
		else if(tinfo < handsNum[tno])
			log->append("Player " + playerName[tno]+" has lost " + QString::number(handsNum[tno]-tinfo) + " hands.");
		else
			log->append("Player " + playerName[tno]+" has " + QString::number(handsNum[tno]-tinfo) + " hands.");
        handsNum[tno] = tinfo;
        cardNum[tno]->setText(str_num+QString::number(tinfo));
    }
    else
        error();
    //processMsg();
    connect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(processMsg()));
    disconnect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(cardnumUpdate()));

}

void MainWindow::ruleUpdate(){
    connect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(processMsg()));
//    connect(rules,SIGNAL(changed(QList<QRectF>)),this,SLOT(reRules()));
//    connect(goals,SIGNAL(changed(QList<QRectF>)),this,SLOT(reGoal()));

    if(msgBox->getMsg(RULE,tcards)){
        QList<QGraphicsItem*> items(rules->items());
        for(int i = 0; i != items.size(); i++){
            rules->removeItem(items[i]);
            delete items[i];
        }
        items = goals->items();
        for(int i = 0; i != items.size(); i++){
            goals->removeItem(items[i]);
            delete items[i];
        }
		QString newrules;
        for(int i = 0; i != tcards.size(); i++){
            switch(tcards[i]->getType()){
            case Card::BASIC_RULE:
            case Card::NEW_RULE:{
                QCard* temp = new QCard(tcards[i],QRULE);
                rules->addItem(temp);
                temp->setPos(CARD_W*i,0);
                break;}
            case Card::GOAL:                      
                goals->addItem(new QCard(tcards[i],QGOAL));
                break;
                
            default:    break;
            }
			newrules = newrules + s2q(tcards[i]->getName()) + " ";
        }
		log->append("Now the rules are: "+newrules);
    }
    else
        error();
//    disconnect(goals,SIGNAL(changed(QList<QRectF>)),this,SLOT(reGoal()));
//    disconnect(rules,SIGNAL(changed(QList<QRectF>)),this,SLOT(reRules()));
    disconnect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(ruleUpdate()));

   // //processMsg();
}

void MainWindow::keeperUpdate(){
    connect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(processMsg()));
//    connect(keepers,SIGNAL(changed(QList<QRectF>)),this,SLOT(reKeepers(QList<QRectF>)));
    if(msgBox->getMsg(KEEPER_UPDATE,tcards,tno,tinfo)){
        QList<QGraphicsItem*> items = keepers->items(rKeepers[tno]);
        switch(tinfo){
        case 2:{
            for(int i = 0; i != items.size(); i++){
                keepers->removeItem(items[i]);
                delete items[i];
            }
			QString hiskeepers;
            for(int i = 0; i != tcards.size(); i++){
                QCard* temp = new QCard(tcards[i],QKEEPER);
                temp->setPixmap(QPixmap(":/cards/s"+QString::number(temp->data(0).toInt())).scaled(KEEPER_W,KEEPER_L));
                keepers->addItem(temp);
                temp->setPos(i/KNUM_ROW*KEEPER_W,KEEPER_VL*tno+KEEPER_L*(i%KNUM_ROW));
				hiskeepers = hiskeepers + s2q(tcards[i]->getName()) + " ";
            }
            log->append("Now player " + playerName[tno]+"'s keepers are: " + hiskeepers);
            break;}
        case 1:{
            for(int i = 0; i != tcards.size(); i++){
                QCard* temp = new QCard(tcards[i],QKEEPER);
                temp->setPixmap(QPixmap(":/cards/s"+QString::number(temp->data(0).toInt())).scaled(112,28));
                keepers->addItem(temp);
                temp->setPos(items.size()/KNUM_ROW*KEEPER_W,KEEPER_VL*tno+KEEPER_L*(items.size()%KNUM_ROW));
				log->append("Player " + playerName[tno]+" has gained a keeper: " + s2q(tcards[i]->getName()));
            }
            break;}
        case 0:{
            for(int i = 0; i != tcards.size(); i++){
                for(int j = 0; j != items.size(); i++)
                    if(toNo(tcards[i]) == items[j]->data(0).toInt()){
                        keepers->removeItem(items[j]);
                        delete items[j];
                    }
				log->append("Player " + playerName[tno]+" has lost a keeper: " + s2q(tcards[i]->getName()));
            }
            items = keepers->items(rKeepers[tno]);
            for(int i = 0; i != items.size(); i++)
                items[i]->setPos(i/KNUM_ROW*KEEPER_W,KEEPER_VL*tno+i%KNUM_ROW*KEEPER_L);
            ;
            break;}
        default:    break;
        }
    }
    else
        error();
//    disconnect(keepers,SIGNAL(changed(QList<QRectF>)),this,SLOT(reKeepers(QList<QRectF>)));
    disconnect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(keeperUpdate()));

    ////processMsg();
}

void MainWindow::cardUpdate(){
    connect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(processMsg()));
    if(msgBox->getMsg(CARD_UPDATE,tcards,tinfo)){
//        connect(myHands,SIGNAL(changed(QList<QRectF>)),this,SLOT(reHands()));
        switch(tinfo){
        case 2:{
            QList<QGraphicsItem*> items(myHands->items());
            for(int i = 0; i != items.size(); i++){
                myHands->removeItem(items[i]);
                delete items[i];
            }
			QString newhands;
            for(int i = 0; i != tcards.size(); i++){
                QCard* temp = new QCard(tcards[i],QHAND);
                myHands->addItem(temp);
				newhands = newhands + s2q(tcards[i]->getName()) + " ";
             }
			log->append("Now your hands are: " + newhands);
            reHands();
            break;}
        case 1:{
            if(randomStart){
                std::vector<const Card*> tt;
                tt.push_back(tcards[0]);
                msgBox->createMsg(PLAY_I,tt);
            }
            for(int i = 0; i != tcards.size(); i++){
                QCard* temp = new QCard(tcards[i],QHAND);
                myHands->addItem(temp);
				log->append("You have drawn a hand: " + s2q(tcards[i]->getName()));
            }
            reHands();
            break;}
        case 0:{
            ;
            ;
            QList<QGraphicsItem*> items(myHands->items());
            for(int i = 0; i != tcards.size(); i++){
                for(int j = 0; j != items.size(); j++){
                    if(items[j]->data(0) == toNo(tcards[i])){
                        myHands->removeItem(items[j]);
                        delete items[j];
                    }
                }
				log->append("You have lost a hand: " + s2q(tcards[i]->getName()));
            }
            reHands();
            break;}
        }
    }
    else
        error();
//    disconnect(myHands,SIGNAL(changed(QList<QRectF>)),this,SLOT(reHands()));
    disconnect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(cardUpdate()));

    //processMsg();
}

void MainWindow::cardDropped(){
    connect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(processMsg()));
    if(msgBox->getMsg(CARD_DROPED,tcards,tno)){
        if(tno == myNo){
			QString dropped;
            QList<QGraphicsItem*> items(myHands->items());
            for(int i = 0; i != tcards.size(); i++){
                for(int j = 0; j != items.size(); j++){
                    if(items[j]->data(0) == toNo(tcards[i])){
                        myHands->removeItem(items[j]);
                        delete items[j];
                    }
                }
				dropped = dropped + s2q(tcards[i]->getName());
                QLabel* temp = new QLabel;
                temp->setPixmap(QPixmap(QString::number(toNo(tcards[i]))));
                temp->setGeometry(400-40*tcards.size()+80*i,240,80,120);
                droppedCards.push_back(temp);
            }
			log->append("You have dropped these cards: " + dropped);
            QTimer::singleShot(2000,this,SLOT(clearPresent()));
        }
        else{           
			QString dropped;
            for(int i = 0; i != tcards.size(); i++){
                QLabel* temp = new QLabel;
                temp->setPixmap(QPixmap(QString::number(toNo(tcards[i]))));
                temp->setGeometry(400-40*tcards.size()+80*i,240,80,120);
                droppedCards.push_back(temp);
				dropped = dropped + s2q(tcards[i]->getName());
            }
			log->append("Player " + playerName[tno] +" have dropped these cards: " + dropped);
            QTimer::singleShot(2000,this,SLOT(clearPresent()));
            handsNum[tno] -= tcards.size();
            cardNum[tno]->setText(str_num+QString::number(handsNum[tno]));
        }
    }
    else
        error();
    ////processMsg();
    disconnect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(cardDropped()));

}

void MainWindow::gameOver(){
    connect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(processMsg()));
    if(msgBox->getMsg(GAME_OVER,tno)){
        if(tno == myNo){
            QMessageBox::information(NULL, "Game Over", "Congratulations! You win!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        }
        else{
            QMessageBox::information(NULL, "Game Over", "Player "+playerName[tno]+" wins!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        }

    }
    else
        error();
    disconnect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(gameOver()));

}

void MainWindow::enable(int no){
    for(int i = 0; i != icons.size(); i++){
        if(i != myNo){
          icons[i]->setSelectable(true);
          connect(icons[i],SIGNAL(clicked()),this,SLOT(chooseConstraint()));
        }
     }
}

void MainWindow::enable(QGraphicsScene *cards, int i){
    QList<QGraphicsItem*> items(cards->items());
    for(int i = 0; i != items.size(); i++){
        items[i]->setFlag(QGraphicsItem::ItemIsSelectable);
    }
    connect(cards,SIGNAL(selectionChanged()),this,SLOT(chooseConstraint()));
}

void MainWindow::enable(bool exchange)
{
    if(exchange){
        QList<QGraphicsItem*> items = keepers->items();
        for(int i = 0; i != items.size(); i++){
            items[i]->setFlag(QGraphicsItem::ItemIsSelectable);
            connect(keepers,SIGNAL(selectionChanged()),this,SLOT(chooseConstraint()));
        }
    }
    else{
        for(int i = 0; i != CLIENT_MAX; i++){
            if(i == myNo)
                continue;
            QList<QGraphicsItem*> items(keepers->items(rKeepers[i]));
            for(int j = 0; j != items.size(); j++){
                connect(keepers,SIGNAL(selectionChanged()),this,SLOT(chooseConstraint()));
                items[j]->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
    }
}

void MainWindow::disable(){
    for(int i = 0; i != icons.size(); i++)
        icons[i]->setSelectable(false);
    QList<QGraphicsItem*> items = myHands->items();
    for(int i = 0; i != items.size(); i++)
        items[i]->setFlag(QGraphicsItem::ItemIsSelectable,false);
    items = keepers->items();
    for(int i = 0; i != items.size(); i++)
        items[i]->setFlag(QGraphicsItem::ItemIsSelectable,false);
    items = rules->items();
    for(int i = 0; i != items.size(); i++)
        items[i]->setFlag(QGraphicsItem::ItemIsSelectable,false);
    items = goals->items();
    for(int i = 0; i != items.size(); i++)
        items[i]->setFlag(QGraphicsItem::ItemIsSelectable,false);
}

void MainWindow::sendChoice(){
    switch(tmsg){
    case CHOOSE_PLAYER_C:{
        for(int i = 0; i != icons.size(); i++)
            if(icons[i]->isSelected()){
                tno = i;
            }
        if(!msgBox->createMsg(CHOOSE_PLAYER_I,tno))
            error();
        break;}
    case PLAY_C:
    {
        tcards.clear();
        QList<QGraphicsItem*> sel(myHands->selectedItems());
        for(int i = 0; i != sel.size(); i++)
            tcards.push_back(toCard(sel[i]));
        if(!msgBox->createMsg(PLAY_I,tcards))
            error();
        break;}
    case DROP_CARD_C:
    {
        tcards.clear();
        QList<QGraphicsItem*> sel(myHands->selectedItems());
        for(int i = 0; i != sel.size(); i++)
            tcards.push_back(toCard(sel[i]));
        if(!msgBox->createMsg(DROP_CARD_I,tcards))
            error();
        break;}
    case DROP_KEEPER_C:{
        tcards.clear();
        QList<QGraphicsItem*> sel(keepers->selectedItems());
        for(int i = 0; i != sel.size(); i++)
            tcards.push_back(toCard(sel[i]));
        if(!msgBox->createMsg(DROP_KEEPER_I,tcards))
            error();
        break;}
    case CHOOSE_KEEPER_C:
    case EXCHANGE_KEEPER_C:{
        tcards.clear();
        QList<QGraphicsItem*> sel(keepers->selectedItems());
        for(int i = 0; i != sel.size(); i++)
            tcards.push_back(toCard(sel[i]));
        if(!msgBox->createMsg(CHOOSE_KEEPER_I,tcards))
            error();
        break;}
    case DROP_RULE_C:{
        tcards.clear();
        QList<QGraphicsItem*> sel(rules->selectedItems());
        for(int i = 0; i != sel.size(); i++)
            tcards.push_back(toCard(sel[i]));
        if(!msgBox->createMsg(DROP_RULE_I,tcards))
            error();
        break;}
    case CHOOSE_GOAL_C:{
        tcards.clear();
        QList<QGraphicsItem*> sel(goals->selectedItems());
        for(int i = 0; i != sel.size(); i++)
            tcards.push_back(toCard(sel[i]));
        if(!msgBox->createMsg(CHOOSE_GOAL_I,tcards))
            error();
        break;}
    default:    break;
    }
    disable();
    disconnect(confirm,SIGNAL(clicked()),this,SLOT(sendChoice()));
    confirm->setEnabled(false);
    ////processMsg();
    connect(msgBox->tcpSocket,SIGNAL(readyRead()),this,SLOT(processMsg()));
}

void MainWindow::reHands()
{
    QList<QGraphicsItem*> items = myHands->items();
    if(items.size()>6){
        for(int i = 0; i != items.size(); i++)
            items[i]->setPos((HAND_W-CARD_W)/(float)(items.size())*i,0);
    }
    else
        for(int i = 0; i != items.size(); i++)
            items[i]->setPos((CARD_W+HAND_S)*i,0);
}

//void MainWindow::reKeepers(QList<QRectF> rect)
//{
//    QGraphicsScene* theScene = dynamic_cast<QGraphicsScene*>(sender());
//    int no = rect[0].bottom()/200;
//    QList<QGraphicsItem*> items = theScene->items(QRectF(0,no*200,140,140));
//    for(int i = 0; i != items.size()/4; i++)
//        for(int j = 0; j != 4 && (4*i+j) != items.size(); j++)
//            items[i]->setPos(140*i,35*j+no*200);
//}

//void MainWindow::reRules()
//{
//    QGraphicsScene* theScene = dynamic_cast<QGraphicsScene*>(sender());
//    QList<QGraphicsItem*> items = theScene->items();
//    for(int i = 0; i != items.size(); i++)
//        items[i]->setPos(80*i,0);
//}

//void MainWindow::reGoal()
//{
//    QGraphicsScene* theScene = dynamic_cast<QGraphicsScene*>(sender());
//    QList<QGraphicsItem*> items = theScene->items();
//    for(int i = 0; i != items.size(); i++)
//        items[i]->setPos(80*i,0);
//}

void MainWindow::clearPresent()
{
    presentCard->clear();
	presentCard->setFixedSize(CARD_W,CARD_L);
    for(int i = 0; i != droppedCards.size(); i++){
        delete droppedCards[i];
    }
}

void MainWindow::chooseConstraint()
{
    if(dynamic_cast<QAvatar*>(sender())){
        int counter = 0;
        for(int i = 0; i != icons.size(); i++)
            if(icons[i]->isSelected())
                counter++;
        if(counter == 1){
            for(int i = 0; i != icons.size(); i++)
                if(!icons[i]->isSelected())
                    icons[i]->setSelectable(false);
        }
        else{
//            confirm->setEnabled(true);
            for(int i = 0; i != icons.size(); i++)
                    icons[i]->setSelectable(true);
        }
    }
    else if(QGraphicsScene* scene = dynamic_cast<QGraphicsScene*>(sender())){
        QList<QGraphicsItem*> items(scene->items());
        int no = (tmsg == DROP_KEEPER_C || tmsg == DROP_RULE_C || tmsg == DROP_CARD_C)?tinfo:1;
        if(tmsg == DROP_KEEPER_C)
            items = scene->items(rKeepers[myNo]);
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

//void MainWindow::chooseConstraint(bool exchange)
//{
//    if(exchange){
//        QList<QGraphicsItem*> sel = keepers->selectedItems();
//        switch(sel.size()){
//        case 0:     enable(true);break;
//        case 1:
//            confirm->setEnabled(false);
//            if(rKeepers[myNo].contains(sel[0]->boundingRect())){
//                QList<QGraphicsItem*> items(keepers->items(rKeepers[myNo]));
//                for(int i = 0; i != items.size(); i++)
//                    if(!items[i]->isSelected())
//                        items[i]->setFlag(QGraphicsItem::ItemIsSelectable,false);
//            }
//            else{
//                QList<QGraphicsItem*> items(keepers->items());
//                for(int i = 0; i != items.size(); i++)
//                    if(!items[i]->isSelected() && !rKeepers[myNo].contains(items[i]->boundingRect()))
//                        items[i]->setFlag(QGraphicsItem::ItemIsSelectable,false);
//            }
//            break;
//        case 2:
//          confirm->setEnabled(true);
//          QList<QGraphicsItem*> items(keepers->items());
//          for(int i = 0; i != items.size(); i++)
//              if(!items[i]->isSelected())
//                  items[i]->setFlag(QGraphicsItem::ItemIsSelectable,false);
//          break;
//        }
//    }
//}

void MainWindow::error(){
    QMessageBox::warning(NULL, "Oops~", "Something wrong!"+QString::number(tmsg), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    close();
}
