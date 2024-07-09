#ifndef PLUMMINE_H
#define PLUMMINE_H
#include "plants.h"]

class PlumMine : public Plants
{
    Q_OBJECT
public:
    PlumMine(int x, int y, QWidget *parent = nullptr);
};

#endif // PLUMMINE_H
