#include "qcard.h"

QCard::QCard(QGraphicsItem *parent) :
    QGraphicsPixmapItem(parent)
{
    lp_timer = new QTimer();
    lp_timer->setSingleShot(true);
    connect(lp_timer,SIGNAL(timeout()),this,SLOT(longPress()));
//    selected = false;
}

QCard::QCard(const QPixmap &pixmap, QGraphicsItem *parent):
    QGraphicsPixmapItem(pixmap,parent)
{
    lp_timer = new QTimer();
    lp_timer->setSingleShot(true);
    connect(lp_timer,SIGNAL(timeout()),this,SLOT(longPress()));
//    selected = false;
//    setData(0,QVariant::fromValue(*acard));
}

QCard::QCard(const Card* acard, qcardType type, QGraphicsItem* parent):
    QGraphicsPixmapItem(QPixmap(s2q(acard->getAddr())).scaled(80,120),parent)
{
    lp_timer = new QTimer();
    lp_timer->setSingleShot(true);
    connect(lp_timer,SIGNAL(timeout()),this,SLOT(longPress()));
//    selected = false;
    setData(0,(int)acard->getType()*100+acard->getNum());
    setData(1,type);
}


void QCard::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    lp_timer->start(100);
}

void QCard::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(lp_timer->remainingTime() > 0){
        lp_timer->stop();
        if(isSelected()){
            setPos(scenePos()+QPointF(0,10));
            setSelected(false);
        }
        else{
            setPos(scenePos()+QPointF(0,-10));
            setSelected(true);
        }
        emit clicked();
    }
}

void QCard::longPress(){

}
