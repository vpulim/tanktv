#ifndef BOX_H
#define BOX_H

class Box 
{
 public:
  double x;
  double y;
  double w;
  double h;

 public:
  Box();
  Box(double x, double y, double w, double h);
  Box & operator= (const Box &other);
  void move(double x, double y);
  void resize(double w, double h);
  void clip(const Box &box);
};

#endif
