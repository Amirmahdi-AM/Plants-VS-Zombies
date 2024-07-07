#ifndef TWOPEASHOOTER_H
#define TWOPEASHOOTER_H
#include "plants.h"

class TwoPeaShooter : public Plants
{
    Q_OBJECT
public:
    TwoPeaShooter(int x, int y, QWidget *parent = nullptr);
};

#endif // TWOPEASHOOTER_H
