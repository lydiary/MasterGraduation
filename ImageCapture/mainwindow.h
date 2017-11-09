#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <thread>
#include "Utilities.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void exitCaptureThread();

private:
    Ui::MainWindow *ui;

    Utilities *utilities_;
    std::thread *captureThread;
    bool capturing;
};

#endif // MAINWINDOW_H
