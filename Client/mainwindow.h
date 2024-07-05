#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTcpSocket>
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

public slots:

    void Loading_rotation();
    void waiting_rotation();

private slots:
    void on_SignUp_clicked();

    void on_SignupCheck_clicked();

    void on_Back_clicked();

    void on_eye_login_clicked();

    void on_eye_signup_clicked();

    void on_forgot_pass_clicked();

    void on_Ok_RPass_clicked();

    void on_Ok_newpass_2_clicked();

    void on_Ok_newpass_3_clicked();

    void on_Ok_newpass_clicked();

    void onReadyRead();

    void on_connectButton_clicked();

    void onConnected();

    void onError(QTcpSocket::SocketError socketError);

    void onConnectionTimeout();

    void on_Ok_clicked();

    void sun_rotation();

    void Spawnning_sun();

    void Fade_sun();

    void on_Spawned_sun_clicked();

    void on_Start_Game_Botton_clicked();

private:
    Ui::MainWindow *ui;

    QTimer* Rotate;

    QTimer* Rotate2;

    QTimer* Sun_spawn;

    QTimer* fade;

    QTimer* Sun_Rotate;

    QTcpSocket *socket;

    QTimer *connectionTimer;

    int P_or_Z;
};
#endif // MAINWINDOW_H
