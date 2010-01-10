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
  along with TankTV.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef BOX_H
#define BOX_H

class Box 
{
 public:
  int x;
  int y;
  int w;
  int h;

 public:
  Box();
  Box(int x, int y, int w, int h);
  void move(int x, int y);
  void resize(int w, int h);
  void clip(const Box &box);
};

Box operator+ (const Box &first, const Box &second);
bool operator& (const Box &first, const Box &second);
 
#endif
