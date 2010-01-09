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

#ifndef MMSINPUTMANAGER_
#define MMSINPUTMANAGER_

#include "mmstools/mmstools.h"
#include "mmsinput/mmsinputhandler.h"
#include "mmsinput/mmsinputthread.h"
#include "mmsgui/interfaces/immswindowmanager.h"
#include "mmsinput/mmsinputmapper.h"

#include <vector>

class MMSInputManager  {
	private:
		MMSMutex mutex;
		void handleInput(MMSInputEvent *inputevent);
		vector <MMSInputThread *> threads;
		IMMSWindowManager *windowmanager;
		vector <class MMSInputSubscription *> subscriptions;
		MMSKeySymbol lastkey;
		MMSInputMapper *mapper;
		MMSConfigData *config;
        MMSKeyMap mmskeys;

		//! store the window on which the button was pressed
		MMSWindow 	*buttonpress_window;
		bool		button_pressed;

	public:
		MMSInputManager(string file, string name);
		~MMSInputManager();
		void addDevice(MMS_INPUT_DEVICE device, int inputinterval);
		void setWindowManager(IMMSWindowManager *wm);
		void addSubscription(class MMSInputSubscription *sub);
		void startListen();
		void stopListen();

	friend class MMSInputThread;
};

#include "mmsinputsubscription.h"

#endif /*MMSINPUTMANAGER_*/
