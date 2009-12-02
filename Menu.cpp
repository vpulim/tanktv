#include <string.h>
#include "Menu.h"
#include "Utils.h"

Menu::Menu(Application *application, const char *title)
  : Screen(application),
    m_size(0),
    m_current(-1),
    m_top(160),
    m_idle_count(0)
{  
  setLabel(title);
}

Menu::~Menu()
{
  for (int i; i<m_size; i++) {
    delete m_menuItems[i];
  }
}

void Menu::add(MenuItem *menuItem)
{
  if (m_size < MAX_MENU_ITEMS) {
    menuItem->setIndex(m_size);
    m_menuItems[m_size++] = menuItem;
  }
  if (m_size == 1) m_current = 0;
}

bool Menu::handleEvent(Event &event)
{
  if (m_current > -1) {
    switch (event.key) {
    case KEY_UP: if (m_current > 0) m_current--; break;
    case KEY_DOWN: if (m_current < m_size - 1) m_current++; break;
    case KEY_PAGE_UP: 
      m_current -= 10; 
      if (m_current < 0) m_current = 0;
      break;
    case KEY_PAGE_DOWN: 
      m_current += 10; 
      if (m_current > m_size - 1) m_current = m_size - 1;
      break;
    case KEY_ENTER: if (m_current > -1) m_menuItems[m_current]->select(); break;
    }
    debug("current item: %s\n", m_menuItems[m_current]->label());
  }
  clearDirty();
  setDirtyRegion(Box(MENU_X, m_top, 445, m_box.h - m_top));
  m_idle_count=0;
  debug("handleEvent\n");
  return true;
}

bool Menu::handleIdle()
{
  int start, end;
  bool dirty = false;

  m_idle_count++;
  if (m_idle_count < 8) return false;
  if (m_idle_count == 8) {
    debug("handleIdle\n");
    setDirtyRegion(Box(0, 0, MENU_X - 60, m_box.h));    
    paint();
    m_app->renderer()->flip();
    setDirtyRegion(Box(0, 0, MENU_X - 60, m_box.h));    
    dirty = true;
  }

  getVisibleRange(&start, &end);        
    
  for (int i=start; i<=end; i++) {
    m_menuItems[i]->update();
    if (m_menuItems[i]->dirty()) dirty = true;
  }

  return dirty;
}

bool Menu::paintDetails(MenuItem *menuItem)
{
  return true;
}

void Menu::focusItem(MenuItem *menuItem)
{
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
    int x = MENU_X - 32, height = m_menuItems[0]->box().h;
    int y = m_top + (index - start) * height - 18;
    Renderer *r = m_app->renderer();

    if (index == m_current) {
      r->image(x, y, "images/menuitem_bg.png");  
    }
    else if (eraseOld) {
      r->color(0, 0, 0, 0xff);
      r->rect(MENU_X, y+18, 445, height);
    }
  }
}

void Menu::paint()
{
  if (!m_app) return;

  Renderer *r = m_app->renderer();
  int buffer = r->activeBuffer();
  Box dirtyBox = getDirtyRegion(buffer);

  int x = MENU_X;

  if (dirtyBox & Box(0, 0, x-60, m_box.h)) {
    r->color(0, 0, 0, 0xff);
    r->rect(0, 0, x - 60, m_box.h);      
    paintDetails(m_menuItems[m_current]);
  }

  if (dirtyBox & Box(x, 0, 445, m_top)) {
    r->color(0, 0, 0, 0xff);
    r->rect(x-60, 0, 445+120, m_top);  
    r->font(BOLD_FONT, 37);
    r->color(0xff, 0xff, 0xff, 0xff);
    r->text(x + 222, m_top - 40, m_label, 445, JUSTIFY_CENTER);
  }

  if (m_current > -1) {
    int start, end, y, height = m_menuItems[0]->box().h;

    getVisibleRange(&start, &end);        

    if (dirtyBox & Box(x-60, m_top, 445+120, m_box.h - m_top)) {
      r->color(0, 0, 0, 0xff);
      r->rect(x-60, m_top, 445+120, m_box.h - m_top);
      paintBackground(start, end, m_current, false);

      for (int i=start; i<=end; i++) {
        MenuItem *mi = m_menuItems[i];

        y = (i - start) * height;
        mi->move(x, m_top + y);
        mi->paint();      
        if (i-start == 0 && start > 0)
          r->image(x - 32, m_top - 18, "images/fade_top.png", true);  
        if (i-start == 9)
          r->image(x - 32, m_top + y - 18, "images/fade_bot.png", true);  
      }
    }
    else {
      bool paintCurrent = false;
      for (int i=start; i<=end; i++) {
        MenuItem *mi = m_menuItems[i];

        if (mi->dirty(buffer)) {
          paintBackground(start, end, i, true);
          if (i >= m_current-1 && i <= m_current+1)
            paintCurrent = true;
          else {
            mi->paint();      
          }
        }
      }
      if (paintCurrent) {
        paintBackground(start, end, m_current, true);      
        for (int i=m_current-1; i <= m_current+1; i++)
          if (i >= 0 && i < m_size) m_menuItems[i]->paint();
      }
    }
  }
  else {
    if (dirtyBox & Box(x-60, m_top, 445+120, m_box.h - m_top)) {
      r->color(0, 0, 0, 0xff);
      r->rect(x-60, m_top, 445+120, m_box.h - m_top);  
    }
  }

  clearDirty(buffer);
}
