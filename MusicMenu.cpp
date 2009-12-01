#include <stdlib.h>
#include "Menu.h"
#include "Player.h"
#include "Utils.h"

class Song
{
public:
  int song_id;
  char title[256];
  char album[256];
  char artist[256];
  char genre[256];
  char path[1024];
  int length;

  Song(Result &result) 
  {
    song_id = strtol(result["rowid"], NULL, 10);
    safe_strcpy(title, result["title"]);
    safe_strcpy(album, result["album"]);
    safe_strcpy(artist, result["artist"]);
    safe_strcpy(genre, result["genre"]);
    safe_strcpy(path, result["path"]);
    length = strtol(result["length"], NULL, 10);
  }
};

class Album
{
public:
  int album_id;
  char album[256];
  char artist[256];
  char genre[256];
  int artist_id;
  int genre_id;
  int tracks;
  int length;
  int num_artists;

  Album(Result &result) 
  {
    album_id = strtol(result["rowid"], NULL, 10);
    safe_strcpy(album, result["album"]);
    safe_strcpy(artist, result["artist"]);
    safe_strcpy(genre, result["genre"]);
    artist_id = result.find("artist_id")==result.end() ? 0 : strtol(result["artist_id"], NULL, 10);
    genre_id = result.find("genre_id")==result.end() ? 0 : strtol(result["genre_id"], NULL, 10);
    tracks = strtol(result["tracks"], NULL, 10);
    length = strtol(result["length"], NULL, 10);
    num_artists = result.find("num_artists")==result.end() ? 1 : strtol(result["num_artists"], NULL, 10);
  }
};

class Artist
{
public:
  int artist_id;
  char artist[256];
  int genre_id;
  
  Artist(Result &result) 
  {
    artist_id = strtol(result["rowid"], NULL, 10);
    safe_strcpy(artist, result["artist"]);
    genre_id = result.find("genre_id")==result.end() ? 0 : strtol(result["genre_id"], NULL, 10);
  }
};

class Genre
{
public:
  int genre_id;
  char genre[256];
  
  Genre(Result &result) 
  {
    genre_id = strtol(result["rowid"], NULL, 10);
    safe_strcpy(genre, result["genre"]);
  }
};

class SongsMenu : public Menu
{
private:

  std::vector<class Song> m_songs;

public:
  SongsMenu(Application *application, Album *album=NULL) 
    : Menu(application, "Songs")
  {
    Database *db = m_app->database();
    Result *result;
    char sql[512];

    if (album && album->num_artists > 1)
      sqlite3_snprintf(sizeof(sql), sql, "select songs.rowid, path, title, artist, album, genre, length from songs, albums, artists, genres where artists.rowid=artist_id and albums.rowid=album_id and genres.rowid=genre_id and album=%Q order by title", album->album);
    else if (album && album->genre_id)
      sqlite3_snprintf(sizeof(sql), sql, "select songs.rowid, path, title, artist, album, genre, length from songs, albums, artists, genres where artists.rowid=artist_id and albums.rowid=album_id and genres.rowid=genre_id and album_id=%d and genre_id=%d order by title", album->album_id, album->genre_id);
    else if (album)
      sqlite3_snprintf(sizeof(sql), sql, "select songs.rowid, path, title, artist, album, genre, length from songs, albums, artists, genres where artists.rowid=artist_id and albums.rowid=album_id and genres.rowid=genre_id and album_id=%d order by title", album->album_id);
    else
      sqlite3_snprintf(sizeof(sql), sql, "select songs.rowid, path, title, artist, album, genre, length from songs, albums, artists, genres where artists.rowid=artist_id and albums.rowid=album_id and genres.rowid=genre_id order by title");
    if (db->execute(sql))
      while ((result=db->next())) {
        m_songs.push_back(Song(*result));
	new ArrowItem(this, (*result)["title"]);
      }
  }

  void selectItem(MenuItem *menuItem)
  {
    Song &song = m_songs[menuItem->index()];
    if (strcmp(m_app->audio()->nowPlaying(), song.path)) {
      if (!m_app->audio()->open(song.path, song.artist, song.album, song.title, song.genre, song.length)) 
        return;
    }
    m_app->go(new Player(m_app));        
  }
};

class AlbumsMenu : public Menu
{
private:

  std::vector<class Album> m_albums;

public:
  AlbumsMenu(Application *application, Artist *artist=NULL) 
    : Menu(application, "Albums")
  {
    Database *db = m_app->database();
    Result *result;
    char sql[512];

    if (artist && artist->genre_id) 
      sqlite3_snprintf(sizeof(sql), sql, "select albums.rowid, count(1) as tracks, sum(length) as length, genre, artist, album from genres, albums, artists, songs where genres.rowid=genre_id and artists.rowid=artist_id and albums.rowid=album_id and artist_id=%d and genre_id=%d group by album_id order by album", artist->artist_id, artist->genre_id);
    else if (artist)
      sqlite3_snprintf(sizeof(sql), sql, "select albums.rowid, count(1) as tracks, sum(length) as length, genre, artist, album from genres, albums, artists, songs where genres.rowid=genre_id and artists.rowid=artist_id and albums.rowid=album_id and artist_id=%d group by album_id order by album", artist->artist_id);
    else
      sqlite3_snprintf(sizeof(sql), sql, "select count(distinct artist_id) as num_artists, albums.rowid, count(1) as tracks, sum(length) as length, genre, artist, album from genres, albums, artists, songs where genres.rowid=genre_id and artists.rowid=artist_id and albums.rowid=album_id group by album order by album");      
    if (db->execute(sql))
      while ((result=db->next())) {
        m_albums.push_back(Album(*result));
	new ArrowItem(this, (*result)["album"]);
      }
  }

  void selectItem(MenuItem *menuItem)
  {
    Album *album = &m_albums[menuItem->index()];
    m_app->go(new SongsMenu(m_app, album));        
  }
};

class ArtistsMenu : public Menu
{
private:

  std::vector<class Artist> m_artists;

public:
  ArtistsMenu(Application *application, Genre *genre=NULL) 
    : Menu(application, "Artists")
  {
    Database *db = m_app->database();
    Result *result;
    char sql[256];

    if (genre) 
      sqlite3_snprintf(sizeof(sql), sql, "select artists.rowid, artist, genre_id from artists, albums, songs where artists.rowid=artist_id and albums.rowid=album_id and genre_id=%d group by artist_id order by artist", genre->genre_id);
    else
      sqlite3_snprintf(sizeof(sql), sql, "select rowid, artist from artists order by artist");
    if (db->execute(sql))
      while ((result=db->next())) {
        m_artists.push_back(Artist(*result));
	new ArrowItem(this, (*result)["artist"]);
      }
  }

  void selectItem(MenuItem *menuItem)
  {
    Artist *artist = &m_artists[menuItem->index()];
    m_app->go(new AlbumsMenu(m_app, artist));    
  }
};

class GenresMenu : public Menu
{
private:

  std::vector<class Genre> m_genres;

public:
  GenresMenu(Application *application) 
    : Menu(application, "Genres")
  {
    Database *db = m_app->database();
    Result *result;
    if (db->execute("select rowid, genre from genres order by genre"))
      while ((result=db->next())) {
        m_genres.push_back(Genre(*result));
	new ArrowItem(this, (*result)["genre"]);
      }
  }

  void selectItem(MenuItem *menuItem)
  {
    Genre *genre = &m_genres[menuItem->index()];
    m_app->go(new ArtistsMenu(m_app, genre));    
  }
};

MusicMenu::MusicMenu(Application *application)
  : Menu(application, "Music")
{
  new ArrowItem(this, "Artists");
  new ArrowItem(this, "Albums");
  new ArrowItem(this, "Songs");
  new ArrowItem(this, "Genres");
}

void MusicMenu::selectItem(MenuItem *menuItem)
{
  if (!strcmp(menuItem->label(), "Artists"))
    m_app->go(new ArtistsMenu(m_app));
  if (!strcmp(menuItem->label(), "Albums"))
    m_app->go(new AlbumsMenu(m_app));
  if (!strcmp(menuItem->label(), "Songs"))
    m_app->go(new SongsMenu(m_app));
  if (!strcmp(menuItem->label(), "Genres"))
    m_app->go(new GenresMenu(m_app));
}