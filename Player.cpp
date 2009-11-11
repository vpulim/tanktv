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
  int progress;

  r->color(0x0, 0x0, 0x0, 0xff);
  r->rect(570, 420, 620, 175);
  if (!a->isStopped()) {
    r->color(0xff, 0xff, 0xff, 0xff);
    r->font("fonts/LucidaSansDemiboldRoman.ttf", 37);
    if (a->title()) r->text(575, 455, a->title());

    r->color(0xad, 0xad, 0xad, 0xff);
    r->font("fonts/LucidaSansRegular.ttf", 29);
    if (a->artist()) r->text(575, 490, a->artist());
    if (a->album()) r->text(575, 525, a->album());

    r->color(0xff, 0xff, 0xff, 0xff);
    r->font("fonts/LucidaSansDemiboldRoman.ttf", 18);
    sprintf(time, "%d:%02d", a->elapsed() / 60, a->elapsed() % 60); 
    r->text(575, 590, time);
    sprintf(time, "-%d:%02d", a->remaining() / 60, a->remaining() % 60); 
    r->text(1120, 590, time);
    
    r->image(630, 573, "images/position_bar.png");
    progress = (int)(465 * ((float)a->elapsed())/(a->elapsed()+a->remaining()));
    r->color(0x80, 0x80, 0x80, 0xff);
    r->rect(633, 576, 6+progress, 10);
    r->image(633 + progress, 575, "images/position_knob.png", true);

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
  r->image(100, 92, "images/unknown_album.png");
  r->flip();  
  r->color(0x0, 0x0, 0x0, 0xff);
  r->rect(m_screen_x, m_screen_y, m_box.w, m_box.h);
  r->image(100, 92, "images/unknown_album.png");
  r->flip();  
}
