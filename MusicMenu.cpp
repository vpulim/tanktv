#include <stdlib.h>
#include "Menu.h"
#include "Player.h"
#include "Utils.h"

class ArtistsMenu : public Menu
{
private:
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

  std::vector<class Artist> m_artists;

public:
  ArtistsMenu(Application *application) 
    : Menu(application, "Artists")
  {
    Database *db = m_app->database();
    Result *result;
    if (db->execute("select rowid, artist from artists order by artist"))
      while ((result=db->next())) {
        m_artists.push_back(Artist(*result));
	new ArrowItem(this, (*result)["artist"]);
      }
  }

  virtual bool paintDetails(MenuItem *menuItem) 
  {
    debug("paintingDetails\n");
    if (m_current > -1) {
      Renderer *r = m_app->renderer();
      Artist &artist = m_artists[m_current];
      r->color(0, 0, 0, 0xff);
      r->rect(0,0, MENU_X-70, r->height());
      r->font(BOLD_FONT, 37);
      r->color(0xff, 0, 0, 0xff);
      r->text(100,100, artist.artist);
    }
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
}
