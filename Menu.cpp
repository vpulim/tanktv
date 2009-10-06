#include <string.h>
#include "Menu.h"
#include "Utils.h"

Menu::Menu(Application *application, const char *title)
  : Screen(application),
    m_size(0),
    m_current(-1),
    m_top(180)
{  
  setLabel(title);
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
    int x = 675, y, height = m_menuitems[0]->box().h;
    Renderer *r = m_app->renderer();

    r->color(0, 0, 0, 0xff);
    r->rect(640, 0, 640, 720);

    if (m_current > m_size - 5) {
      start = max(0, m_size - 9);
    }
    else if (m_current > 4) {
      start = m_current - 4;
    }
    end = min(m_size-1, start + 9);
    
    r->font("fonts/LucidaSansDemiboldRoman.ttf", 32);
    r->color(0xff, 0xff, 0xff, 0xff);
    r->text(x + 150, m_top - 40, m_label);

    r->image(x - 32, m_top + (m_current - start) * height - 18, "images/menuitem_bg.png");  

    r->font("fonts/LucidaSansDemiboldRoman.ttf", 28);
    for (int i=start; i<=end; i++) {
      MenuItem *mi = m_menuitems[i];
      y = (i - start) * height;
      mi->move(x, m_top + y);
      mi->paint();
      if (i-start == 0 && start > 0) {
	for (int j=0; j<50; j++) {
	  r->color(0, 0, 0, j > 35 ? 0xff : j*7.28);
	  r->line(x, m_top + y + (49 - j), x + 465, m_top + y + (49 - j), true);
	}
      }
      if (i-start == 9) {
	for (int j=0; j<50; j++) {
	  r->color(0, 0, 0, j > 35 ? 0xff : j*7.28);
	  r->line(x, m_top + y + j, x + 465, m_top + y + j, true);
	}
      }
    }

    r->flip();
  }

  setDirty(false);
}
