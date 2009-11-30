#include "Screen.h"
#include "Application.h"

Screen::Screen(Application *application)
  : Widget(NULL)
{
  m_app = application;
  resize(m_app->renderer()->width(), m_app->renderer()->height());
}
