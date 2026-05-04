#include "Minutiae.h"

Minutiae::Minutiae(int x, int y, Type t)
         : locX(x), locY(y), type(t)
{
    //ctor
}

Minutiae::~Minutiae()
{
    //dtor
}

int Minutiae::getLocX()
{
    return locX;
}

int Minutiae::getLocY()
{
    return locY;
}

Minutiae::Type Minutiae::getType()
{
    return type;
}
int Minutiae::distanceMean(int meanX,int meanY)
{
	return ((locX-meanX)^2+(locY-meanY)^2);
}