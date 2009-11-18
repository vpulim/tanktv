#include <string.h>
#include "Menu.h"
#include "Utils.h"

Menu::Menu(Application *application, const char *title)
  : Screen(application),
    m_size(0),
    m_current(-1),
    m_top(180),
    m_dirty_back_buffer(true),
    m_dirty_details(true)
{  
  setLabel(title);
}

Menu::~Menu()
{
  for (int i; i<m_size; i++) {
    delete m_menuitems[i];
  }
}

void Menu::add(MenuItem *menuItem)
{
  if (m_size < MAX_MENU_ITEMS) {
    menuItem->setIndex(m_size);
    m_menuitems[m_size++] = menuItem;
  }
  if (m_size == 1) m_current = 0;
}

bool Menu::handleEvent(Event &event)
{
  debug("in Menu::handleEvent: %d\n", event.key);
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
      setDirty(true);
    }
    break;
  }
  if (m_current > -1) debug("current item: %s\n", m_menuitems[m_current]->label());
  m_dirty_back_buffer = true;
  m_dirty_details = true;
  return true;
}

bool Menu::handleIdle()
{
  if (m_dirty_details) {
    m_dirty_details = !paintDetails();
  }

  if (m_dirty_back_buffer) {
    paint();
    m_dirty_back_buffer = false;
  }

  if (m_current > -1) {
    int start, end;
    bool flip = false, paintCurrent = false;
    getVisibleRange(&start, &end);        
    for (int i=start; i<=end; i++) {
      MenuItem *mi = m_menuitems[i];
      if (mi->handleIdle()) {
	paintBackground(start, end, i, true);
	if (i >= m_current-1 && i <= m_current+1)
	  paintCurrent = true;
	else
	  mi->paint();
	flip = true;
      }
    }
    if (paintCurrent) {
      paintBackground(start, end, m_current, true);      
      for (int i=m_current-1; i <= m_current+1; i++)
	if (i >= 0 && i < m_size) m_menuitems[i]->paint();
    }
    if (flip) m_app->renderer()->flip();
  }

  return true;
}

bool Menu::paintDetails()
{
  return true;
}

void Menu::selectItem(MenuItem *menuItem)
{
}

void Menu::getVisibleRange(int *start, int *end)
{
  *start = 0;
  if (m_current > m_size - 5) {
    *start = max(0, m_size - 9);
  }
  else if (m_current > 4) {
    *start = m_current - 4;
  }
  *end = min(m_size-1, *start + 9);
}

void Menu::paintBackground(int start, int end, int index, bool eraseOld)
{
  if (index >= start && index <= end) {
    int x = MENU_X, height = m_menuitems[0]->box().h;
    Renderer *r = m_app->renderer();

    if (index == m_current) {
      r->image(x - 32, m_top + (index - start) * height - 18, "images/menuitem_bg.png");  
    }
    else if (eraseOld) {
      r->color(0, 0, 0, 0xff);
      r->rect(x - 32, m_top + (index - start) * height - 18, 528, 85);
    }
  }
}

void Menu::paint()
{
  int x = MENU_X;
  Renderer *r = m_app->renderer();
    
  r->color(0, 0, 0, 0xff);
  r->rect(0, 0, r->width(), r->height());  
  
  r->font(BOLD_FONT, 37);
  r->color(0xff, 0xff, 0xff, 0xff);
  r->text(x + 222, m_top - 40, m_label, 445, JUSTIFY_CENTER);

  if (m_current > -1) {

    int start, end, y, height = m_menuitems[0]->box().h;

    getVisibleRange(&start, &end);        

    paintBackground(start, end, m_current);

    for (int i=start; i<=end; i++) {
      MenuItem *mi = m_menuitems[i];

      y = (i - start) * height;
      mi->move(x, m_top + y);
      mi->paint();
      if (i-start == 0 && start > 0) {
	r->image(x - 32, m_top - 18, "images/fade_top.png", true);  
      }
      if (i-start == 9) {
	r->image(x - 32, m_top + y - 18, "images/fade_bot.png", true);  
      }
    }
  }
  r->flip();
}
