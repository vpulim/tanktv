#include "Box.h"
#include "Utils.h"

Box::Box()
{
  move(0,0);
  resize(200,200);
}

Box::Box(int x, int y, int w, int h)
{
  move(x,y);
  resize(w,h);
}

void Box::move(int _x, int _y)
{
  x = _x;
  y = _y;
}

void Box::resize(int _w, int _h)
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
