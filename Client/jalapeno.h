#ifndef JALAPENO_H
#define JALAPENO_H
#include "plants.h"

class Jalapeno : public Plants
{
    Q_OBJECT
public:
    Jalapeno(int x, int y, QWidget *parent = nullptr);
};

#endif // JALAPENO_H
