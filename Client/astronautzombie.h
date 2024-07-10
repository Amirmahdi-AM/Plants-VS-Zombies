#ifndef ASTRONAUTZOMBIE_H
#define ASTRONAUTZOMBIE_H
#include "zombies.h"

class AstronautZombie : public Zombies
{
    Q_OBJECT
public:
    AstronautZombie(int x, int y, QWidget *parent = nullptr);
    void decreaseHP(int power) override;
private slots:
    void move() override;

};

#endif // ASTRONAUTZOMBIE_H
