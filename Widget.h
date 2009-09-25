#ifndef WIDGET_H
#define WIDGET_H

#include "Box.h"
#include "Renderer.h"

#define MAX_LABEL_LENGTH 256

class Widget 
{
 protected:
  Box m_box;
  char m_label[MAX_LABEL_LENGTH];
  Widget *m_parent;
  Renderer *m_renderer;

 public:
  virtual void draw() = 0;
  virtual void move(double x, double y);
  virtual void resize(double w, double h);

 public:
  Widget(Widget *parent);
  void setRenderer(Renderer *renderer);
  virtual const Box &box();
  virtual void setBox(const Box &box);
  virtual const char *label();
  virtual void setLabel(const char *label);
};

#endif 
