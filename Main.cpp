#include "Application.h"
#include "Menu.h"

int main(int argc, char **argv)
{
  Renderer *renderer = new Renderer(argc, argv);
  Audio *audio = new Audio();

  Application app(renderer, audio);
  app.setScreen(new MainMenu(&app));
  app.run();

  delete renderer;
  delete audio;
}
