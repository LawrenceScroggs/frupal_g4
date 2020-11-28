#include "item.h"


////////////////////////////////////////////////////////////////////
//                        Item functions
////////////////////////////////////////////////////////////////////

Item::Item(): whiffles(0), isOwned(0) {}

Item::~Item()
{ 
    whiffles = 0;
    isOwned = 0;
}


////////////////////////////////////////////////////////////////////
//                      Treasure   functions
////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
//                       Tool  functions
////////////////////////////////////////////////////////////////////


Tools::Tools(): rating(0) {}

Tools::~Tools()
{
    rating = 0;
}


//initializes a tool
void Tools::init(int i)
{
    //create Axe
    if( i == 0)
    {
        rating = 2;
        whiffles = 25;
        name = "Axe";
    }

    //create Hammer
    else if( i == 1)
    {
        rating = 3;
        whiffles = 40;
        name = "Hammer";
    }
}


////////////////////////////////////////////////////////////////////
//                      Food   functions
////////////////////////////////////////////////////////////////////
