#ifndef MENUITEM_H
#define MENUITEM_H

#define MENUITEM_WIDTH 435
#define SCROLL_SPEED 8
#define SCROLL_DELAY 8

class MenuItem : public Widget
{
 protected:
  const void *m_data;
  MenuItemCallback m_cb;
  int m_index;
  char m_image[256];
  int m_x;
  int m_y;
  int m_offset;
  int m_label_width;

 protected:
  friend void Menu::add(MenuItem *menuItem);
  bool hasFocus();

 public:
  MenuItem(Menu *menu, const char *label, MenuItemCallback callback=NULL, const char *image=NULL, int x=0, int y=0);
  void setCallback(MenuItemCallback callback);
  virtual void select();
  int index() { return m_index; }
  virtual void paint();
};


class ArrowMenuItem : public MenuItem
{
 public:
  ArrowMenuItem(Menu *menu, const char *label, MenuItemCallback callback=NULL, const char *image=NULL, int x=0, int y=0);
  virtual void paint();
};


#endif
