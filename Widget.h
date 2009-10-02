#ifndef WIDGET_H
#define WIDGET_H

#include "Event.h"
#include "Box.h"

#define MAX_LABEL_LENGTH 256

class Widget : public EventListener
{
 protected:
  Box m_box;
  char m_label[MAX_LABEL_LENGTH];
  Widget *m_parent;
  class Application *m_app;
  bool m_dirty;

 protected:
  double screen_x();
  double screen_y();

 public:
  Widget(Widget *parent);
  void setDirty(bool flag) { m_dirty = flag; }
  bool dirty() { return m_dirty; }
  Application *application() { return m_app; }
  virtual bool handleEvent(Event &event);
  virtual bool handleIdle();
  virtual void paint();
  virtual void move(double x, double y);
  virtual void resize(double w, double h);
  virtual const Box &box();
  virtual void setBox(const Box &box);
  virtual const char *label();
  virtual void setLabel(const char *label);
};

#endif 
