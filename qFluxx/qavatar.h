#ifndef QAVATAR_H
#define QAVATAR_H

#include <QLabel>
#include <QString>

class QAvatar : public QLabel
{
    Q_OBJECT
public:
    explicit QAvatar(QWidget *parent = 0);
    explicit QAvatar(int no, QString name, QWidget *parent = 0);
    int getNum() { return playerNum;}
    bool isSelected() { return selected;}
    void setSelected(bool sel);
    void setSelectable(bool enable) { selectable = enable;}
    QString getName() { return playerName;}

signals:
    void clicked();

public slots:

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

private:
    int playerNum;
    QString playerName;
    bool selected;
    bool selectable;
};

#endif // QICON_H


