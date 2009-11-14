#include "Application.h"
#include "Menu.h"
#include "File.h"

int main(int argc, char **argv)
{
  File f("test.Mp3", "/share/Apps/nmti/test.Mp3");

  printf("ext: %s\n", f.extension());
  printf("name: %s\n", f.name());

  Renderer *renderer = new Renderer(argc, argv);
  Audio *audio = new Audio();

  Application app(renderer, audio);
  app.setScreen(new MainMenu(&app));
  app.run();

  delete renderer;
  delete audio;
}
