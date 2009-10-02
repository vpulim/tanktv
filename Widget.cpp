#include <string.h>
#include "Widget.h"
#include "Application.h"

Widget::Widget(Widget *parent)
  : m_parent(parent),
    m_app(0),
    m_dirty(true)
{
  if (parent) {
    m_box = parent->box();
    m_app = parent->m_app;
  }
  else {
    m_box = Box(0, 0, 200, 200);
  }
}

bool Widget::handleEvent(Event &event)
{
  debug("in Widget::handleEvent()\n");
  return true;
}

bool Widget::handleIdle() 
{
  return true;
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

void Widget::paint()
{
  debug("in Widget::paint()\n");
  Renderer *r = m_app->renderer();
  r->color(0.0, 0.0, 0.5, 1.0);
  r->rect(m_box.x, m_box.y, m_box.w, m_box.h);
  r->flip();
}

double Widget::screen_x()
{
  double x = 0.0;
  Widget *w = this;
  while (w) {
    x += w->m_box.x;
    w = w->m_parent;
  }
  return x;
}

double Widget::screen_y()
{
  double y = 0.0;
  Widget *w = this;
  while (w) {
    y += w->m_box.y;
    w = w->m_parent;
  }
  return y;
}
