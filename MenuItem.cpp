#include <string.h>
#include "MenuItem.h"
#include "Player.h"
#include "Utils.h"

MenuItem::MenuItem(Menu *menu, const char *label, const void *data)
  : Widget(menu),
    m_index(-1),
    m_offset(0),
    m_menu(menu),
    m_data(data)
{
  setLabel(label);
  m_image_on[0] = m_image_off[0] = 0;
  m_label_width = m_app->renderer()->textWidth(label);
  m_scroll = m_label_width > MENUITEM_WIDTH;
  resize(465, 50);
  m_menu->add(this);
}

bool MenuItem::hasFocus()
{
  return m_parent && m_index == m_menu->current();
}

void MenuItem::select()
{
  m_offset=0;
  m_menu->selectItem(this);
}

void MenuItem::setImage(const char *image_on, const char *image_off)
{
  safe_strcpy(m_image_on, image_on);
  
  if (image_off[0]) {
    safe_strcpy(m_image_off, image_off);
  }
  else {
    safe_strcpy(m_image_off, m_image_on);
  }
}

void MenuItem::update()
{
  if (m_scroll) {
    if (hasFocus()) {
      m_offset += SCROLL_SPEED;
      if (m_offset >= m_label_width+60+SCROLL_SPEED * SCROLL_DELAY) m_offset = 0;
    }    
    else m_offset=0;
  }
  else m_offset=0;
  if (m_scroll && hasFocus()) {
    setDirty();
  }
}

void MenuItem::paint()
{
  if (!m_app) return;

  Renderer *r = m_app->renderer();
  int buffer = r->activeBuffer();

  r->font(BOLD_FONT, 29);
  r->color(0xff, 0xff, 0xff, 0xff);
  if (!hasFocus() || !m_scroll)
    r->text(m_screen_x, m_screen_y + 35, m_label, MENUITEM_WIDTH);

  if (hasFocus()) {
    if (m_scroll) {
      IDirectFBSurface *primary = r->surface();
      Box clip, textclip(m_screen_x, m_screen_y-5, MENUITEM_WIDTH, 50);
      int offset = 0;
      r->getClip(&clip);
      r->setClip(&textclip);
      if (m_offset > SCROLL_SPEED * SCROLL_DELAY) 
        offset = m_offset - SCROLL_SPEED * SCROLL_DELAY;
      r->text(m_screen_x-offset, m_screen_y + 35, m_label, MENUITEM_WIDTH+offset, JUSTIFY_LEFT, true);
      r->text(m_screen_x-offset+m_label_width+60, m_screen_y + 35, m_label, MENUITEM_WIDTH+offset, JUSTIFY_LEFT, true);
      r->setClip(&clip);
      if (offset > 0 && offset < m_label_width+30)
        r->image(m_screen_x-14, m_screen_y, "data/fade_left.png", true);
      r->image(m_screen_x+MENUITEM_WIDTH-36, m_screen_y, "data/fade_right.png", true);
    }
  }

  if (m_image_on[0] && m_image_off[0])
    m_app->renderer()->image(m_screen_x + m_box.w - 50, m_screen_y, 
                             hasFocus() ? m_image_on : m_image_off,
                             true);

  clearDirty(buffer);
}

FileItem::FileItem(Menu *menu, File *file)
  : MenuItem(menu, file->name()),
    m_file(file)
{
  if (file->isDirectory())
    setImage("data/arrow_on.png", "data/arrow.png");
}

void FileItem::select()
{
  Application *app = m_menu->application();
  
  if (m_file->isDirectory()) {
    app->go(new FileMenu(app, m_file->name(), m_file->path()));      
  }
  else {
    if (m_file->isVideo()) {
      if (!app->audio()->isStopped()) app->audio()->close();
      debug("playing %s\n", m_file->path());
      app->renderer()->play(m_file->path());
    }
    else if (m_file->isAudio()) {
      if (strcmp(app->audio()->nowPlaying(), m_file->path())) {
	debug("opening %s...\n", m_file->path());
	if (!m_app->audio()->open(m_file->path(), "", "", m_file->name(), "", 188)) 
	  return;
      }
      app->go(new Player(app));
    }
  }

  MenuItem::select();
}

ArrowItem::ArrowItem(Menu *menu, const char *label, const void *data)
  : MenuItem(menu, label, data)
{
  setImage("data/arrow_on.png", "data/arrow.png");
}

InfoItem::InfoItem(Menu *menu, const char *label, const char *info)
  : MenuItem(menu, label)
{
  setInfo(info);
}

void InfoItem::setInfo(const char *info, const char *label)
{
  if (label) setLabel(label);
  safe_strcpy(m_info, info);
  setDirty();
}

void InfoItem::paint()
{
  if (!m_app) return;

  Renderer *r = m_app->renderer();
  int buffer = r->activeBuffer();

  MenuItem::paint();

  r->font(REGULAR_FONT, 23);
  if (hasFocus())
    r->color(0xff, 0xff, 0xff, 0xff);
  else
    r->color(0x99, 0x99, 0x99, 0xff);
  r->text(m_screen_x + MENUITEM_WIDTH, m_screen_y + 35, m_info, 200, JUSTIFY_RIGHT);
  
  clearDirty(buffer);
}
