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
