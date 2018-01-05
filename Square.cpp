#include "Square.h"

/*Square :: Square(){
    cout <<"Constructor";
}*/

void Square:: setSquare(Point _ll, double width)
{
    //set ll
    ll = _ll;

    //set lr
    lr.setX(_ll.getX() + width);
    lr.setY(_ll.getY());

    //set ul
    ul.setX(_ll.getX());
    ul.setY(_ll.getY()+width);

    //set ur
    ur.setX(_ll.getX() + width);
    ur.setY(_ll.getY() + width);
}

Point Square:: getLl(){
    return ll;
}

Point Square :: getLr(){
    return lr;
}

Point Square:: getUl(){
    return ul;
}

Point Square:: getUr(){
    return ur;
}
