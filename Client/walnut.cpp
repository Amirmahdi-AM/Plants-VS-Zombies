#include "walnut.h"

Walnut::Walnut(int x, int y, QWidget *parent) : Plants(parent) {
    picture.load(":/Images/Walnut_P.png");
    setPixmap(picture);
    setGeometry(x, y, 100, 100);
    show();
    HP = 400;
}

void Walnut::decreaseHP(int power)
{

      HP -= power;
      if(HP<=250){
          if(HP<=150){
              picture.load(":/Images/Wallnut_cracked2.png");
              setPixmap(picture);
          }
          else{
              picture.load(":/Images/Wallnut_cracked1.png");
              setPixmap(picture);
          }

      }

}
