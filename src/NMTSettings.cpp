/*
  Copyright (c) 2010 Vinay Pulim & Nathan Huizinga

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

/*! \file
 * \author Nathan Huizinga
 * \date Created: 16-01-2010
 * \date Last modified: 25-01-2010
 * \date Last reviewed: -
 */

/*------------------------------------------------------------------------
 *	system includes
 *------------------------------------------------------------------------
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <boost/assert.hpp>

/*------------------------------------------------------------------------
 *	application includes
 *------------------------------------------------------------------------
 */
#include "NMTSettings.h"

/*------------------------------------------------------------------------
 *	general definitions
 *------------------------------------------------------------------------
 */
#define NMT_SETTINGS_FILE   "/tmp/setting.txt"

/*------------------------------------------------------------------------
 * type definitions
 *------------------------------------------------------------------------
 */

/*------------------------------------------------------------------------
 *	static variables
 *------------------------------------------------------------------------
 */

/////////////////////////////// PUBLIC ///////////////////////////////////////

NMTSettings::NMTSettings() :
  m_videoMode(-1)
{
  if (checkSettingsFile(NMT_SETTINGS_FILE) == false)
  {
    fprintf(stderr, "Error reading from: \"%s\"!\n", NMT_SETTINGS_FILE);
  }
  else
  {
    // Load settings from file
    BOOST_ASSERT(getVideoModeFromFile(NMT_SETTINGS_FILE) == true);
  }
}

NMTSettings::~NMTSettings()
{

}

int
NMTSettings::getVideoMode()
{
  return m_videoMode;
}

bool NMTSettings::setVideoMode(int videoMode)
{
  // For now, just set the new mode.
  m_videoMode = videoMode;

  return true;
}

const char * NMTSettings::getVideoModeStr()
{
  char * str = "";

  switch (m_videoMode)
  {
    case (0):
    {
      str = "Auto";
    }
    break;

    case (1):
    {
      str = "Composite NTSC";
    }
    break;

    case (2):
    {
      str = "Composite PAL";
    }
    break;

    case (3):
    {
      str = "Component NTSC 480i 60Hz";
    }
    break;

    case (4):
    {
      str = "Component PAL 576i 50Hz";
    }
    break;

    case (5):
    {
      str = "Component 480p 60Hz";
    }
    break;

    case (6):
    {
      str = "Component 720p 60Hz";
    }
    break;

    case (7):
    {
      str = "Component 1080p 60Hz";
    }
    break;

    case (8):
    {
      str = "Component 1080i 60Hz";
    }
    break;

    case (9):
    {
      str = "HDMI 480p 60Hz";
    }
    break;

    case (10):
    {
      str = "HDMI 720p 60Hz";
    }
    break;

    case (11):
    {
      str = "Component 1080p 24Hz";
    }
    break;

    case (13):
    {
      str = "Component 720p 50Hz";
    }
    break;

    case (14):
    {
      str = "Component 1080p 50Hz";
    }
    break;

    case (15):
    {
      str = "Component 1080i 50Hz";
    }
    break;

    case (16):
    {
      str = "HDMI 720p 50Hz";
    }
    break;

    case (18):
    {
      str = "HDMI 1080p 50Hz";
    }
    break;

    case (29):
    {
      str = "HDMI 1080p 60Hz";
    }
    break;

    case (30):
    {
      str = "Component 576p 50Hz";
    }
    break;

    case (31):
    {
      str = "HDMI 576p 50Hz";
    }
    break;
 
    case (32):
    {
      str = "HDMI 1080i 60Hz";
    }
    break;

    default:
    {
      str = "Unsupported video mode";
    }
    break;
  }

  return str;
}

/////////////////////////////// PROTECTED ////////////////////////////////////


///////////////////////////////  PRIVATE  ////////////////////////////////////

bool
NMTSettings::checkSettingsFile(const char * filename)
{
  FILE * f;

  if ((f = fopen(filename, "r")) == NULL)
  {
    return false;
  }

  fclose(f);

  return true;
}

bool
NMTSettings::getVideoModeFromFile(const char * filename)
{
  FILE * f;

  // Open settings file, should *not* fail, because
  // it was checked by calling checkSettingsFile().
  f = fopen(filename, "r");
  BOOST_ASSERT(f != NULL);

	char line[256];

	while (fgets(line, sizeof(line), f) != NULL)
  {
    char * p;

    if ((p = strchr(line, '=')) != NULL)
    {
      *(p++) = '\0';
      if (strcmp(line, "video_output") == 0)
      {
        m_videoMode = atoi(p);
        break;
      }
    }
  }

  fclose(f);

  return (m_videoMode != -1);
}

