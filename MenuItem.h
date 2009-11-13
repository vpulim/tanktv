#ifndef MENUITEM_H
#define MENUITEM_H

#define MENUITEM_WIDTH 435
#define SCROLL_SPEED 8
#define SCROLL_DELAY 8

class MenuItem : public Widget
{
 protected:
  const void *m_data;
  int m_index;
  char m_image[256];
  int m_x;
  int m_y;
  int m_offset;
  int m_label_width;
  Menu *m_menu;

 protected:
  bool hasFocus();

 public:
  MenuItem(Menu *menu, const char *label, const char *image=NULL, int x=0, int y=0);
  virtual void select();
  void setIndex(int i) { m_index = i; }
  int index() { return m_index; }
  virtual void paint();
};


class ArrowMenuItem : public MenuItem
{
 public:
  ArrowMenuItem(Menu *menu, const char *label, const char *image=NULL, int x=0, int y=0);
  virtual void paint();
};


#endif
