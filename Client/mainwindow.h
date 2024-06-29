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
    void ChangetabHandle();
private slots:
    void on_SignUp_clicked();

    void on_SignupCheck_clicked();

    void on_Back_clicked();

private:
    Ui::MainWindow *ui;
    QTimer* changetab;
};
#endif // MAINWINDOW_H
