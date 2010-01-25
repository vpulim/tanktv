/*
  Copyright (c) 2009 Vinay Pulim

  This file is part of TankTV.

  TankTV is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  TankTV is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with TankTV.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <signal.h>
#include "Application.h"
#include "Menu.h"

Application * application = NULL;

void sigint_handler(int signal)
{
  application->exit();
}

int main(int argc, char **argv)
{
  int status;

  try
  {
    fprintf(stdout, "*** %s version %s build at %s\n", APP_NAME, VERSION, __DATE__);

    application = new Application();
    signal(SIGINT, sigint_handler);

    // Parse command line options
    status = application->parseCommandLine(argc, argv);
    if (status)
    {
      application->startGUI(argc, argv);
      application->setScreen(new MainMenu(application));

      application->run();
    }
  }
  catch (std::exception& e)
  {
    fprintf(stderr, "%s\n", e.what());
  }

  if (NULL != application)
  {
    delete application;
    application = NULL;
  }

  fprintf(stdout, "*** %s: Exit\n", APP_NAME);

  return status;
}
