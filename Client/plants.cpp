#include "plants.h"

Plants::Plants(const QString &imagePath, QWidget *parent) : QLabel(parent)
{
    picture.load(imagePath);
    setPixmap(picture);
    setGeometry(100,100,100,100);
    show();
}
