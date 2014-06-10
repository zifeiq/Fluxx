#ifndef QCARD_H
#define QCARD_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QTimer>
#include "./cardLib_code/cardLib.h"

//Q_DECLARE_METATYPE(Card*)

class QCard : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit QCard(QGraphicsItem *parent = 0);
    explicit QCard(const QPixmap& pixmap, QGraphicsItem *parent = 0);
    explicit QCard(const Card *acard, QGraphicsItem *parent = 0);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

signals:

public slots:
    void longPress();

private:
    QTimer* lp_timer;

    QString s2q(const std::string& s) { return QString(QString::fromLocal8Bit(s.c_str())); }
    std::string q2s(const QString& s) { return std::string((const char *)s.toLocal8Bit()); }

};

#endif // QCARD_H
