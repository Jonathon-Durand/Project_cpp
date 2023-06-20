// This file was supplied by Prof. Marshall Long for ENAS 130.

#ifndef MOREGRAPHICS_H
#define MOREGRAPHICS_H
#include "moregraphicswindow.h"
#include <QPainter>
#include <QPixmap>
#include <QApplication>
#include <time.h>

/*
Define graphics functions as follows:

void ploton(int height,int width)	Open a window of specified size (in pixels)
void scalx(float xmin,float xmax)	Scale window's x axis
void scaly(float ymin,float ymax)	Scale window's y axis
void set(float x,float y)		Place the pen at location x,y
void dvect(float x,float y)		Draw a line to location x,y, leave the pen at the new point
void cross(float x,float y)		Draw a cross at location x,y
void drawPoly(Vertex* polygon, int num, int fc, float intensity)	Draw a polygon.
                    polygon is an array containing num vertices that make up the polygon.
                    num is between 3 and 10.
                    fc specifies the color with 0-3 for grey, red, green, blue.
                    fc = -1 draws polygon without fill.
                    intensity is between 0. - 1.
void drawRect(float x0, float y0, float x1, float y1, int fc, float intensity)
                    Draw a rectangle with corners x0,y0 and x1,y1
                    fc specifies the color with 0-3 for grey, red, green, blue.
                    fc = -1 draws polygon without fill.
                    intensity is between 0. - 1.
void clear()            Erase everything in the window
void updateImage()      Update the image with everything that has been drawn so far
void delay(int centisecs)   Delay for the specified number of hundredths of seconds
int getmouse(float *x, float *y, int* drag)	Gets the x-y coordinates of a mouse click.
                    Scaling is as per scalx and scaly calls.
                    Return value is the number of the button pressed (or dragged).
                    Values are 1, 2, or 3
                    (Defined constants LEFT_BUTTON, CENTER_BUTTON, RIGHT_BUTTON.)
int notRightClick() 	Return 1 unless the right mouse button has been clicked. Then return 0.
 */

enum {NO_BUTTON,LEFT_BUTTON,CENTER_BUTTON,RIGHT_BUTTON,LEFT_MOVING,CENTER_MOVING,RIGHT_MOVING};

enum Color {GREY, RED, GREEN, BLUE};

// Use this data structure to define points in 3D
typedef struct {
    float x;
    float y;
    float z;
} Vertex;

void ploton(int,int);
void scalx(float,float);
void scaly(float,float);
void set(float,float);
void dvect(float,float);
void drawLine(int x0, int y0, int x1, int y1);
void cross(float,float);
void drawPoly(Vertex* polygon, int num, int fc, float intensity);
void drawRect(float x0, float y0, float x1, float y1, int fc, float intensity);
void clear();
void updateImage();
void delay(int centisecs);
int getmouse(float *x, float *y, int *drag);
int notRightClick();
QPoint screenCoord(Vertex p);
void getcolors();

#endif // MOREGRAPHICS_H

