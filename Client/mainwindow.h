#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
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
    ~MainWindow();
public slots:

    void Loading_rotation();
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

private:
    Ui::MainWindow *ui;

    QTimer* Rotate;
};
#endif // MAINWINDOW_H
