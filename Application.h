#ifndef APPLICATION_H
#define APPLICATION_H

#include "Event.h"
#include "Screen.h"
#include "Renderer.h"
#include "Audio.h"
#include "Database.h"

#define MAX_STACK_SIZE 100

class Stack
{
 private:
  Screen *m_screens[MAX_STACK_SIZE];
  int m_top;

 public:
  Stack();
  ~Stack();
  bool push(Screen *screen);
  Screen *pop();
  Screen *top();
  int size() { return m_top + 1; }
  void cleanUp();
};

class Application : public EventListener
{
 private:
  Renderer *m_renderer;
  Audio *m_audio;
  Database *m_db;
  Stack m_stack;

 protected:
  bool handleEvent(Event &event);
  bool handleIdle();

 public: 
  Application(Renderer *renderer, Audio *audio, Database *database);
  void setScreen(Screen *screen);
  void go(Screen *screen);
  void back();
  void run();
  void exit();
  Renderer *renderer() { return m_renderer; }
  Audio *audio() { return m_audio; }
  Database *database() { return m_db; }
};

#endif
