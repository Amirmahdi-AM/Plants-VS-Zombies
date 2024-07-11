#include "boomerangpea.h"

void BoomerangPea::onMove()
{
    if(rotationAngle==365){
        rotationAngle=0;
    }
    QPixmap Pic(":/Images/BoomrangBullet.png");
    QTransform transform;
    transform.rotate(rotationAngle);
    QPixmap rotatedPic = Pic.transformed(transform);
    this->setPixmap(rotatedPic);
    rotationAngle+=10;
    setGeometry(x() + 10, y(), 30, 30);
}

BoomerangPea::BoomerangPea(int x, int y, int _power, QWidget *parent) : QLabel(parent)
{
    picture.load(":/Images/BoomrangBullet.png");
    setPixmap(picture);
    setGeometry(x+10,y-5,30,30);
    rotationAngle = 0;
    power = _power;
    show();
    move = new QTimer(this);
    connect(move, &QTimer::timeout, this, &BoomerangPea::onMove);
    move->start(25);
}

int BoomerangPea::getPower()
{
    return power;
}
