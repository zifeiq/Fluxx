#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QBoxLayout>


class Register : public QDialog
{
    Q_OBJECT

public:
    QLineEdit* name;
    QPushButton* ok;
    QPushButton* cancel;

    explicit Register(QWidget *parent = 0);
    ~Register();
private:
    QVBoxLayout* layout;
    QHBoxLayout* layout_pb;

    QLabel* request;
};

#endif // REGISTER_H
