#ifndef WAITING_H
#define WAITING_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QList>
#include <QVBoxLayout>
#include <QGridLayout>

#include "config.h"

class Waiting : public QDialog
{
    Q_OBJECT
public:
    explicit Waiting(bool host,QWidget *parent = 0);
    ~Waiting();

//    QPushButton* start;
    void addPlayer(int no, QString name);

    QLabel* logo;
    QLabel* playerName[CLIENT_MAX];
    QLabel* icon[CLIENT_MAX];
    QVBoxLayout* layout;
    QGridLayout* players;

    QPushButton* ai_s;
    QPushButton* ai_h;

signals:

public slots:
    void enableStart();
};

#endif // WAITING_H
