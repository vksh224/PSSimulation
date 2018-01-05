#ifndef POINT_H
#define POINT_H


class Point
{
    public:
        //Point();
        //virtual ~Point();
        Point(double x = 0.0, double y = 0.0);
		double getX();
		double getY();
		void setX(double _xval);
		void setY(double _yval);
		double dist(Point other);
		void move(double a, double b);
		Point add(Point b);
		Point sub(Point b);
    protected:
    private:
        double xval, yval;
};

#endif // POINT_H
