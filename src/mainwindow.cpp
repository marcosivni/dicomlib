#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    const std::string filename="24759123/20010101/MR2/22490.dcm";
    DCMImage *dicomImage = new DCMImage(filename);
    QImage *img = convertImageToQImage(dicomImage);
    ui->lblImage->setPixmap(QPixmap::fromImage(img->scaled(500, 500)));

    delete(img);
    delete (dicomImage);
}

QImage* MainWindow::convertImageToQImage(Image *src){

    QImage *img = new QImage(src->getWidth(), src->getHeight(), QImage::Format_RGB32);

    uint16_t div;
    if ((src->getBitsPerPixel() > 8) && (src->getBitsPerPixel() <= 16))
        div = 16;
    else
        div = 1;

    for(size_t x = 0; x < src->getWidth(); x++){
        for (size_t y = 0; y < src->getHeight(); y++){
            uint16_t pp = (uint16_t) (src->getPixel(x, y).getGrayPixelValue()/div);
            //std::cout << x << " " << y << " " << pp << std::endl;
            if ((src->getPixel(x, y).getGrayPixelValue()/div) > 255)
                img->setPixel(x, y, qRgb(255, 255, 255));
            else
                img->setPixel(x, y, qRgb(pp, pp, pp));
        }
    }

    return img;
}

