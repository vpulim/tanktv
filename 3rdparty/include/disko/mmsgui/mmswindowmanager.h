/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 BerLinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009      BerLinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <stefan.schwarzer@diskohq.org>,                 *
 *      Matthias Hardt     <matthias.hardt@diskohq.org>,                   *
 *      Jens Schneider     <pupeider@gmx.de>,                              *
 *      Guido Madaus       <guido.madaus@diskohq.org>,                     *
 *      Patrick Helterhoff <patrick.helterhoff@diskohq.org>,               *
 *      René Bählkow       <rene.baehlkow@diskohq.org>                     *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License version 2.1 as published by the Free Software Foundation.     *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 **************************************************************************/

#ifndef MMSWINDOWMANAGER_H_
#define MMSWINDOWMANAGER_H_

#include "mmsgui/interfaces/immswindowmanager.h"

class MMSWindowManager : public IMMSWindowManager {
	private:
		//! visible screen area
        MMSFBRectangle		vrect;

        //! windows known by the window manager
        vector<MMSWindow*>	windows;

        //! the toplevel window
        MMSWindow           *toplevel;

        //! the background window
        MMSWindow           *backgroundwindow;

        //! translator instance which can be used to translate text
        MMSTranslator		translator;

        //! connection object for onTargetLangChanged callback
        sigc::connection 	onTargetLangChanged_connection;


        void showBackgroundWindow();

        void onTargetLangChanged(MMS_LANGUAGE_TYPE lang);

	public:
		MMSWindowManager(MMSFBRectangle vrect);
		virtual ~MMSWindowManager();
		void reset();

        MMSFBRectangle getVRect();

		void addWindow(MMSWindow *window);
        void removeWindow(MMSWindow *window);

        bool lowerToBottom(MMSWindow *window);
		bool raiseToTop(MMSWindow *window);

        bool hideAllMainWindows(bool goback = false);
        bool hideAllPopupWindows(bool except_modal = false);
        bool hideAllRootWindows(bool willshown = false);

        bool setToplevelWindow(MMSWindow *window);
        MMSWindow *getToplevelWindow();
		void removeWindowFromToplevel(MMSWindow *window);

        void setBackgroundWindow(MMSWindow *window);
        MMSWindow *getBackgroundWindow();

        void setPointerPosition(int pointer_posx, int pointer_posy, bool pressed = true);

        MMSTranslator *getTranslator();
};

/* access to global mmswindowmanager */
//extern MMSWindowManager mmswindowmanager;

#endif /*MMSWINDOWMANAGER_H_*/
