#ifndef WALNUT_H
#define WALNUT_H
#include "plants.h"

class Walnut : public Plants
{
    Q_OBJECT
public:
    Walnut(int x, int y, QWidget *parent = nullptr);
};

#endif // WALNUT_H
