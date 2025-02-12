#include "boomerang.h"

Boomerang::Boomerang(int x, int y, QWidget *parent) : Plants(parent) {
    picture.load(":/Images/Boomerang_P.png");
    setPixmap(picture);
    setGeometry(x, y, 100, 100);
    show();
    checkTarget = new QTimer(this);
    checkTarget->start(100);
    shooting = new QTimer(this);
    connect(checkTarget, &QTimer::timeout, this, &Boomerang::onCheckedTargets);
    connect(shooting, &QTimer::timeout, this, &Boomerang::Fire);
    firingRate = 1000;
    HP = 200;
    attackPowe = 30;
}
void Boomerang::Fire()
{
    emit createBBullet(x() + 50, y() + 18, attackPowe);
}

void Boomerang::onCheckedTargets()
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
