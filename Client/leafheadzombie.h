#ifndef LEAFHEADZOMBIE_H
#define LEAFHEADZOMBIE_H
#include "zombies.h"

class LeafHeadZombie : public Zombies
{
    Q_OBJECT
public:
    LeafHeadZombie(int x, int y, QWidget *parent = nullptr);
};

#endif // LEAFHEADZOMBIE_H
