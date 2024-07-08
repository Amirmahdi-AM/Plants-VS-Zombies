#ifndef BUCKETHEADZOMBIE_H
#define BUCKETHEADZOMBIE_H
#include "zombies.h"

class BucketHeadZombie : public Zombies
{
    Q_OBJECT
public:
    BucketHeadZombie(int x, int y, QWidget *parent = nullptr);
};

#endif // BUCKETHEADZOMBIE_H
