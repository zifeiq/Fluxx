#include "qavatar.h"

QAvatar::QAvatar(QWidget *parent) :
    QLabel(parent)
{
    selected = false;
}

QAvatar::QAvatar(int no, QString name, QWidget *parent) :
    QLabel(parent)
{
    this->setPixmap(QPixmap("/Users/zoe/Desktop/icon"+QString::number(no)+".jpg").scaled(100,100));
    playerNum = no;
    playerName = name;
    selected = false;
}

void QAvatar::mousePressEvent(QMouseEvent *event)
{
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

void QAvatar::mouseReleaseEvent(QMouseEvent *event)
{
    emit clicked();
}
