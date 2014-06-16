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
    explicit Register(QWidget *parent = 0);
    ~Register();
    QLineEdit* name;        //输入昵称的单行文本框
    QPushButton* ok;        //确认按钮
    QPushButton* cancel;    //取消按钮
private:
    QVBoxLayout* layout;
    QHBoxLayout* layout_pb;
    QLabel* request;
};

#endif // REGISTER_H
