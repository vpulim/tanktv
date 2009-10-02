#include <string.h>
#include "Menu.h"
#include "Utils.h"

MenuItem::MenuItem(const char *label, MenuItemCallback callback, const void *data)
  : Widget(NULL),
    m_cb(callback),
    m_data(data),
    m_index(-1)
{
  setLabel(label);
  resize(465, 50);
}

void MenuItem::select()
{
  if (m_cb)
    m_cb((Menu *)m_parent, this);
}

void MenuItem::paint()
{
  if (m_app) {
    double x = screen_x();
    double y = screen_y();
    Renderer *r = m_app->renderer();

    r->color(1, 1, 1, 1);
    r->text(x, y + 37, m_label);  
  }
}

Menu::Menu(Application *application)
  : Screen(application),
    m_size(0),
    m_current(-1)
{  
}

Menu::~Menu()
{
  for (int i; m_size; i++) {
    delete m_menuitems[i];
  }
}

void Menu::add(MenuItem *menuItem)
{
  if (m_size < MAX_MENU_ITEMS) {
    menuItem->m_index = m_size;
    menuItem->m_parent = this;
    menuItem->m_app = m_app;
    m_menuitems[m_size++] = menuItem;
  }
  if (m_size == 1) m_current = 0;
}

bool Menu::handleEvent(Event &event)
{
  debug("in Main::handleEvent\n");
  switch (event.key) {
  case KEY_UP:
    if (m_current > 0) {
      m_current--; 
      setDirty(true);
    }
    break;
  case KEY_DOWN:
    if (m_current < m_size - 1) {
      m_current++;
      setDirty(true);
    }
    break;
  case KEY_ENTER:
    if (m_current > -1) {
      m_menuitems[m_current]->select();
    }
    break;
  }

  return true;
}

void Menu::paint()
{
  debug("in Main::paint\n");
  debug("m_current = %d\n", m_current);


  if (m_current > -1) {
    int start = 0, end = 0;
    double x = 675, y, top = 150, height = m_menuitems[0]->box().h;
    Renderer *r = m_app->renderer();

    r->color(0, 0, 0, 1.0);
    r->rect(640, 0, 640, 720);

    if (m_current > m_size - 6) {
      start = max(0, m_size - 10);
    }
    else if (m_current > 4) {
      start = m_current - 4;
    }
    end = min(m_size-1, start + 9);
    
    r->image(x - 32, top + (m_current - start) * height - 17, "images/menuitem_bg.png");  

    for (int i=start; i<=end; i++) {
      MenuItem *mi = m_menuitems[i];
      y = (i - start) * height;
      mi->move(x, top + y);
      mi->paint();
    }

    r->flip();
  }

  setDirty(false);
}
