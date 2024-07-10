#ifndef BOOMERANG_H
#define BOOMERANG_H
#include "plants.h"

class Boomerang : public Plants
{
    Q_OBJECT

signals:
    void createBBullet(int x, int y, int power);
private slots:
    void Fire();
    void onCheckedTargets();
public:
    Boomerang(int x, int y, QWidget *parent = nullptr);
    int target = 0;
    QTimer *checkTarget;
    QTimer *shooting;
};

#endif // BOOMERANG_H
