#ifndef TWOPEASHOOTER_H
#define TWOPEASHOOTER_H
#include "plants.h"

class TwoPeaShooter : public Plants
{
    Q_OBJECT

signals:
    void createPea(int x, int y);
private slots:
    void Fire();
    void onCheckedTargets();
public:
    TwoPeaShooter(int x, int y, QWidget *parent = nullptr);
    int target = 0;
    QTimer *checkTarget;
    QTimer *shooting;
};

#endif // TWOPEASHOOTER_H
