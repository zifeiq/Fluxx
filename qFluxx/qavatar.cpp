#include "qavatar.h"

QAvatar::QAvatar(QWidget *parent) :
    QLabel(parent)
{
    selected = false;
    selectable = false;
}

QAvatar::QAvatar(int no, QString name, QWidget *parent) :
    QLabel(parent)
{
    this->setPixmap(QPixmap(":/pics/player"+QString::number(no)).scaled(100,100));
    playerNum = no;
    playerName = name;
    selected = false;
    selectable = false;
}

void QAvatar::setSelected(bool sel)
{
    if(selectable){
        selected = sel;
        if(selected){
            setFrameStyle(QFrame::NoFrame);
            setStyleSheet("border: 0px solid  #000000");
        }
        else{
            setFrameStyle(QFrame::Panel | QFrame::Raised);
            setStyleSheet("border: 2px solid  #c23a71");
        }
    }
}

void QAvatar::mousePressEvent(QMouseEvent *event)
{
    if(selectable){
        if(selected){
            setFrameStyle(QFrame::NoFrame);
            setStyleSheet("border: 0px solid  #000000");
        }
        else{
            setFrameStyle(QFrame::Panel | QFrame::Raised);
            setStyleSheet("border: 2px solid  #c23a71");
        }
        selected = !selected;
    }
}

void QAvatar::mouseReleaseEvent(QMouseEvent *event)
{
    if(selectable)
        emit clicked();
}
