#ifndef PEA_H
#define PEA_H
#include <QLabel>
#include <QTimer>
#include <QPixmap>

class Pea : public QLabel
{
    Q_OBJECT
    QTimer *move;
    int power;
    QPixmap picture;

private slots:
    void onMove();

public:
    Pea(int x, int y, int _power, QWidget *parent = nullptr);
    int getPower();
};

#endif // PEA_H
