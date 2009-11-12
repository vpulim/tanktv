#include "Menu.h"

SettingsMenu::SettingsMenu(Application *application)
  : Menu(application, "NMT TV")
{
}

void SettingsMenu::paint()
{
  debug("in SettingsMenu::paint\n");

  Renderer *r = m_app->renderer();
  r->color(0, 0, 0, 0xff);
  r->rect(m_screen_x, m_screen_y, m_box.w, m_box.h);
  r->color(0xff, 0xff, 0xff, 0xff);
  r->font("fonts/LucidaSansDemiBold.ttf", 37); r->text(50, 100, "Beautiful");
  r->font("fonts/LucidaSansRegular.ttf", 29); r->text(350, 100, "Akon ft Kardinal Offishall Top 100");
  r->font("fonts/LucidaSansDemiBold.ttf", 18);    r->text(1000, 100, "1:23 4:56 7:89 0");
  r->font("fonts/bold.ttf", 37); r->text(50, 140, "Beautiful");
  r->font("fonts/regular.ttf", 29); r->text(350, 140, "Akon ft Kardinal Offishall Top 100");
  r->font("fonts/bold.ttf", 18);    r->text(1000, 140, "1:23 4:56 7:89 0");
  r->font("fonts/lucida_bold.ttf", 37); r->text(50, 180, "Beautiful");
  r->font("fonts/lucida.ttf", 29); r->text(350, 180, "Akon ft Kardinal Offishall Top 100");
  r->font("fonts/lucida_bold.ttf", 18);    r->text(1000, 180, "1:23 4:56 7:89 0");
  r->flip();  
}
