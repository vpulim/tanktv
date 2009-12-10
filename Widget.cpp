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

#include <string.h>
#include "Widget.h"
#include "Application.h"
#include "Utils.h"

Widget::Widget(Widget *parent)
  : m_parent(parent),
    m_app(0)
{
  if (parent) {
    m_app = parent->m_app;
  }
  move(0,0);
  resize(200,200);
  setLabel("");
  setDirty();
}

bool Widget::handleEvent(Event &event)
{
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

Box Widget::getDirtyRegion(int buffer)
{
  if (buffer < 0) buffer = m_app ? m_app->renderer()->activeBuffer() : 0;
  return m_dirty[buffer];
}

bool Widget::dirty(int buffer)
{
  if (buffer < 0) buffer = m_app ? m_app->renderer()->activeBuffer() : 0;
  return m_dirty[buffer].w > 0;
}

void Widget::clearDirty(int buffer)
{
  if (buffer < 0) buffer = m_app ? m_app->renderer()->activeBuffer() : 0;
  m_dirty[buffer] = Box(0,0,0,0);  
}

void Widget::setDirtyRegion(Box region, int buffer)
{
  if (buffer < 0) {
    setDirtyRegion(region, 0);
    setDirtyRegion(region, 1);
  }
  else 
    m_dirty[buffer] = m_dirty[buffer] + region;
}

void Widget::setDirty(int buffer)
{
  if (buffer < 0) {
    setDirty(0);
    setDirty(1);
  }
  else
    m_dirty[buffer] = Box(m_box.x, m_box.y, m_box.w, m_box.h);
}

void Widget::move(int x, int y)
{
  if (m_box.x != x || m_box.y != y) {
    m_box.move(x,y);
    update_screen_xy();
    setDirty();
  }
}

void Widget::resize(int w, int h)
{
  if (m_box.w != w || m_box.h != h) {
    m_box.resize(w,h);
    setDirty();
  }
}

void Widget::setBox(const Box &box) 
{
  m_box = box;
  update_screen_xy();
}

void Widget::setLabel(const char *label)
{
  safe_strcpy(m_label, label);
}

const char *Widget::label()
{
  return m_label;
}

void Widget::paint()
{
  debug("in Widget::paint()\n");
  if (m_app) {
    Renderer *r = m_app->renderer();
    Box dirty = getDirtyRegion();
    r->color(0,0,0,0xff);
    r->rect(m_screen_x, m_screen_y, dirty.w, dirty.h);
  }
}
