#include <stdio.h>
#include "Menu.h"

class ScanningItem : public InfoItem
{
 private:
  void updateLabel() {
    if (m_app->database()->isScanning()) {
      char count[100];
      setLabel("Stop Scan");
      sprintf(count, "Files: %d", m_app->database()->scanCount());
      setInfo(count);
    }
    else {
      setLabel("Scan Media");
      setInfo("");
    }
  }

 public:
  ScanningItem(Menu *menu)
    : InfoItem(menu, "") 
  {
    handleIdle();
  }  

  virtual bool ScanningItem::handleIdle()
  {
    updateLabel();
  }

  virtual void select() 
  {
    if (m_app->database()->isScanning()) 
      m_app->database()->stopScan();
    else
      m_app->database()->startScan();
    updateLabel();
  }
};

SettingsMenu::SettingsMenu(Application *application)
  : Menu(application, "NMT TV")
{  
  new ScanningItem(this);
  new MenuItem(this, "Dummy");
}
