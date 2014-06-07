#include "mode.h"

Mode::Mode(QWidget *parent) :
    QDialog(parent)
{
    layout = new QVBoxLayout;
    host = new QPushButton("Create A Host");
    part = new QPushButton("Participate A Game");

    layout->addStretch(200);
    layout->addWidget(host);
    layout->addWidget(part);
    layout->addStretch(200);

    this->setLayout(layout);
}
