#include "Application.h"

int main(int argc, char **argv)
{
  Renderer *renderer = new Renderer(argc, argv);

  Application app(renderer);
  app.run();

  delete renderer;
}
