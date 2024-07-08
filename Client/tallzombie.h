#ifndef TALLZOMBIE_H
#define TALLZOMBIE_H
#include "zombies.h"

class TallZombie : public Zombies
{
    Q_OBJECT
public:
    TallZombie(int x, int y, QWidget *parent = nullptr);
private slots:
    void move() override;
};

#endif // TALLZOMBIE_H
