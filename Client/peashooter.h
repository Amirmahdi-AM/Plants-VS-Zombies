#ifndef PEASHOOTER_H
#define PEASHOOTER_H
#include "plants.h"

class PeaShooter : public Plants
{
    Q_OBJECT

signals:
    void createPea(int x, int y, int power);
private slots:
    void Fire();
    void onCheckedTargets();
public:
    PeaShooter(int x, int y, QWidget *parent = nullptr);
    int target = 0;
    QTimer *checkTarget;
    QTimer *shooting;
};

#endif // PEASHOOTER_H
