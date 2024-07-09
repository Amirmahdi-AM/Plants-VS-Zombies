#ifndef PLANTS_H
#define PLANTS_H

#include <QWidget>
#include <QTimer>
#include <Qwidget>
#include <QString>
#include <QPixmap>
#include <QLabel>

class Plants : public QLabel
{
    Q_OBJECT


protected:
    QTimer *shootTimer;
    int HP;
    int neededSun;
    bool shootState = false;
    int firingRate = 0;
    int attackPowe = 0;
    QPixmap picture;
public:
    Plants(QWidget *parent = nullptr);
    //virtual void decreaseHP(int power);
};

#endif // PLANTS_H
