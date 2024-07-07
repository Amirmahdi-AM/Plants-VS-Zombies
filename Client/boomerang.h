#ifndef BOOMERANG_H
#define BOOMERANG_H
#include "plants.h"

class Boomerang : public Plants
{
    Q_OBJECT
public:
    Boomerang(int x, int y, QWidget *parent = nullptr);
};

#endif // BOOMERANG_H
