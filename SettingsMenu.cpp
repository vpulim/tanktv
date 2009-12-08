#include <stdio.h>
#include "Menu.h"

class ScanningItem : public InfoItem
{
 public:
  ScanningItem(Menu *menu)
    : InfoItem(menu, "") 
  {
    update();
  }  

  virtual void update()
  {
    if (m_app->database()->isScanning()) {
      char count[100];
      sprintf(count, "Files: %d", m_app->database()->scanCount());
      setInfo(count, "Stop Scan");
    }
    else {
      if (strcmp(m_label, "Scan Media"))
        setInfo("", "Scan Media");
    }
  }

  virtual void select() 
  {
    if (m_app->database()->isScanning()) 
      m_app->database()->stopScan();
    else
      m_app->database()->startScan();
    update();
  }
};

SettingsMenu::SettingsMenu(Application *application)
  : Menu(application)
{  
  new ScanningItem(this);
  //  new MenuItem(this, "Dummy");
}
