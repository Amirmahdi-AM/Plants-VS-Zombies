#include "plants.h"
#include <QMovie>
Plants::Plants(QWidget *parent) : QLabel(parent)
{

    /*.load(imagePath);
    setPixmap(picture);
    setGeometry(x,y,100,100);
    show();*/
}

void Plants::decreaseHP(int power)
{
    HP -= power;
}

int Plants::getHP()
{
    return HP;
}
