#ifndef PLANTS_H
#define PLANTS_H

#include <QWidget>
#include <QTimer>
#include <Qwidget>
#include <QString>
#include <QPixmap>
#include <QLabel>

class Plants : QLabel
{
    Q_OBJECT
private:
    QTimer *shootTimer;
    int HP;
    QPixmap picture;
public:
    Plants(const QString &imagePath, QWidget *parent = nullptr);
    virtual void decreaseHP(int power) = 0;

};

#endif // PLANTS_H
