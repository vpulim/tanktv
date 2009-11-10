#ifndef MENUITEM_H
#define MENUITEM_H

class MenuItem : public Widget
{
 protected:
  const void *m_data;
  MenuItemCallback m_cb;
  int m_index;
  char m_image[256];
  friend void Menu::add(MenuItem *menuItem);
  bool hasFocus();

 public:
  MenuItem(const char *label, MenuItemCallback callback=NULL, const char *image=NULL);
  void setCallback(MenuItemCallback callback);
  void select();
  int index() { return m_index; }
  virtual void paint();
};


class ArrowMenuItem : public MenuItem
{
 public:
  ArrowMenuItem(const char *label, MenuItemCallback callback=NULL, const char *image=NULL);
  virtual void paint();
};


#endif
