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

#ifndef INDEXER_H
#define INDEXER_H

#include <pthread.h>
#include "Database.h"

class Indexer
{
 private:
  volatile int m_index_count;
  volatile bool m_indexing;  
  pthread_t m_thread;  

 private:
  static void *index_thread(void *arg);
  void index(const char *dir, Database *db);

 public:
  Indexer();
  ~Indexer();
  void start();
  void stop();
  bool isIndexing() { return m_indexing; }
  int indexCount() { return m_index_count; }
};

#endif
