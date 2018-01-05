#include "Point.h"
#include <math.h>


// Class to represent points.

// Constructor uses default arguments to allow calling with zero, one,
// or two values.
Point:: Point(double x, double y) {
        xval = x;
        yval = y;
}

// Extractors.
double Point:: getX() {
    return xval;
}

double Point:: getY() {
    return yval;
}

void Point:: setX(double _xval){
    xval = _xval;
}

void Point:: setY(double _yval){
    yval = _yval;
}

// Distance to another point.  Pythagorean thm.
double Point:: dist(Point other) {
        double xd = xval - other.xval;
        double yd = yval - other.yval;
        return sqrt(xd*xd + yd*yd);
}

// Add or subtract two points.
Point Point:: add(Point b)
{
        return Point(xval + b.xval, yval + b.yval);
}
Point Point:: sub(Point b)
{
        return Point(xval - b.xval, yval - b.yval);
}

// Move the existing point.
void Point:: move(double a, double b)
{
        xval += a;
        yval += b;
}

