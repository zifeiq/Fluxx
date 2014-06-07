#ifndef MODE_H
#define MODE_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>

class Mode : public QDialog
{
    Q_OBJECT
public:
    explicit Mode(QWidget *parent = 0);
    QPushButton* host;      // for a host game
    QPushButton* part;      // for participation

private:
    QVBoxLayout* layout;
};

#endif // MODE_H
