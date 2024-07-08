#ifndef REGULARZOMBIE_H
#define REGULARZOMBIE_H
#include "zombies.h"

class RegularZombie : public Zombies
{
    Q_OBJECT
public:
    RegularZombie(int x, int y, QWidget *parent = nullptr);
private slots:
    void move() override;
};

#endif // REGULARZOMBIE_H
