#ifndef BOOMERANGPEA_H
#define BOOMERANGPEA_H
#include <QLabel>
#include <QTimer>
#include <QPixmap>

class BoomerangPea : public QLabel
{
    Q_OBJECT

    QTimer *move;
    int power;
    QPixmap picture;

protected slots:
    void onMove();

public:
    BoomerangPea(int x, int y, int _power, QWidget *parent = nullptr);
    int getPower();
};

#endif // BOOMERANGPEA_H
