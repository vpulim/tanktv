#include <signal.h>
#include "Application.h"
#include "Menu.h"

Application *application;

void sigint_handler(int signal)
{
  application->exit();
}

int main(int argc, char **argv)
{
  application = new Application(argc, argv);
  application->setScreen(new MainMenu(application));
  signal(SIGINT, sigint_handler);

  application->run();

  delete application;
}
