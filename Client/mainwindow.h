#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMouseEvent>
#include <QMainWindow>
#include <QTimer>
#include <QTcpSocket>
#include <list>
#include "person.h"
#include "QLabel"
#include "pea.h"
#include "plants.h"
#include "zombies.h"
#include "boomerangpea.h"
#include <mutex>
#include <shared_mutex>
#include <thread>
using namespace std;



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void Plants_set();
    void Zombies_set();
    ~MainWindow();
    void BOOM(int _x, int _y,QString type);

public slots:

    void Loading_rotation();
    void waiting_rotation();
    void decreasePlantsTargets(int _y);

private slots:
    void on_SignUp_clicked();

    void on_SignupCheck_clicked();

    void on_Back_clicked();

    void on_eye_login_clicked();

    void on_eye_signup_clicked();

    void on_forgot_pass_clicked();

    void on_Ok_RPass_clicked();

    void on_Back_newpass_clicked();

    void on_Back_newpass_2_clicked();

    void on_Ok_newpass_clicked();

    void onReadyRead();

    void on_connectButton_clicked();

    void onConnected();

    void onError(QTcpSocket::SocketError socketError);

    void onConnectionTimeout();

    void on_Ok_clicked();

    void sun_rotation();

    void brain_rotation();

    void Spawnning_Item();

    void Fade_Item();

    void on_Spawned_Item_Lable_clicked();

    void on_Start_Game_Botton_clicked();

    void Drag_Lable();

    void onCreateBullets(int x, int y, int power);

    void onCheckcollision();

    void onCleanLocation(int x, int y);

    void onCreateBBullets(int x, int y, int power);

    void onBulletcollision();


signals:
    void brainClicked();

    void sunClicked();
private:

    Person Player;

    Ui::MainWindow *ui;

    QTimer* Rotate;

    QTimer* Rotate2;

    QTimer* Sun_spawn;

    QTimer* fade;

    QTimer* Sun_Rotate;

    QTimer* brain_Rotate;

    QTimer* brain_spawn;

    QTimer* Brainfade;

    QTimer* Labeldrag_drop;

    QTimer* checkCollision;

    QLabel* draging_Label;

    QLabel* spawnedItemp_Label;

    QTcpSocket *socket;

    QTimer *connectionTimer;

    QTimer* Bulletcollision;

    QWidget* currentMap;

    int P_or_Z = 0;

    int selection = 0;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    list<pair<int, int>> fullLocations;

    QVector<Plants*> plants;
    QVector<Zombies*> zombies;


    QVector<Pea*> Peavec;
    QVector<BoomerangPea*> BPeavec;

    mutex PeaBulletmute;
    mutex BPeaBulletmute;
    mutex Locationmute;
    shared_mutex resourceMute;

};
#endif // MAINWINDOW_H
