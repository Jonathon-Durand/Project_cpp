// This file is based on one supplied by Prof. Marshall Long for ENAS 130.

#ifndef LIFE_H
#define LIFE_H

// Class declaration
class Universe
{
    private:
        int* array; // pointer to a width x height array of cells that constitutes the universe
                    // for the Game of Life.  A value of 1 signifies a live cell at that location.
        int width;  // width of the universe
        int height; // height of the universe
        int wrap;   // wrap = 0: Universe ends at the array boundaries
                    // wrap = 1: Universe wraps around on itself (has periodic boundaries),
                    //           meaning that the right boundary connects to the left boundary
                    //           and the top boundary connects to the bottom boundary
    public:
        Universe();                   // Default constructor: Sets array to nullptr, 
                                      //              and sets width, height, and wrap to 0.
        Universe(int, int, int);      // Constructor: Allocates space for a Universe of given width (first value)
                                      //              height (second value), and wrap setting (third value). 
                                      //              Sets all cells to 0.
        ~Universe();                  // Destructor:  Frees memory associated with array.
        void display();               // Displays the live cells in a graphics window.
                                      //              When display() is called the 1st time, it must open the graphics window.
                                      //              Subsequent calls to display() must NOT open the graphics window,
                                      //              because it's already open.
        void setup();                 // Displays the Universe, and then allows the user to modify the cell settings
                                      //              via mouse interactions.  When the user right-clicks the mouse,
                                      //              then the setup ends.
        void operator<<(char*);       // Reads in a Universe from a file.  The first, second, and third numbers in
                                      //              the file are width, height, and wrap setting.  After that
                                      //              are 1s/0s in a 2D integer array representing live/dead cells.
        void operator>>(char*);       // Writes a Universe to a file.  The format of the file is the same as above.
        void operator=(Universe&);    // Copies the contents of one Universe to another.
        void operator<<(Universe&);   // Calculates the new generation by applying the rules.  Then displays the
                                      //              new generation.
        int neighbors(int, int);      // Returns the number of live neighbors of the cell at row,col.
        int value(int, int);          // Returns the value (1 or 0) of the cell at row,col.
        void turnOn(int, int);        // Turns on the cell (makes it live) at row,col.
        void turnOff(int, int);       // Turns off the cell (makes it dead) at row,col.
};

#endif // LIFE_H
