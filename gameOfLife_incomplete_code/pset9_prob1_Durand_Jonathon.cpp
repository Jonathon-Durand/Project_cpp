// This file was NOT supplied by Prof. Marshall Long.

#include "moregraphics.h"
#include "life.h"
#include <iostream>
#include <fstream>
#include <cstdlib> // needed for exit(1)

using namespace std;

#define INTENSITY 0.5
#define MAXWIDTHPIXELS 1439 // Put your screen width (in pixels) here.
#define MAXHEIGHTPIXELS 800 // Put your screen height (in pixels) here.
// You may use MAXWIDTHPIXELS and MAXHEIGHTPIXELS inside the body of your display function
// to calculate a value that will override the user input value for globalCellSize
// if the graphics window will be too large for your screen.  Adding this capability to your code
// is optional; you won't be penalized for NOT doing so, nor will you be rewarded for doing so.

extern int globalColor;    // color (for plotting live cells in graphics window)
extern int globalDelay_cs; // delay in hundredths of seconds (for pausing the display in the graphics window)
extern int globalCellSize; // each cell will be a square that's globalCellSize pixels wide by globalCellSize pixels high

//------------------------------------------------------------------------------
// Conway's Game of Life
//------------------------------------------------------------------------------

// Your code to implement the Universe class (declared in life.h) goes here.
// Also change the values of MAXWIDTHPIXELS and MAXHEIGHTPIXELS above, if you would like to.
// (See the comment that appears below their #define statements.)

// Don't make changes to any other file.  You will only submit your version of life.cpp.

//Constructor for an empty universe
Universe :: Universe()
{
    //Assigns the array to null and sets all other variables in the objeect to 0
    array = nullptr;
    width = 0;
    height = 0;
    wrap = 0;
}

//Constructor for user inputted values to create an empty object
Universe :: Universe(int userWidth, int userHeight, int userWrap)
{
    //Sets variables to the user's inputs
    width = userWidth;
    height = userHeight;
    wrap = userWrap;

    //Allocate memory for an array
    array = new int [width * height];

    //Read through the entire array and input 0 for every value
    for(int i = 0; i < (width * height); i++)
    {
        array[i] = 0;
    }
}

//Destructor
Universe :: ~Universe()
{
    //If the array is not already null...
    if(array != nullptr)
    {
        //then delete the values and set it to null
        delete[] array;
        array = nullptr;
    }
}
//Member function that will display the image to the user
void Universe :: display()
{
    //Create a static int so that we can make a first time check
    static int firstTime = 1;
    //Define a row and column variable
    int r, c;

    //Check to see if this is the first time that display has been called
    //If it is, decrement the firstTime variable and then open the graphics window
    if(firstTime == 1)
    {
        firstTime--;

        //Window should be proportional to the height and width of the object
        //but also to the size of the cells
        ploton((height * globalCellSize), (width * globalCellSize));
        scalx(0, width);
        scaly(height, 0);
    }
    //Clear the window so that a new image can be shown
    clear();

    //Any time that the value of a cell is 1 (alive) in our array of cell values
    //we want to draw a rectangle of specified color
    for(int i = 0; i < (width * height); i++)
    {
        r = i / width;
        c = i % width;

        if(value(r, c) == 1)
        {
            drawRect(c, r, c+1, r+1, globalColor, INTENSITY);
        }

    }

    //Updates any new drawings to the display window
    updateImage();

}
//Member functioonn allows the user to set up the initial image exactly how they
//want to by using the mouse to add or take away cells.
void Universe :: setup()
{
    //Define the variables needed by the getmouse as well as row and column
    float x = 0., y = 0.;
    int drag , r, c;

    //Display the current image
    display();

    //While not right clicking...
    while(getmouse(&x, &y, &drag) != RIGHT_BUTTON)
    {
        //Set the x location of the mouse to the column and the y location to the row
        c = x;
        r = y;

        //If drag is on cells can only be born not killed
        //If it is off dead cells can be clicked to become alive and vise versa
        if(drag == 1)
            turnOn(r, c);
        else
        {
            if(value(r, c) == 1)
                turnOff(r, c);
            else
                turnOn(r, c);

        }
        //Display the new image
        display();
    }
}

//Member Function that reads an  input file
void Universe :: operator<<(char* fileName)
{
    ifstream inputFile;
    //Open the input file
    inputFile.open(fileName);
    //Check to make sur its not null
    if(inputFile.fail())
    {
        cout << "Cannot open the " << fileName << " file" << endl;
        exit(1);
    }

    //Read in the first three numbers which correspond to width, height, and wrap
    inputFile >> width >> height >> wrap;

    //Allocate space for a new array
    array = new int [width * height];
    //If the array is null then don't allocate
    if(array == nullptr)
    {
        cout << "Memory cannot be allocated for this array.";
        exit(1);
    }

    //Read through the entire array
    for(int i=0; i < (width * height); i++)
    {
        //If at end of file then break out of loop
        if (inputFile.fail())
            break;
        //input the values in the file to the array at every index
        inputFile >> array[i];
    }

    //Close the file
    inputFile.close();

}

//Member function to create an output file
void Universe :: operator>>(char* storingFilename)
{
    ofstream outputFile;
    //Open output file
    outputFile.open(storingFilename);

    //Output width, height, and wrap all to their own lines
    outputFile << width << endl;
    outputFile << height << endl;
    outputFile << wrap << endl;

    //Read through the entire array and take the value at every row and column
    //space to create a matrix of dimensions height x width
    for(int row = 0; row < height; row++)
    {
        for(int column = 0; column < width; column++)
        {
            outputFile << value(row, column) << " ";
        }

        outputFile << endl;
    }

    //Close the file
    outputFile.close();
}

//Member function makes a copy of a universe
void Universe :: operator=(Universe& copy)
{
    //The variables must all equal each other
    width = copy.width;
    height = copy.height;
    wrap = copy.wrap;

    //Allocate memory for a new array
    array = new int [width * height];
    //If the array is null then don't allocate
    if(array == nullptr)
    {
        cout << "Memory cannot be allocated for this array.";
        exit(1);
    }

    //Read through each index of one array and set its value to the index of the other
    for(int k = 0; k < (copy.width * copy.height); k++)
    {
        array[k] = copy.array[k];
    }
}

//Member function applies the rules of cell life
void Universe :: operator<<(Universe& oldGen)
{
    //Define variables for row, column, and one to hold the number of neighbors
    int c, r, neighborValues;
    //Loop through the array, find the corresponding row and column values for
    //each index.
    for(int i = 0; i < (oldGen.width * oldGen.height); i++)
    {
        r = i / oldGen.width;
        c = i % oldGen.width;
        //Find the number of neighbors at each cell location
        neighborValues = oldGen.neighbors(r, c);

        //If the cell is dead, check to see if it has exactly three neighbors
        //if it does then turn it on, if not keep it dead
        if(oldGen.value(r, c) == 0)
        {
            if(neighborValues == 3)
                turnOn(r, c);
            else
                turnOff(r, c);
        }

        //If the cell is alive, check to see if it has less than 2 or more than 3
        //neighbors. If it does, kill it, if not, keep it alive.
        if(oldGen.value(r, c) == 1)
        {
            if(neighborValues < 2 || neighborValues > 3)
                turnOff(r, c);
            else
                turnOn(r, c);
        }


    }

    //Show the changes made and delay so the user has time to view.
    display();
    delay(globalDelay_cs);

}
//Member function determines the amount of live neighbors a cell has
int Universe :: neighbors(int row, int column)
{
    //Initialize live neighbors to zero
    int liveNeighbors = 0;

    //For wrap being zero the boundaries are set so just the cells that are at the
    //8 points of contact or those that are inside of the boundaries of the matrix
    //will be looked at for neighbors
    if(wrap == 0)
    {
        if((column - 1) >= 0)
        {
            liveNeighbors = liveNeighbors + value(row, (column - 1));

            if((row - 1) >= 0)
                liveNeighbors = liveNeighbors + value((row - 1), (column - 1));
            if((row + 1) < height)
                liveNeighbors = liveNeighbors + value((row + 1), (column - 1));
        }

        if((row - 1) >= 0)
            liveNeighbors = liveNeighbors + value((row - 1), column);

        if((row + 1) < height)
            liveNeighbors = liveNeighbors + value((row + 1), column);

        if((column + 1) < width)
        {
            liveNeighbors = liveNeighbors + value(row, (column + 1));

            if((row - 1) >= 0)
                liveNeighbors = liveNeighbors + value((row - 1), (column + 1));

            if((row + 1) < height)
                liveNeighbors = liveNeighbors + value((row + 1), (column + 1));

        }
    }
    //For wrap being one the boundaries wrap around so there are always 8 points
    //of contact for any cell in the universe. There have to be additional checks
    //to guarantee that the cells can make around the wrap
    else
    {
        if((column - 1) >= 0)
        {

            liveNeighbors = liveNeighbors + value(row, (column - 1));

            if((row - 1) >= 0)
                liveNeighbors = liveNeighbors + value((row - 1), (column - 1));
            else
                liveNeighbors = liveNeighbors + value((height - 1), (column - 1));

            if((row + 1) < height)
                liveNeighbors = liveNeighbors + value((row + 1), (column - 1));
            else
                liveNeighbors = liveNeighbors + value(0, (column - 1));
        }
        else
        {
            liveNeighbors = liveNeighbors + value(row, (width - 1));

            if((row - 1) >= 0)
                liveNeighbors = liveNeighbors + value((row - 1), (width - 1));
            else
                liveNeighbors = liveNeighbors + value((height - 1), (width - 1));

            if((row + 1) < height)
                liveNeighbors = liveNeighbors + value((row + 1), (width - 1));
            else
                liveNeighbors = liveNeighbors + value(0, (width - 1));
        }

        if((row - 1) >= 0)
            liveNeighbors = liveNeighbors + value((row - 1), column);
        else
            liveNeighbors = liveNeighbors + value((height - 1), column);

        if((row + 1) < height)
            liveNeighbors = liveNeighbors + value((row + 1), column);
        else
            liveNeighbors = liveNeighbors + value(0, column);

        if((column + 1) < width)
        {
            liveNeighbors = liveNeighbors + value(row, (column + 1));

            if((row - 1) >= 0)
                liveNeighbors = liveNeighbors + value((row - 1), (column + 1));
            else
                liveNeighbors = liveNeighbors + value((height - 1), (column + 1));

            if((row + 1) < height)
                liveNeighbors = liveNeighbors + value((row + 1), (column + 1));
            else
                liveNeighbors = liveNeighbors + value(0, (column + 1));
        }
        else
        {
            liveNeighbors = liveNeighbors + value(row, 0);

            if((row - 1) >= 0)
                liveNeighbors = liveNeighbors + value((row - 1), 0);
            else
                liveNeighbors = liveNeighbors + value((height - 1), 0);

            if((row + 1) < height)
                liveNeighbors = liveNeighbors + value((row + 1), 0);
            else
                liveNeighbors = liveNeighbors + value(0, 0);
        }

    }

    //return the number of live cells at the end
    return liveNeighbors;
}

//Member function gets the value of any cell at a specified row, column location
int Universe :: value(int row, int column)
{
    //Find the array index using a formula with row and column
    int index = (row * width) + column;
    int state = array[index];

    return state;
}
//Member function takes a cell at row, column and makes it live
void Universe :: turnOn(int row, int column)
{
    int index = (row * width) + column;
    array[index] = 1;
}

//Member function takes a cell at row, column and makes it dead
void Universe :: turnOff(int row, int column)
{
    int index = (row * width) + column;
    array[index] = 0;
}
