#ifndef WAITING_H
#define WAITING_H

#include <QMainWindow>
#include <QPushButton>

class Waiting : public QMainWindow
{
    Q_OBJECT
public:
    explicit Waiting(QWidget *parent = 0);
    QPushButton* start;


signals:

public slots:

};

#endif // WAITING_H
