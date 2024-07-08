#include "bucketheadzombie.h"

BucketHeadZombie::BucketHeadZombie(int x, int y, QWidget *parent)  : Zombies(parent)
{
    picture.load(":/Images/Bucket_Z.png");
    setPixmap(picture);
    setGeometry(x, y, 100, 100);
    show();
}
