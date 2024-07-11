#include "peashooter.h"
#include "qmovie.h"

void PeaShooter::Fire()
{
    emit createPea(x() + 50, y() + 18, attackPowe);
}

void PeaShooter::onCheckedTargets()
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

PeaShooter::PeaShooter(int x, int y, QWidget *parent) :Plants(parent) {
//    picture.load(":/Images/Peashooter_P.png");
//    setPixmap(picture);
    animation = new QMovie(":/Images/Peashooter_Moving.gif");
    this->setMovie(animation);
    animation->start();

    setGeometry(x, y, 100, 100);
    show();
    checkTarget = new QTimer(this);
    checkTarget->start(100);
    shooting = new QTimer(this);
    connect(checkTarget, &QTimer::timeout, this, &PeaShooter::onCheckedTargets);
    connect(shooting, &QTimer::timeout, this, &PeaShooter::Fire);
    firingRate = 1000;
    HP = 200;
    attackPowe = 15;
}
