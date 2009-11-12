#include <string.h>
#include "Menu.h"

MenuItem::MenuItem(Menu *menu, const char *label, MenuItemCallback callback, const char *image, int x, int y)
  : Widget(menu),
    m_cb(callback),
    m_index(-1),
    m_x(x),
    m_y(y),
    m_offset(0)
{
  setLabel(label);
  m_label_width = m_app->renderer()->textWidth(label);
  setDirty(m_label_width > MENUITEM_WIDTH);
  if (image && strlen(image) < sizeof m_image - 2) 
    strcpy(m_image, image);
  else
    m_image[0] = 0;
  resize(465, 50);
  menu->add(this);
}

bool MenuItem::hasFocus()
{
  return m_parent && m_index == ((Menu *)m_parent)->current();
}

void MenuItem::select()
{
  m_offset=0;
  if (m_cb)
    m_cb((Menu *)m_parent, this);
}

void MenuItem::paint()
{
  if (m_app) {
    Renderer *r = m_app->renderer();

    r->color(0xff, 0xff, 0xff, 0xff);
    if (!hasFocus() || !dirty())
      r->text(m_screen_x, m_screen_y + 35, m_label, MENUITEM_WIDTH);

    if (hasFocus()) {
      if (dirty()) {
	IDirectFBSurface *primary = r->surface();
	DFBRegion clip, textclip = {m_screen_x, m_screen_y-5, m_screen_x+MENUITEM_WIDTH, m_screen_y+45};
	int offset = 0;
	primary->GetClip(primary, &clip);
	primary->SetClip(primary, &textclip);
	if (m_offset > SCROLL_SPEED * SCROLL_DELAY) 
	  offset = m_offset - SCROLL_SPEED * SCROLL_DELAY;
	r->text(m_screen_x-offset, m_screen_y + 35, m_label, MENUITEM_WIDTH+offset, JUSTIFY_LEFT, true);
	r->text(m_screen_x-offset+m_label_width+60, m_screen_y + 35, m_label, MENUITEM_WIDTH+offset, JUSTIFY_LEFT, true);
	primary->SetClip(primary, &clip);
	if (offset > 0 && offset < m_label_width+30)
	  r->image(m_screen_x-14, m_screen_y+1, "images/fade_left.png", true);
	r->image(m_screen_x+MENUITEM_WIDTH-36, m_screen_y+1, "images/fade_right.png", true);
	m_offset += SCROLL_SPEED;
	if (m_offset >= m_label_width+60+SCROLL_SPEED * SCROLL_DELAY) m_offset = 0;
      }
      if (*m_image)
	r->image(m_x, m_y, m_image);
      else {
	r->color(0, 0, 0, 0xff);
 	r->rect(0, 0, 640, 720);
      }
    }
    else {
      m_offset=0;
    }
  }
}

ArrowMenuItem::ArrowMenuItem(Menu *menu, const char *label, MenuItemCallback callback, const char *image, int x, int y)
  : MenuItem(menu, label, callback, image, x, y)
{
}

void ArrowMenuItem::paint()
{
  if (m_app) {    
    MenuItem::paint();    
    m_app->renderer()->image(m_screen_x + m_box.w - 50, m_screen_y, 
			     hasFocus() ? "images/arrow_on.png" : "images/arrow.png",
			     true);
  }
}
