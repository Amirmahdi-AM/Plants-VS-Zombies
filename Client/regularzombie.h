#ifndef REGULARZOMBIE_H
#define REGULARZOMBIE_H
#include "zombies.h"

class RegularZombie : public Zombies
{
    Q_OBJECT
public:
    RegularZombie(int x, int y, QWidget *parent = nullptr);
};

#endif // REGULARZOMBIE_H
