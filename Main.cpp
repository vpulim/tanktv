#include <signal.h>
#include "Application.h"
#include "Menu.h"
#include "File.h"

Application *application = 0;

void sigint_handler(int signal)
{
  if (application) 
    application->exit();
}

int main(int argc, char **argv)
{
  Renderer *renderer = new Renderer(argc, argv);
  Audio *audio = new Audio();
  Database *database = new Database();

  application = new Application(renderer, audio, database);
  application->setScreen(new MainMenu(application));

  signal(SIGINT, sigint_handler);
  application->run();

  delete database;
  delete renderer;
  delete audio;
}
