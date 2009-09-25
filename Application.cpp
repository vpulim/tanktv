#include <stdio.h>
#include "Application.h"
#include "MenuItem.h"

Application::Application(Renderer *renderer)
  : m_renderer(renderer)
{
  m_x = 200;
  m_y = 0;
  m_widget = new MenuItem(0);
  m_widget->setRenderer(renderer);
}

void Application::run()
{
  if (m_renderer->initialized()) {
    m_renderer->loop(this);
  }
}

bool Application::handleEvent(Event &event)
{
  switch (event.key) {
  case KEY_STOP:
    return false;
  }
  return true;
}

bool Application::handleIdle()
{
  Box box;
  m_renderer->color(0,0,0,1);
  box = m_widget->box();
  box.move(box.x, box.y-20);
  box.resize(box.w+40, box.h+40);
  m_renderer->rect(box);
  
  m_y += 10;
  if (m_y > 800) m_y = 0;
  m_widget->move(m_x, m_y);
  m_widget->draw();
  m_renderer->flip();
  return true;
}
