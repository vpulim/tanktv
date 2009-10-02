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
  Box & operator= (const Box &other);
  void move(int x, int y);
  void resize(int w, int h);
  void clip(const Box &box);
};

#endif
