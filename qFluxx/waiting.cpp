#include "waiting.h"

Waiting::Waiting(bool host, QWidget *parent) :
    QMainWindow(parent)
{ 
    if(host){
        start = new QPushButton("START",this);
        start->setEnabled(flase);
    }
    else
        start = NULL;
    show();
}

Waiting::~Waiting()
{
    for(int i = 0; i != playerList.size(); i++)
        delete playerList[i];
    if(start)
        delete start;
}

void Waiting::addPlayer(QString name)
{
    QLabel* temp = new QLabel(name,this);
    layout.addWidget(temp);
    playerList.push_back(temp);
}

void Waiting::enableStart()
{
    start->setEnabled(true);
}

