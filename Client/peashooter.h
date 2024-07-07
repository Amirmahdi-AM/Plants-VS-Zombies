#ifndef PEASHOOTER_H
#define PEASHOOTER_H
#include "plants.h"

class PeaShooter : public Plants
{
    Q_OBJECT
public:
    PeaShooter(int x, int y, QWidget *parent = nullptr);
};

#endif // PEASHOOTER_H
