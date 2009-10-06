#ifndef APPLICATION_H
#define APPLICATION_H

#include "Event.h"
#include "Screen.h"
#include "Renderer.h"

#define MAX_STACK_SIZE 100

class Stack
{
 private:
  Screen *m_screens[MAX_STACK_SIZE];
  int m_top;

 public:
  Stack();
  bool push(Screen *screen);
  Screen *pop();
  Screen *top();
  int size() { return m_top + 1; }
};

class Application : public EventListener
{
 private:
  Renderer *m_renderer;
  Stack m_stack;

 protected:
  bool handleEvent(Event &event);
  bool handleIdle();
  
 public: 
  Application(Renderer *renderer);
  void setScreen(Screen *screen);
  void go(Screen *screen) { m_stack.push(screen); }
  void back() { if (m_stack.size() > 1) delete m_stack.pop(); }
  void run();
  Renderer *renderer();
};

#endif
