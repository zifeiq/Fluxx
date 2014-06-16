#include "qcard.h"
#include <iostream>
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
    QGraphicsPixmapItem(QPixmap(":/cards/"+s2q(acard->getAddr())).scaled(CARD_W,CARD_L),parent)
{
    std::cout << ":/cards/"+acard->getAddr() << std::endl;
    lp_timer = new QTimer();
    lp_timer->setSingleShot(true);
    connect(lp_timer,SIGNAL(timeout()),this,SLOT(longPress()));
//    selected = false;
    setData(0,(int)acard->getType()*100+acard->getNum());
    setData(1,type);
}


void QCard::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    lp_timer->start(300);
}

void QCard::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(flags().testFlag(QGraphicsItem::ItemIsSelectable)){
        if(lp_timer->remainingTime() > 0){
            lp_timer->stop();
            if(isSelected()){
				if(data(1).toInt()==QHAND)
					setPos(scenePos()+QPointF(0,10));
                setSelected(false);
            }
            else{
				if(data(1).toInt()==QHAND)
					setPos(scenePos()+QPointF(0,-10));
                setSelected(true);
            }
            emit clicked();
        }
    }
}

void QCard::longPress(){

}
