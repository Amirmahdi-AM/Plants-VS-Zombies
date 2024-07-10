#include "pea.h"

void Pea::onMove()
{
    setGeometry(x() + 10, y(), 30, 30);
}

Pea::Pea(int x, int y, int _power, QWidget *parent) : QLabel(parent)
{
    picture.load(":/Images/Pea.png");
    setPixmap(picture);
    setGeometry(x+10,y-5,30,30);
    power = _power;
    show();
    move = new QTimer(this);
    connect(move, &QTimer::timeout, this, &Pea::onMove);
    move->start(25);
}

int Pea::getPower()
{
    return power;
}
