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

#include <taglib.h>
#include <tag.h>
#include <fileref.h>
#include "Indexer.h"
#include "Utils.h"
#include "File.h"

Indexer::Indexer()
  : m_index_count(0),
    m_indexing(0)
{
}

Indexer::~Indexer()
{
  stop();
}

void Indexer::start()
{
  if (!m_indexing) {
    m_indexing = true;
    m_index_count = 0;
    debug("started indexing\n");
    pthread_create(&m_thread, NULL, index_thread, this);
  }
}

void Indexer::stop()
{
  if (m_indexing) {
    debug("stopped indexing\n");
    m_indexing = false;
    if (m_thread) pthread_join(m_thread, 0);
    m_thread = 0;
  }
}

void Indexer::index(const char *dir, Database *db)
{
  std::vector<File> files;
  TagLib::FileRef *f;
  TagLib::Tag *tag;
  TagLib::AudioProperties *props;
  const char *path;
  const char *name;
  char *c;
  char artist[1024], album[1024], title[1024], genre[256];
  int length;
  
  debug("scanning: %s\n", dir);
  File::listDirectory(dir, files);

  m_index_count++;
  for (int i=0; i < files.size() && m_indexing; i++) {    
    debug("processing:   i = %d\n", i);
    path = files[i].path();
    name = files[i].name();
    debug("           path = %s\n", path);
    debug("           name = %s\n", name);
    if (files[i].isDirectory()) {
      index(path, db);
    }
    else if (files[i].isAudio()) {
      if (!db->execute("select rowid from songs where path=%Q", files[i].path())) {
	debug("parsing: %s\n", name);
        f = new TagLib::FileRef(path);
        debug("got f: %d\n", f);
        tag = f->tag();
	debug("ok\n");
        if (tag) {
          debug("processing: %s\n", name);
	  safe_strcpy(artist, tag->artist().isEmpty() ? "Unknown Artist" : tag->artist().to8Bit().c_str());
	  safe_strcpy(album, tag->album().isEmpty() ? "Unknown Album" : tag->album().to8Bit().c_str());
	  safe_strcpy(title, tag->title().to8Bit().c_str());
	  safe_strcpy(genre, tag->genre().isEmpty() ? "Unknown Genre" : tag->genre().to8Bit().c_str());
	  if (!title[0]) {
	    safe_strcpy(title, files[i].name());
	    if ((c = strrchr(title, '.'))) *c=0;
	  }
	  length = 0;
	  if ((props = f->audioProperties()))
	    length = props->length();
          debug("inserting: %s\n", name);
          int song_id = db->insertSong(path, title, album, artist, genre, length);
          debug("inserted song_id=%d\n", song_id);
          debug("done: %s\n", name);
	}
	debug("deleting f\n");
        delete f;
	debug("done deleting f\n");
      }
    }
  }  
}

void *Indexer::index_thread(void *arg)
{
  Indexer *indexer = (Indexer *)arg;
  Database db;
  indexer->index("/share/Music", &db);
  indexer->m_thread = 0;
  indexer->stop();
}
