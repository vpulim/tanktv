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

#ifndef MMSDIALOGMANAGER_H_
#define MMSDIALOGMANAGER_H_

#include "mmsgui/mmswindow.h"
#include "mmsgui/mmschildwindow.h"

class MMSDialogManager {
	private:
        MMSWindow               *rootWindow;
        bool					rootWindow_is_mine;

        vector<MMSChildWindow*> childWins;

        string              filename;
        MMSDescriptionClass description;    /* description of the dialog */

        vector<MMSWidget*>  namedWidgets;

        void insertNamedWidget(MMSWidget *widget);

        void throughDoc(MMSTaffFile *tafff, MMSWidget *currentWidget, MMSWindow *rootWindow,
        				MMSTheme *theme, bool only_first_child = false);

        void getDescriptionValues(MMSTaffFile *tafff, MMSTheme *theme);

        void getMainWindowValues(MMSTaffFile *tafff, MMSTheme *theme);
        void getPopupWindowValues(MMSTaffFile *tafff, MMSTheme *theme);
        void getRootWindowValues(MMSTaffFile *tafff, MMSTheme *theme);
        void getChildWindowValues(MMSTaffFile *tafff, MMSWindow *rootWindow, MMSTheme *theme);
        string getTemplateValues(MMSTaffFile *tafff, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getVBoxValues(MMSTaffFile *tafff, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getHBoxValues(MMSTaffFile *tafff, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getLabelValues(MMSTaffFile *tafff, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getButtonValues(MMSTaffFile *tafff, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getImageValues(MMSTaffFile *tafff, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getProgressBarValues(MMSTaffFile *tafff, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getMenuValues(MMSTaffFile *tafff, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getTextBoxValues(MMSTaffFile *tafff, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getArrowValues(MMSTaffFile *tafff, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getSliderValues(MMSTaffFile *tafff, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getInputValues(MMSTaffFile *tafff, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getCheckBoxValues(MMSTaffFile *tafff, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);
        string getGapValues(MMSTaffFile *tafff, MMSWidget *currentWidget, MMSWindow *rootWindow, MMSTheme *theme);

    public:
        MMSDialogManager();
        MMSDialogManager(MMSWindow *rootWindow);
        ~MMSDialogManager();
        bool isLoaded();
        MMSWindow* loadDialog(string filename, MMSTheme *theme = NULL);
        MMSChildWindow* loadChildDialog(string filename, MMSTheme *theme = NULL);
        MMSWidget* searchForWidget(string name);
        MMSWidget* operator[](string name);

        MMSDescriptionClass getDescription();
};

MMS_CREATEERROR(MMSDialogManagerError);

#endif /*MMSDIALOGMANAGER_H_*/
