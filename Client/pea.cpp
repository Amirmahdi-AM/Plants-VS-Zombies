#include "pea.h"

void Pea::onMove()
{
    setGeometry(x() + 2, y(), 60, 60);
}

Pea::Pea(int x, int y, int power, QWidget *parent) : QLabel(parent)
{
    picture.load(":/Images/Pea.png");
    setPixmap(picture);
    setGeometry(x,y,60,60);
    show();
    move = new QTimer();
    connect(move, &QTimer::timeout, this, &Pea::onMove);
    move->start(10);
}
