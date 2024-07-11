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
    QMovie* WalkingAnimation;
    QMovie* EatingAnimation;
    QPixmap picture;

    double moveX = 5;

signals:
    void cleanLocation(int x, int y);


public:
    Plants *target = NULL;
    int getHp();
protected slots:
    virtual void move() = 0;
    virtual void onAttack();

public:
    Zombies(QWidget *parent = nullptr);
    virtual void decreaseHP(int power);
    virtual void offMovement();
    void onMovemevt();

};

#endif // ZOMBIES_H
