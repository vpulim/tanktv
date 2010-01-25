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

#ifndef NMTSettings_H
#define NMTSettings_H

/*! \file
 */

/*------------------------------------------------------------------------
 * system includes
 *------------------------------------------------------------------------
 */

/*------------------------------------------------------------------------
 * application includes
 *------------------------------------------------------------------------
 */

/*------------------------------------------------------------------------
 * general definitions
 *------------------------------------------------------------------------
 */

/*------------------------------------------------------------------------
 * type definitions
 *------------------------------------------------------------------------
 */

/*! \brief Short description (one line)
 *
 * Long description.
 *
 * \author Nathan Huizinga
 * \date Created: 16-01-2010
 * \date Last modified: 25-01-2010
 * \date Last reviewed: -
 */
class NMTSettings
{
public:

  /*! \brief Default constructor.
	 */
	NMTSettings();

  /*! \brief Default destructor.
	 */
  ~NMTSettings();

  /*!
   * \brief Get current video mode.
   * 
   * \return 
   */
  int getVideoMode();

  /*!
   * \brief Set (override) video mode.
   * 
   * \param[in] videoMode
   * 
   * \return 
   */
  bool setVideoMode(int videoMode);

  /*!
   * \brief Get current video mode as zero-terminater string.
   * 
   * \return 
   */
  const char * getVideoModeStr();

protected:

private:
  int   m_videoMode;

  bool checkSettingsFile(const char * filename);
  bool getVideoModeFromFile(const char * filename);

};

#endif // NMTSettings_H


