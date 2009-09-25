#ifndef APPLICATION_H
#define APPLICATION_H

#include "Event.h"
#include "Widget.h"
#include "Renderer.h"

class Application : public EventListener
{
 private:
  Renderer *m_renderer;
  double m_x, m_y;
  Widget *m_widget;

 protected:
  bool handleEvent(Event &event);
  bool handleIdle();
  
 public: 
  Application(Renderer *renderer);
  void run();
};

#endif
