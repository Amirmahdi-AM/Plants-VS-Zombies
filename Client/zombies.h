#ifndef ZOMBIES_H
#define ZOMBIES_H
#include <QLabel>
#include "plants.h"
#include <QTimer>
#include <QPixmap>

class Zombies : public QLabel
{
    Q_OBJECT

protected:
    int HP;
    int moveMentDelay;
    int attackPower;
    int TBA;

    QTimer *attackTimer;
    QTimer *moveTimer;
    QPixmap picture;

public:
    Plants *target;
    int getHp();
protected slots:
    virtual void move() = 0;

public:
    Zombies(QWidget *parent = nullptr);
    void decreaseHP(int power);
};

#endif // ZOMBIES_H
