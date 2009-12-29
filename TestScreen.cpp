#include "Screen.h"
#include "Application.h"

TestScreen::TestScreen(Application *application)
  : Screen(application)
{
}


void TestScreen::paint()
{
  Renderer *r = m_app->renderer();
}
