#include "Menu.h"

SettingsMenu::SettingsMenu(Application *application)
  : Menu(application, "NMT TV")
{
}

void SettingsMenu::paint()
{
  debug("in SettingsMenu::paint\n");

  Renderer *r = m_app->renderer();
  r->color(0x0, 0x0, 0x0, 0xff);
  r->rect(m_screen_x, m_screen_y, m_box.w, m_box.h);
  r->color(0xff, 0x00, 0x00, 0xff);
  r->font("fonts/LucidaSansRegular.ttf", 64);
  r->text(100, 100, "Hello World!");
  r->font("fonts/LucidaSansRegular.ttf", 36);
  r->text(100, 300, "Hello World!");
  r->font("fonts/LucidaSansRegular.ttf", 14);
  r->text(100, 500, "Hello World! 1234567890");
  r->font("fonts/LucidaSansDemiboldRoman.ttf", 28);
  r->text(575, 455, "Beautiful");
  r->font("fonts/LucidaSansRegular.ttf", 18);
  r->text(575, 490, "Artist Goes Here");
  r->flip();  
}
