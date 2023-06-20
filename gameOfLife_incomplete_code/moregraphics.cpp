// This file was supplied by Prof. Marshall Long for ENAS 130.

#include "moregraphics.h"
#include "moregraphicswindow.h"
#include "ui_moregraphicswindow.h"
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

extern GraphicsWindow *w2;

float   xmin,xmax,ymin,ymax,xscalerange,scalex,yscalerange,scaley;
int     ix,iy;
int     ymaxim,pixSize;

#define NUM_COLOR 4
#define SHADES 256
QColor colors[NUM_COLOR][SHADES];
QPixmap *thePixMap;

void ploton(int height,int width)
{
    if (height < 10) height = 10;
    if (width < 10) width = 10;

    scalex = width;
    scaley = height;
    ymaxim = height;

    w2->setGeometry(10,10,width,height);
    w2->show();
    w2->setupPixmap(width,height);
    w2->update();

    getcolors();

    return;
}

void scalx(float x,float y)
{
    xmin = x;
    xmax = y;
    xscalerange = scalex/(xmax-xmin);

    return;
}

void scaly(float x,float y)
{
    ymin = x;
    ymax = y;
    yscalerange = scaley/(ymax-ymin);

    return;
}

void set(float x,float y)
{
    ix = (int)((x-xmin)*xscalerange);
    iy = (int)((y-ymin)*yscalerange);
    iy = ymaxim - iy;

    return;
}

void dvect(float x,float y)
{
    int ix0,iy0;
    ix0 = ix;
    iy0 = iy;

    ix = (int)((x-xmin)*xscalerange);
    iy = (int)((y-ymin)*yscalerange);
    iy = ymaxim - iy;

    drawLine(ix0,iy0,ix,iy);

    return;
}

void drawLine(int x0, int y0, int x1, int y1)
{
    extern int pixSize;
    QPainter painter(thePixMap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setPen(QColor("black"));
    if (pixSize == 1)
        painter.drawLine(x0,y0,x1,y1);
    else 
    {
        QSize windowSize = w2->size();
        int extraHeight = windowSize.height()/2;
        painter.drawLine(x0,y0+extraHeight,x1,y1+extraHeight);
    }
    w2->update();

    return;
}

#define LX 4
void cross(float x,float y)
{
    ix = (int)((x-xmin)*xscalerange);
    iy = (int)((y-ymin)*yscalerange);
    iy = ymaxim - iy;

    drawLine( ix-LX, iy-LX, ix+LX,iy+LX);
    drawLine( ix-LX, iy+LX, ix+LX,iy-LX);

    return;
}

/*
    Draw a polygon
polygon is an array containing num vertices that make up the polygon
num is between 3 and 10
fc specifies the color with 0-3 for grey, red, green, blue; fc = -1 draws polygon without fill
intensity is between 0. - 1.
*/
void drawPoly(Vertex* polygon, int num, int fc, float intensity)
{
    int j;
    QPoint ppoly[11];
    extern int pixSize;

    for(j=0;j<num;j++)
    {
        ppoly[j] = screenCoord(polygon[j]);
        if (pixSize == 2)
        {
            QSize windowSize = w2->size();
            int extraHeight = windowSize.height()/2;
            ppoly[j].setY(ppoly[j].y()+extraHeight);
        }
    }

    if (fc >= 0 && fc < 4)
    {
        int shade = (int)(intensity*SHADES);
        if(shade>SHADES-1)shade=SHADES-1;   // shouldn't happen

        QPainter painter(thePixMap);
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter.setBrush(colors[fc][shade]);
        painter.setPen(colors[fc][shade]);
        painter.drawPolygon(ppoly, num);
        //draw the borders of this polygon in black
    }
    else
    {
        QPainter painter(thePixMap);
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter.setPen(Qt::black);
        painter.drawPolygon(ppoly, num);
    }
    w2->update();

    return;
}

void drawRect(float x0, float y0, float x1, float y1, int fc, float intensity)
{
    Vertex polygon[4];

    polygon[0].x = x0;
    polygon[0].y = y0;

    polygon[1].x = x1;
    polygon[1].y = y0;

    polygon[2].x = x1;
    polygon[2].y = y1;

    polygon[3].x = x0;
    polygon[3].y = y1;

    drawPoly(polygon,4,fc,intensity);

    return;
}

void clear()
{
    thePixMap->fill(Qt::white);
    w2->update();

    return;
}

void updateImage()
{
    extern QApplication *a2;
    a2->processEvents(QEventLoop::WaitForMoreEvents);

    return;
}

void delay(int centisecs)
{
    clock_t start;
    start = clock();
    while ( (clock()- start)*100/CLOCKS_PER_SEC < (clock_t)centisecs);

    return;
}

// Gets the x-y coordinates of a mouse click. Scaling is as per scalx and scaly calls.
// Return value is the number of the button pressed (or dragged).
// Values are 1, 2, or 3 (Defined constants LEFT_BUTTON, CENTER_BUTTON, RIGHT_BUTTON.)
int getmouse(float *x, float *y, int *drag)
{
    extern float scaledX,scaledY;
    extern int mouseState;
    extern QApplication *a2;

    // wait for mouse events
    while(mouseState == NO_BUTTON)
    {
        a2->processEvents(QEventLoop::WaitForMoreEvents);
    }

    *x = scaledX;
    *y = scaledY;
    *drag = 0;

    int button = mouseState;
    if(button > RIGHT_BUTTON){
        // there was a drag event
        *drag =1;
        button -= 3;  // get button number in range 1 - 3
    }
    mouseState = NO_BUTTON;

    return button;
}

int notRightClick()
{
    extern int mouseState;
    if(mouseState == RIGHT_BUTTON) return 0;

    return 1;
}

QPoint screenCoord(Vertex p)
{   
    // scale the x and y components of a Vertex to screen
    /*
    QPoint sp;
    sp.setX( (int)((p.x-xmin)*xscalerange));
    sp.setY( ymaxim -  (int)((p.y-ymin)*yscalerange));
    return(sp);
    */

    return QPoint((int)((p.x-xmin)*xscalerange), ymaxim-(int)((p.y-ymin)*yscalerange));
}

void getcolors()
{
    int intensity[SHADES];
    int colorindex,i;
    float cAmbient = 75;     // start the shades here -- provides ambient lighting

    // Do grayscale
    colorindex = 0;
    for(i=0; i<SHADES; i++)
    {
        intensity[i] = cAmbient+(255.-cAmbient)/SHADES*(i+1);
        colors[colorindex][i].setRgb(intensity[i],intensity[i],intensity[i],255);
    }
    // Do Red
    colorindex=1;
    for(i=0; i<SHADES; i++)
    {
        colors[colorindex][i].setRgb(intensity[i],0,0,255);
    }
    // Do Green
    colorindex=2;
    for(i=0; i<SHADES; i++)
    {
        colors[colorindex][i].setRgb(0,intensity[i],0,255);
    }
    // Do Blue
    colorindex=3;
    for(i=0; i<SHADES; i++)
    {
        colors[colorindex][i].setRgb(0,0,intensity[i],255);
    }

    return;
}
