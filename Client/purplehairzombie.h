#ifndef PURPLEHAIRZOMBIE_H
#define PURPLEHAIRZOMBIE_H
#include "zombies.h"

class PurpleHairZombie : public Zombies
{
    Q_OBJECT
public:
    PurpleHairZombie(int x, int y, QWidget *parent = nullptr);
};

#endif // PURPLEHAIRZOMBIE_H
