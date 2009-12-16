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

#include <stdio.h>
#include "Database.h"
#include "Utils.h"
#include "File.h"

Database::Database(const char *file)
  : m_db(NULL),
    m_result_table(NULL),
    m_curr_row(0)
{
  bool newDB = true;
  FILE *f;

  if ((f = fopen(file, "r"))) {
    fclose(f);
    newDB = false;
  }
  
  if (sqlite3_open_v2(file, &m_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX, 0) != SQLITE_OK) {
    sqlite3_close(m_db);
    m_db = NULL;
  }
  
  if (newDB) createTables();
}

void Database::createTables() 
{
  execute("CREATE TABLE albums (album text, artist_id integer)");
  execute("CREATE TABLE artists (artist text unique)");
  execute("CREATE TABLE genres (genre text unique)");
  execute("CREATE TABLE songs (title text, album_id integer, genre_id integer, length integer, path text)");
}

int Database::execute(const char *sql_fmt, ...)
{
  if (!m_db) return false;
  if (m_result_table) finalize();

  va_list ap;
  va_start(ap, sql_fmt);
  char *sql = sqlite3_vmprintf(sql_fmt, ap);
  va_end(ap);

  if (!sql) {
    fprintf(stderr, "can't allocate memory for sql statement!\n");
    return 0;
  }

  m_result_table = NULL;
  m_curr_row = 0;
  if (sqlite3_get_table(m_db, sql, &m_result_table, &m_rows, &m_cols, &m_error) != SQLITE_OK) {
    debug("error executing SQL query (%s): %s\n", sql, m_error);
    finalize();
    sqlite3_free(sql);
    return 0;
  }
  sqlite3_free(sql);
  m_curr_row = 1;
  return m_rows;
}

Result *Database::next()
{
  if (m_curr_row > m_rows) 
    return NULL;

  m_result.clear();
  for (int col = 0; col < m_cols; col++) {
    m_result[m_result_table[col]] = m_result_table[m_curr_row*m_cols+col];
  }
  m_curr_row++;
  return &m_result;
}

void Database::finalize()
{
  if (!m_db || !m_result_table) return;
  sqlite3_free_table(m_result_table);
  m_result_table = NULL;
}

int Database::insertArtist(const char *artist)
{
  debug("insertArtist: %s\n", artist);
  if (execute("select rowid from artists where artist=%Q", artist)) {
    debug("returning artist_id\n");
    Result *r = next();
    if (r && r->find("rowid") != r->end())
      return strtol((*r)["rowid"], NULL, 10);
    return 0;
  }
  debug("inserting into artists\n");
  execute("insert into artists (artist) values (%Q)", artist);
  return sqlite3_last_insert_rowid(m_db);
}

int Database::insertGenre(const char *genre)
{
  debug("insertGenre: %s\n", genre);
  if (execute("select rowid from genres where genre=%Q", genre)) {
    debug("returning genre_id\n");
    Result *r = next();
    if (r && r->find("rowid") != r->end())
      return strtol((*r)["rowid"], NULL, 10);
    return 0;
  }
  debug("inserting into genres\n");
  execute("insert into genres (genre) values (%Q)", genre);
  return sqlite3_last_insert_rowid(m_db);
}

int Database::insertAlbum(const char *album, int artist_id)
{
  debug("insertAlbum: %s (artist_id=%d)\n", album, artist_id);
  if (execute("select rowid from albums where album=%Q and artist_id=%d", album, artist_id)) {
    debug("returning album_id\n");
    Result *r = next();
    if (r && r->find("rowid") != r->end())
      return strtol((*r)["rowid"], NULL, 10);
    return 0;
  }
  debug("inserting into albums\n");
  execute("insert into albums (album, artist_id) values (%Q, %d)", album, artist_id);
  return sqlite3_last_insert_rowid(m_db);
}

int Database::insertSong(const char *path, const char *title, const char *album, const char *artist, const char *genre, int length)
{
  debug ("path: %s\n", path ? path : "NULL");
  debug ("title: %s\n", title ? title : "NULL");
  debug ("album: %s\n", album ? album : "NULL");
  debug ("artist: %s\n", artist ? artist : "NULL");
  debug ("genre: %s\n", genre ? genre : "NULL");
  int artist_id = insertArtist(artist);
  debug("inserted artist_id=%d\n", artist_id);
  int album_id;
  if (artist_id > 0) {
    album_id = insertAlbum(album, artist_id);
    debug("inserted album_id=%d\n", album_id);
  }
  else
    return 0;
  debug("insertSong: %s (album_id=%d)\n", title, album_id);
  if (album_id > 0 && execute("select rowid from songs where title=%Q and album_id=%d", title, album_id)) {
    debug("returning song_id\n");
    Result *r = next();
    if (r && r->find("rowid") != r->end())
      return strtol((*r)["rowid"], NULL, 10);
    return 0;
  }

  int genre_id = insertGenre(genre);
  debug("inserted genre_id=%d\n", genre_id);
  debug("inserting into songs\n");
  execute("insert into songs (title, album_id, genre_id, length, path) values (%Q, %d, %d, %d, %Q)", title, album_id, genre_id, length, path);
  return sqlite3_last_insert_rowid(m_db);
}

Database::~Database()
{
  finalize();
  sqlite3_close(m_db);
}
