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
};

class Application : public EventListener
{
 private:
  Renderer *m_renderer;
  double m_x, m_y;
  Widget *m_widget;
  Stack m_stack;

 protected:
  bool handleEvent(Event &event);
  bool handleIdle();
  
 public: 
  Application(Renderer *renderer);
  void setScreen(Screen *screen);
  void run();
  Renderer *renderer();
};

#endif
