// This file was supplied by Prof. Marshall Long for ENAS 130.

#ifndef MOREGRAPHICSWINDOW_H
#define MOREGRAPHICSWINDOW_H

#include <QDialog>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include "moregraphics.h"

namespace Ui
{
    class GraphicsWindow;
}

class GraphicsWindow : public QDialog
{
public:
    explicit GraphicsWindow(QWidget *parent = 0);
    ~GraphicsWindow();
    void setupPixmap(int width,int height);

protected:
    void mousePressEvent(QMouseEvent*);
    void paintEvent(QPaintEvent*);
    void mouseMoveEvent(QMouseEvent*);

private:
    Q_OBJECT
    Ui::GraphicsWindow *ui;
    QPixmap pixmap;
};

#endif // MOREGRAPHICSWINDOW_H
