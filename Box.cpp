#include "Box.h"

#define min(x,y) ((x)<(y)?(x):(y))
#define max(x,y) ((x)>(y)?(x):(y))

Box::Box()
{
  move(0,0);
  resize(200,200);
}

Box::Box(double x, double y, double w, double h)
{
  move(x,y);
  resize(w,h);
}

void Box::move(double _x, double _y)
{
  x = _x;
  y = _y;
}

void Box::resize(double _w, double _h)
{
  w = _w;
  h = _h;
}

Box & Box::operator= (const Box &other)
{
  if (this != &other) {
    x = other.x;
    y = other.y;
    w = other.w;
    h = other.h;
  }
  return *this;
}

void Box::clip(const Box &box) 
{
  x = max(x, box.x);
  y = max(y, box.y);
  w = max(min(x+w, box.x+box.w) - x, 0);
  h = max(min(y+h, box.y+box.h) - y, 0);
}
