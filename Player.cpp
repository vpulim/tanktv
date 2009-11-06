#include <stdio.h>
#include "Player.h"
#include "Application.h"

Player::Player(Application *application)
  : Screen(application)
{
}

Player::~Player()
{
}

bool Player::handleEvent(Event &event)
{
  debug("in Player::handleEvent: %d\n", event.key);
  
  Audio *audio = m_app->audio();

  switch (event.key) {
  case KEY_ENTER:
    if (audio->isPlaying()) audio->pause();
    else if (audio->isPaused()) audio->play();
    break;
  case KEY_LEFT:
    if (!audio->isStopped()) audio->rewind();
    break;
  case KEY_RIGHT:
    if (!audio->isStopped()) audio->forward();
    break;
  }

  return true;
}

bool Player::handleIdle()
{
  Renderer *r = m_app->renderer();
  Audio *a = m_app->audio();
  char time[10];

  r->color(0x0, 0x0, 0x0, 0x0);
  r->rect(0, 500, 700, 50);
  r->color(0xff, 0xff, 0xff, 0xff);
  r->font("fonts/LucidaSansRegular.ttf", 18);
  if (!a->isStopped()) {
    sprintf(time, "%d:%02d", a->elapsed() / 60, a->elapsed() % 60); 
    r->text(0, 500 + 45, time, 100);
    sprintf(time, "-%d:%02d", a->remaining() / 60, a->remaining() % 60); 
    r->text(300, 500 + 45, time, 100);
    r->flip();  
    if (a->remaining() == 0) {
      a->close();
      return false;
    }
  }

  return true;
}

void Player::paint()
{
  debug("in Player::paint\n");

  Renderer *r = m_app->renderer();
  r->color(0x0, 0x0, 0x0, 0xff);
  r->rect(m_screen_x, m_screen_y, m_box.w, m_box.h);
  r->flip();  
  r->color(0x0, 0x0, 0x0, 0xff);
  r->rect(m_screen_x, m_screen_y, m_box.w, m_box.h);
  r->flip();  
}
