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
    Plants *target;
    QTimer *attackTimer;
    QTimer *moveTimer;
    QPixmap picture;


private slots:
    //virtual void move();

public:
    Zombies(QWidget *parent = nullptr);
    //virtual void decreaseHP(int power);
};

#endif // ZOMBIES_H