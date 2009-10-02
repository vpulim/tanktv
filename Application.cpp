#include "Application.h"

Application::Application(Renderer *renderer)
  : m_renderer(renderer)
{
}

void Application::setScreen(Screen *screen) 
{
  while (m_stack.top()) {
    delete m_stack.pop();
  }
  m_stack.push(screen);
  if (m_renderer->initialized()) {
    screen->paint();
  }
}

void Application::run()
{
  if (m_renderer->initialized()) {
    m_renderer->loop(this);
  }
}

Renderer *Application::renderer()
{
  return m_renderer;
}

bool Application::handleEvent(Event &event)
{
  switch (event.key) {
  case KEY_STOP:
    return false;
  }

  Screen *screen = m_stack.top();

  if (screen) {
    screen->handleEvent(event);
    screen = m_stack.top();
    if (screen && screen->dirty()) screen->paint();
  }

  return true;
}

bool Application::handleIdle()
{
  return true;
}

Stack::Stack()
{
  m_top = -1;
}

bool Stack::push(Screen *screen)
{
  if (m_top < MAX_STACK_SIZE-1) {
    m_screens[++m_top] = screen;
    return true;
  }
  return false;
}

Screen *Stack::pop() 
{
  if (m_top >= 0) {
    return m_screens[m_top--];
  }
  return NULL;
}

Screen *Stack::top() {
  if (m_top > -1) {
    return m_screens[m_top];
  }
  return NULL;
}

