#ifndef DATABASE_H
#define DATABASE_H

#include <map>

#include <sqlite3.h>
#include "config.h"
#include "Types.h"

#define DB_FILE "db"
#define MAX_SQL_SIZE 1024*10

typedef std::map<char *, char *, cmp_strcase> Result;

class Database
{
 private:
  sqlite3 *m_db;
  Result m_result;
  char **m_result_table;
  int m_rows;
  int m_cols;
  char *m_error;
  int m_curr_row;
  pthread_t m_thread;  
  volatile int m_scan_count;
  volatile bool m_scanning;  

 private:
  void createTables();
  void finalize();
  int insertArtist(const char *artist);
  int insertGenre(const char *genre);
  int insertAlbum(const char *album, int artist_id);
  int insertSong(const char *path, const char *title, const char *album, const char *artist, const char *genre, int length);
  static void *scan_thread(void *arg);
  void scan(const char *dir);

 public:
  Database(const char *file=DB_FILE);
  ~Database();
  void startScan();
  void stopScan();
  bool isScanning() { return m_scanning; }
  int scanCount() { return m_scan_count; }
  int execute(const char *sql, ...);
  Result *next();
};

#endif
