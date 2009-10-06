#include <string.h>
#include "Widget.h"
#include "Application.h"

Widget::Widget(Widget *parent)
  : m_parent(parent),
    m_app(0),
    m_dirty(true)
{
  if (parent) {
    m_app = parent->m_app;
  }
  move(0,0);
  resize(200,200);
  setLabel("");
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

void Widget::update_screen_xy()
{
  m_screen_x = m_screen_y = 0;
  Widget *w = this;
  while (w) {
    m_screen_x += w->m_box.x;
    m_screen_y += w->m_box.y;
    w = w->m_parent;
  }
}

void Widget::move(int x, int y)
{
  m_box.move(x,y);
  update_screen_xy();
}

void Widget::resize(int w, int h)
{
  m_box.resize(w,h);
}

void Widget::setBox(const Box &box) 
{
  m_box = box;
  update_screen_xy();
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
  r->color(0, 0, 0x8f, 0xff);
  r->rect(m_screen_x, m_screen_y, m_box.w, m_box.h);
  r->flip();
}
