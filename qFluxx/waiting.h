#ifndef WAITING_H
#define WAITING_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QList>
#include <QVBoxLayout>

#include "config.h"

class Waiting : public QMainWindow
{
    Q_OBJECT
public:
    explicit Waiting(bool host,QWidget *parent = 0);
    ~Waiting();

    QPushButton* start;
    void addPlayer(QString name);

    QList<QLabel*> playerList;
    QVBoxLayout layout;

signals:

public slots:
    void enableStart();
};

#endif // WAITING_H
