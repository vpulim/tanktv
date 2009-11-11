#include "Application.h"

Application::Application(Renderer *renderer, Audio *audio)
  : m_renderer(renderer),
    m_audio(audio)
{
  Renderer *r = renderer;
  r->color(0x80, 0x80, 0x80, 0xff);
  r->rect(0, 0, r->width(), r->height());
  r->image(560, 335, "images/logo.png", true);
  r->flip();  
  r->loadImage("images/arrow.png");
  r->loadImage("images/arrow_on.png");
  r->loadImage("images/fade_bot.png");
  r->loadImage("images/fade_top.png");
  r->loadImage("images/menuitem_bg.png");
  r->loadImage("images/position_bar.png");
  r->loadImage("images/position_knob.png");
  r->loadImage("images/movies.png");
  r->loadImage("images/tvshows.png");
  r->loadImage("images/music.png");
  r->loadImage("images/settings.png");
  r->loadImage("images/unknown_movie.png");
  r->loadImage("images/unknown_album.png");
  r->font("fonts/LucidaSansDemiboldRoman.ttf", 18);
  r->font("fonts/LucidaSansDemiboldRoman.ttf", 29);
  r->font("fonts/LucidaSansDemiboldRoman.ttf", 37);
  r->font("fonts/LucidaSansDemiboldRoman.ttf", 42);
  r->font("fonts/LucidaSansRegular.ttf", 29);
  r->color(0x0, 0x0, 0x0, 0xff);
  r->rect(0, 0, r->width(), r->height());
  r->flip();  
  r->rect(0, 0, r->width(), r->height());
  r->flip();    
}

void Application::setScreen(Screen *screen) 
{
  while (m_stack.top()) {
    delete m_stack.pop();
  }
  m_stack.push(screen);
  if (m_renderer->initialized()) {
    screen->paint();
    screen->setDirty(false);
  }
}

void Application::run()
{
  if (m_renderer->initialized()) {
    m_renderer->loop(this);
  }
}

void Application::go(Screen *screen) 
{ 
  m_stack.push(screen); 
}

void Application::back() 
{ 
  if (m_stack.size() > 1) delete m_stack.pop(); 
}

bool Application::handleEvent(Event &event)
{
  switch (event.key) {
  case KEY_BACK:
    back();
    break;
  }

  Screen *screen = m_stack.top();

  if (screen) {
    if (!screen->handleEvent(event)) back();
    screen = m_stack.top();
    if (screen && screen->dirty()) {
      screen->paint();
      screen->setDirty(false);
    }
  }

  return true;
}

bool Application::handleIdle()
{
  Screen *screen = m_stack.top();

  if (screen) {
    if (!screen->handleIdle()) back();
    screen = m_stack.top();
    if (screen && screen->dirty()) {
      screen->paint();
      screen->setDirty(false);
    }
  }

  return true;
}

Stack::Stack()
{
  m_top = -1;
}

bool Stack::push(Screen *screen)
{
  if (m_top < MAX_STACK_SIZE-1) {
    if (m_top > -1) m_screens[m_top]->setDirty(true);
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

