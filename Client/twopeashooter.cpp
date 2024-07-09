#include "twopeashooter.h"

TwoPeaShooter::TwoPeaShooter(int x, int y, QWidget *parent) : Plants(parent) {
    picture.load(":/Images/two_peashooter_P.png");
    setPixmap(picture);
    setGeometry(x, y, 100, 100);
    show();
    checkTarget = new QTimer(this);
    checkTarget->start(100);
    shooting = new QTimer(this);
    connect(checkTarget, &QTimer::timeout, this, &TwoPeaShooter::onCheckedTargets);
    connect(shooting, &QTimer::timeout, this, &TwoPeaShooter::Fire);
    firingRate = 1000;
}

void TwoPeaShooter::Fire()
{
    emit createPea(x() + 50, y() + 18);
    emit createPea(x() + 60, y() + 18);
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
