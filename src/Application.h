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

#ifndef APPLICATION_H
#define APPLICATION_H

#include "Event.h"
#include "Screen.h"
#include "Renderer.h"
#include "Audio.h"
#include "Database.h"
#include "Indexer.h"
#include "NMTSettings.h"

#define MAX_STACK_SIZE 100

class Stack
{
 private:
  Screen *m_screens[MAX_STACK_SIZE];
  int m_top;

 public:
  Stack();
  ~Stack();
  bool push(Screen *screen);
  Screen *pop();
  Screen *top();
  int size() { return m_top + 1; }
  void cleanUp();
};

class Application : public EventListener
{
 private:
  Renderer *m_renderer;
  Audio *m_audio;
  Database *m_db;
  Indexer *m_indexer;
  Stack m_stack;
  NMTSettings * m_nmtSettings;

 protected:
  bool handleEvent(Event &event);
  bool handleIdle();

 public: 
  Application();
  ~Application();
  void startGUI(int argc, char **argv);
  void setScreen(Screen *screen);
  void go(Screen *screen);
  void back();
  void run();
  void exit();
  int parseCommandLine(int argc, char **argv);
  Renderer *renderer() { return m_renderer; }
  Audio *audio() { return m_audio; }
  Database *database() { return m_db; }
  Indexer *indexer() { return m_indexer; }
  NMTSettings * nmtSettings() { return m_nmtSettings; };
};

#endif
