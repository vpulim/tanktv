#include <string.h>
#include "Menu.h"

MenuItem::MenuItem(const char *label, MenuItemCallback callback, const char *image)
  : Widget(NULL),
    m_cb(callback),
    m_index(-1)
{
  setLabel(label);
  if (image && strlen(image) < sizeof m_image - 2) 
    strcpy(m_image, image);
  else
    m_image[0] = 0;
  resize(465, 50);
}

bool MenuItem::hasFocus()
{
  return m_parent && m_index == ((Menu *)m_parent)->current();
}

void MenuItem::select()
{
  if (m_cb)
    m_cb((Menu *)m_parent, this);
}

void MenuItem::paint()
{
  if (m_app) {
    Renderer *r = m_app->renderer();

    r->color(0xff, 0xff, 0xff, 0xff);
    r->text(m_screen_x, m_screen_y + 35, m_label, 445);
    if (hasFocus()) {
      if (*m_image) {
	r->image(0, 0, m_image);
      }
      else {
	r->color(0, 0, 0, 0xff);
	r->rect(0, 0, 640, 720);
      }
    }
  }
}

ArrowMenuItem::ArrowMenuItem(const char *label, MenuItemCallback callback, const char *image)
  : MenuItem(label, callback, image)
{
}

void ArrowMenuItem::paint()
{
  if (m_app) {    
    m_app->renderer()->image(m_screen_x + m_box.w - 50, m_screen_y, 
			     hasFocus() ? "images/arrow_on.png" : "images/arrow.png",
			     true);
    MenuItem::paint();    
  }
}
