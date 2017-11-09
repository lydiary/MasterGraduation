#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <thread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    utilities_ = Utilities::getInstance();
    captureThread = nullptr;
    capturing = false;
}

MainWindow::~MainWindow()
{
    delete ui;
    exitCaptureThread();
    delete utilities_;
}

void MainWindow::on_pushButton_clicked()
{
    capturing = !capturing;

    if (capturing) {
        ui->pushButton->setText("结束");
        captureThread = new std::thread([this](){
            while (capturing) {
                std::vector<std::vector<UCHAR>> image;
                utilities_->getPicture(image);

                if (image.size() <= 0)
                    continue;

                UCHAR *buffer = new UCHAR[image.size() * image[0].size()];
                UINT32 len = 0;
                for (const auto &item : image) {
                    memcpy(buffer + len, item.data(), item.size());
                    len += item.size();
                }
                ui->label->setPixmap(QPixmap::fromImage(QImage(buffer, image[0].size() / 3,
                                                        image.size(), QImage::Format_RGB888)));
                ui->label->show();
                delete buffer;
            }
        });
    } else {
        ui->pushButton->setText("开始");
        exitCaptureThread();
    }
}

void MainWindow::exitCaptureThread()
{
    capturing = false;
    if (captureThread != nullptr && captureThread->joinable()) {
        captureThread->join();
        delete captureThread;
        captureThread = nullptr;
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    close();
}
