#include <string.h>
#include "Widget.h"

Widget::Widget(Widget *parent)
  : m_parent(parent),
    m_renderer(0)
{
  if (parent) {
    m_box = parent->box();
    m_renderer = parent->m_renderer;
  }
  else {
    m_box = Box(0, 0, 200, 200);
  }
}

void Widget::setRenderer(Renderer *renderer)
{
  m_renderer = renderer;
}

const Box &Widget::box() 
{
  return m_box;
}

void Widget::move(double x, double y)
{
  m_box.move(x,y);
}

void Widget::resize(double w, double h)
{
  m_box.resize(w,h);
}

void Widget::setBox(const Box &box) 
{
  m_box = box;
}

void Widget::setLabel(const char *label)
{
  strncpy(m_label, label, sizeof m_label);
  m_label[sizeof m_label - 1] = 0;
}

const char *Widget::label()
{
  return m_label;
}
