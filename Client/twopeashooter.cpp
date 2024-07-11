#include "twopeashooter.h"
#include "qmovie.h"

TwoPeaShooter::TwoPeaShooter(int x, int y, QWidget *parent) : Plants(parent) {
//    picture.load(":/Images/two_peashooter_P.png");
//    setPixmap(picture);
    animation = new QMovie(":/Images/two_peashooter_P_Mpving.gif");
    this->setMovie(animation);
    animation->start();

    setGeometry(x, y, 100, 100);
    show();
    checkTarget = new QTimer(this);
    checkTarget->start(100);
    shooting = new QTimer(this);
    connect(checkTarget, &QTimer::timeout, this, &TwoPeaShooter::onCheckedTargets);
    connect(shooting, &QTimer::timeout, this, &TwoPeaShooter::Fire);
    firingRate = 1000;
    HP = 200;
    attackPowe = 20;
}

void TwoPeaShooter::Fire()
{
    emit createPea(x() + 50, y() + 18, attackPowe);
    emit createPea(x() + 30, y() + 18, attackPowe);
}

void TwoPeaShooter::onCheckedTargets()
{
    if (target > 0) {
        if (!shooting->isActive())
            shooting->start(firingRate);
    }
    else {
        if (shooting->isActive())
            shooting->stop();
    }
}
