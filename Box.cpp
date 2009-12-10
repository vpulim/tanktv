/*
  Copyright (c) 2009 Vinay Pulim

  This file is part of TankTV.

  TankTV is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  TankTV is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

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

Box operator+ (const Box &first, const Box &second)
{  
  if (!first.w) return second;
  if (!second.w) return first;
  int x = min(first.x, second.x);
  int y = min(first.y, second.y);
  return Box(x, y, 
             max(first.x + first.w, second.x + second.w) - x,
             max(first.y + first.h, second.y + second.h) - y);
}

bool operator& (const Box &first, const Box &second)
{
  if (!first.w || !second.w) return false;
  if (first.x + first.w < second.x) return false;
  if (first.x > second.x + second.w) return false;
  if (first.y + first.h < second.y) return false;
  if (first.y > second.y + second.h) return false;
  return true;
}

void Box::clip(const Box &box) 
{
  x = max(x, box.x);
  y = max(y, box.y);
  w = max(min(x+w, box.x+box.w) - x, 0);
  h = max(min(y+h, box.y+box.h) - y, 0);
}
