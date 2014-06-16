#include "waiting.h"

Waiting::Waiting(bool host, QWidget *parent) :
    QDialog(parent)
{ 
//    else
//        start = NULL;
//    layout = new QVBoxLayout;
//    layout->setDirection(QBoxLayout::TopToBottom);
//    layout->setGeometry(QRect(0,0,400,300));
//    setLayout(layout);
    layout = new QVBoxLayout(this);
    players = new QGridLayout;
    logo = new QLabel(this);
	logo->setPixmap(QPixmap(":/pics/logo").scaled(240,160));
    for(int i = 0; i != CLIENT_MAX; i++){
        playerName[i] = new QLabel(this);
        icon[i] = new QLabel(this);
        playerName[i]->setFixedSize(150,40);
        icon[i]->setFixedSize(100,100);
        players->addWidget(icon[i],i,0);
        players->addWidget(playerName[i],i,1);
        playerName[i]->show();
        icon[i]->show();
        playerName[i]->setStyleSheet("QLabel{border:2px solid black}");
        icon[i]->setStyleSheet("QLabel{border:2px solid black}");
    }
    layout->addStretch(200);
    layout->addWidget(logo);
//    layout->addStretch(200);
    layout->addLayout(players);
    layout->addStretch(200);

	if(host){
//        start = new QPushButton("START",this);
//        start->setEnabled(false);
          ai_s = new QPushButton(SIMPLE_AI,this);
          ai_h = new QPushButton(HARD_AI,this);
		  layout->addWidget(ai_s);
		  layout->addWidget(ai_h);
    }

    show();
}

Waiting::~Waiting()
{
    delete logo;
    for(int i = 0; i != CLIENT_MAX; i++){
        delete playerName[i];
        delete icon[i];
    }
    delete layout;
//    delete players;
	if(ai_s)
		delete ai_s;
	if(ai_h)
		delete ai_h;
}

void Waiting::addPlayer(int no, QString name)
{
//    QLabel* temp = new QLabel(name,this);
//    layout->addWidget(temp);
//    playerList.push_back(temp);
//    temp->setGeometry(50,50+20*playerList.size(),100,20);
//	temp->show();
    playerName[no]->setText(name);
	icon[no]->setPixmap(QPixmap(":/pics/player"+QString::number(no)).scaled(100,100));
}

void Waiting::enableStart()
{
//    start->setEnabled(true);
//    ai_s->setEnabled(false);
//    ai_h->setEnabled(false);
}

