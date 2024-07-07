#include "jalapeno.h"

Jalapeno::Jalapeno(int x, int y, QWidget *parent ) : Plants(parent) {
    picture.load(":/Images/Jalapeno_P.png");
    setPixmap(picture);
    setGeometry(x, y, 100, 100);
    show();
}
