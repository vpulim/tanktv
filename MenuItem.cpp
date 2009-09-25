#include "MenuItem.h"

MenuItem::MenuItem(Widget *parent)
  : Widget(parent)
{
  resize(MENU_ITEM_WIDTH, MENU_ITEM_HEIGHT);
}

void MenuItem::draw()
{
  m_renderer->color(0.5,0,0,1.0);
  m_renderer->image(m_box.x, m_box.y, "images/menuitem_bg.png");
}
