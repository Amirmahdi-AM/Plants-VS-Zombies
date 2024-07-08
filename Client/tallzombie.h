#ifndef TALLZOMBIE_H
#define TALLZOMBIE_H
#include "zombies.h"

class TallZombie : public Zombies
{
    Q_OBJECT
public:
    TallZombie(int x, int y, QWidget *parent = nullptr);
};

#endif // TALLZOMBIE_H
