#include <stdio.h>
#include <stdlib.h>
#include <taglib.h>
#include <tag.h>
#include <fileref.h>
#include "Database.h"
#include "File.h"
#include "Utils.h"

Database::Database(const char *file)
  : m_db(NULL),
    m_result_table(NULL),
    m_curr_row(0),
    m_scan_count(0)
{
  bool newDB = true;
  FILE *f;

  if ((f = fopen(file, "r"))) {
    fclose(f);
    newDB = false;
  }
  
  if (sqlite3_open(file, &m_db) != SQLITE_OK) {
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
  if (execute("select rowid from artists where artist=%Q", artist)) {
    return strtol((*next())["rowid"], NULL, 10);
  }
  execute("insert into artists (artist) values (%Q)", artist);
  return sqlite3_last_insert_rowid(m_db);
}

int Database::insertGenre(const char *genre)
{
  if (execute("select rowid from genres where genre=%Q", genre)) {
    return strtol((*next())["rowid"], NULL, 10);
  }
  execute("insert into genres (genre) values (%Q)", genre);
  return sqlite3_last_insert_rowid(m_db);
}

int Database::insertAlbum(const char *album, int artist_id)
{
  if (execute("select rowid from albums where album=%Q and artist_id=%d", album, artist_id)) {
    return strtol((*next())["rowid"], NULL, 10);
  }
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
  int album_id = insertAlbum(album, artist_id);
  if (execute("select rowid from songs where title=%Q and album_id=%d", title, album_id)) {
    return strtol((*next())["rowid"], NULL, 10);
  }

  int genre_id = insertGenre(genre);
  execute("insert into songs (title, album_id, genre_id, length, path) values (%Q, %d, %d, %d, %Q)", title, album_id, genre_id, length, path);
  return sqlite3_last_insert_rowid(m_db);
}

void Database::startScan()
{
  if (!m_scanning) {
    m_scanning = true;
    m_scan_count = 0;
    debug("started scanning\n");
    pthread_create(&m_thread, NULL, scan_thread, this);
  }
}

void Database::stopScan()
{
  debug("stopped scanning\n");
  m_scanning = false;
  if (m_thread) pthread_join(m_thread, 0);
  m_thread = 0;
}

void Database::scan(const char *dir)
{
  std::vector<File> files;
  TagLib::FileRef *f;
  TagLib::Tag *tag;
  TagLib::AudioProperties *props;
  const char *path;
  char *c;
  char artist[1024], album[1024], title[1024], genre[256];
  int length;

  File::listDirectory(dir, files);

  m_scan_count++;
  for (int i=0; i < files.size() && m_scanning; i++) {    
    path = files[i].path();
    if (files[i].isDirectory()) {
      scan(path);
    }
    else if (files[i].isAudio()) {
      if (!execute("select rowid from songs where path=%Q", files[i].path())) {
	debug("inserting: %s\n", path);
	f = new TagLib::FileRef(path);
	tag = f->tag();
	if (tag) {
	  safe_strcpy(artist, tag->artist().isEmpty() ? "Unknown Artist" : tag->artist().to8Bit().c_str());
	  safe_strcpy(album, tag->album().isEmpty() ? "Unknown Album" : tag->album().to8Bit().c_str());
	  safe_strcpy(title, tag->title().to8Bit().c_str());
	  safe_strcpy(genre, tag->genre().to8Bit().c_str());
	  if (!title[0]) {
	    safe_strcpy(title, files[i].name());
	    if ((c = strrchr(title, '.'))) *c=0;
	  }
	  length = 0;
	  if ((props = f->audioProperties()))
	    length = props->length();
	  insertSong(path, title, album, artist, genre, length);
	}
	delete f;
      }
    }
  }  
}

void *Database::scan_thread(void *arg)
{
  Database *db = (Database *)arg;
  db->scan("/share/Music");
  db->m_thread = 0;
  db->stopScan();
}

Database::~Database()
{
  finalize();
  sqlite3_close(m_db);
}
