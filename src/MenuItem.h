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

#ifndef MENUITEM_H
#define MENUITEM_H

#include "Menu.h"
#include "File.h"

#define MENUITEM_WIDTH 435
#define SCROLL_SPEED 8
#define SCROLL_DELAY 8

class MenuItem : public Widget
{
 protected:
  const void *m_data;
  int m_index;
  char m_image_on[256];
  char m_image_off[256];
  int m_x;
  int m_y;
  int m_offset;
  int m_label_width;
  Menu *m_menu;
  bool m_scroll;

 protected:
  bool hasFocus();

 public:
  MenuItem(Menu *menu, const char *label, const void *data=NULL);
  const void *data() { return m_data; }
  virtual void select();
  void setIndex(int i) { m_index = i; }
  int index() { return m_index; }
  void setImage(const char *image_on, const char *image_off="");
  virtual void update();
  virtual void paint();
};

class FileItem : public MenuItem
{
 private:
  File *m_file;

 public:
  FileItem(Menu *menu, File *file);
  virtual void select();
};

class ArrowItem : public MenuItem
{
 public:
  ArrowItem(Menu *menu, const char *label, const void *data=NULL);
};

class InfoItem : public MenuItem
{
 private:
  char m_info[256];
 public:
  InfoItem(Menu *menu, const char *label, const char *info="");  
  void setInfo(const char *info, const char *label=NULL);
  virtual void paint();
};

/*
class MusicItem : public ArrowItem
{
 public:
  MusicItem(Menu *menu, const char *label);
  virtual void select();
};

class GenreMusicItem : public ArrowItem
{
  int m_genre_id;
  char m_genre[128];
  
 public:
  GenreMusicItem(Menu *menu, int genre_id, const char *genre);
  virtual void select();
};

class ArtistMusicItem : public ArrowItem
{
  int m_artist_id;
  char m_artist[256];
  int m_genre_id;
  
 public:
  ArtistMusicItem(Menu *menu, int artist_id, const char *artist, int genre_id=0);
  virtual void select();
};

class AlbumMusicItem : public ArrowItem
{
  int m_album_id;
  char m_artist[256];
  char m_album[256];
  char m_genre[128];
  int m_tracks;
  int m_length;
  int m_genre_id;

 public:
  AlbumMusicItem(Menu *menu, int album_id, const char *artist, const char *album, const char *genre, int tracks, int length, int genre_id=0);
  virtual void select();
  virtual void update();
};

class SongMusicItem : public MenuItem
{
  char m_path[1024];
  char m_title[256];
  char m_artist[256];
  char m_album[256];
  char m_genre[128];
  int m_length;

 public:
  SongMusicItem(Menu *menu, const char *path, const char *artist, const char *album, const char *title, const char *genre, int length);
  virtual void select();
};
*/

#endif
