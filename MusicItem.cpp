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
  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include "Menu.h"
#include "Player.h"
#include "Utils.h"

MusicItem::MusicItem(Menu *menu, const char *label)
  : ArrowItem(menu, label)
{
}

void MusicItem::select()
{
  Application *app = m_menu->application();
  Database *db = app->database();
  Result *result;
  Menu *menu;

  if (!strcmp(m_label, "Music")) {
    menu = new Menu(app, "Music");
    new MusicItem(menu, "Artists");
    new MusicItem(menu, "Albums");
    new MusicItem(menu, "Songs");
    new MusicItem(menu, "Genres");
    m_app->go(menu);
  }
  else if (!strcmp(m_label, "Artists")) {
    menu = new Menu(app, "Artists");
    if (db->execute("select rowid, artist from artists order by artist"))
      while ((result=db->next()))
	new ArtistMusicItem(menu, 
			    strtol((*result)["rowid"], NULL, 10), 
			    (*result)["artist"]);
    m_app->go(menu);
  }
  else if (!strcmp(m_label, "Albums")) {
    menu = new Menu(app, "Albums");
    if (db->execute("select count(1) as tracks, sum(length) as length, genre, artist, album from genres, albums, artists, songs where genres.rowid=genre_id and artists.rowid=artist_id and albums.rowid=album_id group by album order by album"))
      while ((result=db->next()))
	new AlbumMusicItem(menu, 
			   0,
			   (*result)["artist"], 
			   (*result)["album"], 
			   (*result)["genre"], 
			   strtol((*result)["tracks"], NULL, 10),
			   strtol((*result)["length"], NULL, 10));
    m_app->go(menu);
  }
  else if (!strcmp(m_label, "Songs")) {
    menu = new Menu(app, "Songs");
    if (db->execute("select path, title, artist, album, genre, length from songs, albums, artists, genres where artists.rowid=artist_id and albums.rowid=album_id and genres.rowid=genre_id order by title"))
      while ((result=db->next()))
	new SongMusicItem(menu, 
			  (*result)["path"], 
			  (*result)["artist"], 
			  (*result)["album"], 
			  (*result)["title"], 
			  (*result)["genre"],
			  strtol((*result)["length"], NULL, 10));
    m_app->go(menu);
  }
  else if (!strcmp(m_label, "Genres")) {
    menu = new Menu(app, "Genres");
    if (db->execute("select rowid, genre from genres order by genre"))
      while ((result=db->next()))
	new GenreMusicItem(menu, 
			   strtol((*result)["rowid"], NULL, 10),
			   (*result)["genre"]);
    m_app->go(menu);
  }
}

GenreMusicItem::GenreMusicItem(Menu *menu, int genre_id, const char *genre)
  : ArrowItem(menu, genre), m_genre_id(genre_id) 
{
  safe_strcpy(m_genre, genre);
}

void GenreMusicItem::select()
{
  Menu *menu = new Menu(m_app, m_genre);
  Database *db = m_app->database();
  Result *result;
  if (db->execute("select artist_id, artist from artists, albums, songs where artists.rowid=artist_id and albums.rowid=album_id and genre_id=%d group by artist_id order by artist", m_genre_id))
    while ((result=db->next()))
      new ArtistMusicItem(menu, 
			  strtol((*result)["artist_id"], NULL, 10), 
			  (*result)["artist"],
			  m_genre_id);
  m_app->go(menu);  
}

ArtistMusicItem::ArtistMusicItem(Menu *menu, int artist_id, const char *artist, int genre_id) 
  : ArrowItem(menu, artist), m_artist_id(artist_id), m_genre_id(genre_id) 
{
  safe_strcpy(m_artist, artist);
}

void ArtistMusicItem::select()
{
  char genre_sql[256] = "";

  if (m_genre_id) sprintf(genre_sql, "and genre_id=%d", m_genre_id);

  Menu *menu = new Menu(m_app, m_artist);
  Database *db = m_app->database();
  Result *result;
  if (db->execute("select albums.rowid, count(1) as tracks, sum(length) as length, genre, artist, album from genres, albums, artists, songs where genres.rowid=genre_id and artists.rowid=artist_id and albums.rowid=album_id and artist_id=%d %s group by album_id order by album", m_artist_id, genre_sql))
    while ((result=db->next()))
      new AlbumMusicItem(menu, 
			 strtol((*result)["rowid"], NULL, 10), 
			 (*result)["artist"], 
			 (*result)["album"], 
			 (*result)["genre"], 
			 strtol((*result)["tracks"], NULL, 10),
			 strtol((*result)["length"], NULL, 10),
			 m_genre_id);
  m_app->go(menu);  
}

AlbumMusicItem::AlbumMusicItem(Menu *menu, int album_id, const char *artist, const char *album, const char *genre, int tracks, int length, int genre_id)
  : ArrowItem(menu, album), m_album_id(album_id), m_tracks(tracks), m_length(length), m_genre_id(genre_id)
{
  safe_strcpy(m_artist, artist);
  safe_strcpy(m_album, album);
  safe_strcpy(m_genre, genre);
}

void AlbumMusicItem::select()
{
  char album_sql[256] = "";
  char genre_sql[256] = "";

  if (m_album_id) 
    sprintf(album_sql, "and album_id=%d", m_album_id);
  else
    sqlite3_snprintf(256, album_sql, "and album=%Q", m_album);
  if (m_genre_id) sprintf(genre_sql, "and genre_id=%d", m_genre_id);

  Menu *menu = new Menu(m_app, m_album);
  Database *db = m_app->database();
  Result *result;
  if (db->execute("select path, title, artist, album, genre, length from songs, albums, artists, genres where artists.rowid=artist_id and albums.rowid=album_id and genres.rowid=genre_id %s %s order by title", album_sql, genre_sql))
    while ((result=db->next()))
      new SongMusicItem(menu, 
			(*result)["path"], 
			(*result)["artist"], 
			(*result)["album"], 
			(*result)["title"], 
			(*result)["genre"],
			strtol((*result)["length"], NULL, 10));
  m_app->go(menu);  
}

void AlbumMusicItem::update()
{
  /*
  Renderer *r = m_app->renderer();

  r->image(143, 93, "data/unknown_album.png");
  r->color(0xff, 0xff, 0xff, 0xff);
  r->font(BOLD_FONT, 18);
  r->text(91, 518, m_album, 504);
  r->color(0x33, 0x33, 0x33, 0xff);
  r->rect(91, 531, 504, 2);  
  */
}

SongMusicItem::SongMusicItem(Menu *menu, const char *path, const char *artist, const char *album, const char *title, const char *genre, int length)
   : MenuItem(menu, title), m_length(length) 
{
  safe_strcpy(m_path, path);
  safe_strcpy(m_artist, artist);
  safe_strcpy(m_album, album);
  safe_strcpy(m_title, title);
  safe_strcpy(m_genre, genre);
}

void SongMusicItem::select()
{
  if (strcmp(m_app->audio()->nowPlaying(), m_path)) {
    debug("opening %s...\n", m_path);
    if (!m_app->audio()->open(m_path, m_artist, m_album, m_title, m_genre, m_length)) 
      return;
  }
  m_app->go(new Player(m_app));        
}
