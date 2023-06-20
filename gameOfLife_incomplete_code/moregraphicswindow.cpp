// This file was supplied by Prof. Marshall Long for ENAS 130.

#include "moregraphicswindow.h"
#include "ui_moregraphicswindow.h"

extern QPixmap *thePixMap;

float scaledX,scaledY;
int mouseState = 0;

GraphicsWindow::GraphicsWindow(QWidget *parent) : QDialog(parent), ui(new Ui::GraphicsWindow)
{
    ui->setupUi(this);
}

GraphicsWindow::~GraphicsWindow()
{
    delete ui;
}

void GraphicsWindow::mousePressEvent(QMouseEvent *event)
{
    extern float xmin,ymin,xscalerange,yscalerange;
    extern int ymaxim;

        QPoint pos = event->pos();
        //printf("%d %d\n",pos.x(),pos.y());
        scaledX = pos.x()/xscalerange + xmin;
        scaledY = (ymaxim-pos.y())/yscalerange + ymin;
        //printf("%f %f\n",scaledX,scaledY);

        if(event->button() == Qt::RightButton)
        {
            mouseState = RIGHT_BUTTON;
        }
        else if(event->button() == Qt::MiddleButton)
        {
            mouseState = CENTER_BUTTON;
        }
        else
        {
            mouseState = LEFT_BUTTON;
        }

        return;
}

void GraphicsWindow::mouseMoveEvent(QMouseEvent *event)
{
    extern float xmin,ymin,xscalerange,yscalerange;
    extern int ymaxim;

        QPoint pos = event->pos();
        //printf("%d %d\n",pos.x(),pos.y());
        scaledX = pos.x()/xscalerange + xmin;
        scaledY = (ymaxim-pos.y())/yscalerange + ymin;
        //printf("%f %f\n",scaledX,scaledY);

        if(event->button() == Qt::RightButton)
        {
            mouseState = RIGHT_MOVING;
        }
        else if(event->button() == Qt::MiddleButton)
        {
            mouseState = CENTER_MOVING;
        }
        else
        {
            mouseState = LEFT_MOVING;
        }

        return;
}

void GraphicsWindow::paintEvent(QPaintEvent*)
{
    ui->drawArea->setPixmap(pixmap);
    ui->drawArea->show();

    return;
}

void GraphicsWindow::setupPixmap(int width,int height)
{
    extern int pixSize;
    //pixSize = devicePixelRatio();
    pixSize=1;

    if (pixSize == 1)
        pixmap = QPixmap( width, height);
    else
        pixmap = QPixmap( width*2, height*2);

    pixmap.fill(Qt::white);
    ui->drawArea->setPixmap(pixmap);
    ui->drawArea->show();
    thePixMap = &pixmap;

    return;
}
