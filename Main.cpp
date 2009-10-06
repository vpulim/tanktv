#include "Application.h"
#include "Menu.h"

int main(int argc, char **argv)
{
  Renderer *renderer = new Renderer(argc, argv);

  Application app(renderer);
  app.setScreen(new MainMenu(&app));
  app.run();

  delete renderer;
}
