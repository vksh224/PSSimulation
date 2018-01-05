#ifndef SQUARE_H
#define SQUARE_H

#include "Point.h"

class Square
{
    public:
        //Square();
        void setSquare(Point _ll, double width);
        Point getLl();
        Point getLr();
        Point getUl();
        Point getUr();
    protected:
    private:
        Point ul;
        Point ur;
        Point ll;
        Point lr; //Lower Right

};

#endif // SQUARE_H
