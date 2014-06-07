#include "register.h"


Register::Register(QWidget *parent) :
    QDialog(parent)
{
    layout = new QVBoxLayout;
    layout_pb = new QHBoxLayout;
    ok = new QPushButton("OK",this);
    cancel = new QPushButton("Cancel",this);
    name = new QLineEdit(this);
    request = new QLabel(this);

    request->setText("Please enter a name for you:");

    layout->addStretch(200);
    layout->addWidget(request);
    layout->addWidget(name);

    layout_pb->addWidget(ok);
    layout_pb->addWidget(cancel);

    layout->addLayout(layout_pb);
    layout->addStretch(200);

    setLayout(layout);
}

Register::~Register()
{
    delete ok;
    delete cancel;
    delete name;
    delete request;
}
