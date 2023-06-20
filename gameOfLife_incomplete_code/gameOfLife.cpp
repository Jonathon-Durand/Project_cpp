// This file is loosely based on one supplied by Prof. Marshall Long for ENAS 130.

#include "moregraphicswindow.h"
#include "life.h"
#include <QApplication>
#include <QDesktopWidget>
#include <iostream>
#include <fstream>

#define FILENAMESIZE 256 // needed for filename

using namespace std;

// Global variables
QApplication *a2;
GraphicsWindow *w2;
int globalColor;    // color (for plotting live cells in graphics window)
int globalDelay_cs; // delay in hundredths of seconds (for pausing the display in the graphics window)
int globalCellSize; // each cell will be a square that's globalCellSize pixels wide by globalCellSize pixels high


int main(int argc, char *argv[])
{
    // Application and graphics initialization
    QApplication a(argc, argv); // QApplication is a class.
    GraphicsWindow w;           // GraphicsWindow is a class.
    a2 = &a;
    w2 = &w;

    // Declarations and initializations
    int genNumber=0, width, height, wrap;
    char answer;
    char inputFilename[FILENAMESIZE], outputFilename[FILENAMESIZE];
    Universe gen0, gen1, saveGen0;

    int speed;
    int maxDelay=50; // units are hundredths of seconds

    cout << "Enter an integer representing color,\n";
    cout << "where " << GREY  << "=grey, "
                     << RED   << "=red, "
                     << GREEN << "=green, and "
                     << BLUE  << "=blue:" << endl;
    cin >> globalColor;
    cout << "Enter an integer between 1 (slow) and 10 (fast):\n";
    cin >> speed;

    // Losing precision by dividing an integer by an integer doesn't matter here,
    // because the result gets stored in an integer anyway.
    globalDelay_cs = maxDelay/speed;

    cout << "Enter an integer between 10 (small) and 30 (large) for the cell size:\n";
    cin >> globalCellSize;

    // Establish the initial configuration of the universe, which may be read in
    // from a file or may be set up by the user via mouse interactions.
    cout << "Read in the universe setup from a file? (y/n)\n";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        cout << "Enter the name of the file (or drag the file here):\n";
        cin >> inputFilename;
        gen0 << inputFilename;
    }
    else
    {
        cout << "Enter the width of the universe:\n";
        cin >> width;
        cout << "Enter the height of the universe:\n";
        cin >> height;
        cout << "Will the universe wrap around? (y/n)\n";
        cin >> answer;
        if (answer == 'y' || answer == 'Y')
            wrap = 1;
        else
            wrap = 0;

        Universe blankUniverse(width, height, wrap);
        gen0 = blankUniverse;
    }

    gen1 = gen0; // shortcut to make gen1 have the same size and wrap setting as gen0

    // Regardless of whether the user did or did not read in a universe from a file,
    // allow the user to add/delete live cells.
    cout << "\nClick or drag the mouse in the window to enter live cells." << endl;
    cout << "Click a live cell to delete it." << endl;
    cout << "Right-click to start the simulation." << endl;
    gen0.setup();

    saveGen0 = gen0; // save init. config. in case the user wants to write it to a file later

    cout << "\nRight-click to stop the simulation." << endl;

    while (notRightClick())
    {
        if (genNumber%2) // if genNumber is odd...
            gen0 << gen1;
        else             // if genNumber is even...
            gen1 << gen0;

        genNumber++;
    }

    cout << "\nSimulation stopped after " << genNumber << " generations." << endl;

    cout << "\nSave the initial configuration to a file? (y/n)\n";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        cout << "Enter the name of the file:\n";
        cin >> outputFilename;
        saveGen0 >> outputFilename;

        cout << "\nInitial configuration written to file: " << outputFilename << endl;
    }

    return 0;
}
