#include "boomerangpea.h"

void BoomerangPea::onMove()
{
    setGeometry(x() + 4, y(), 30, 30);
}

BoomerangPea::BoomerangPea(int x, int y, int _power, QWidget *parent) : QLabel(parent)
{
    picture.load(":/Images/Pea.png");
    setPixmap(picture);
    setGeometry(x+10,y-5,30,30);
    power = _power;
    show();
    move = new QTimer(this);
    connect(move, &QTimer::timeout, this, &BoomerangPea::onMove);
    move->start(10);
}

int BoomerangPea::getPower()
{
    return power;
}
