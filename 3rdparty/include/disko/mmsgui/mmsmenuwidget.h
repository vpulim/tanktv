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

#ifndef MMSMENUWIDGET_H_
#define MMSMENUWIDGET_H_

#include "mmsgui/mmswidget.h"

//! With this class you can display a list of items.
/*!
The menu widget is focusable. So the user can scroll in it.
Menu items are normal widgets. But the root widget of an menu item has to be a focusable widget.
The root widget is normally a MMSButton widget.
Please note that you can nesting all other widgets under the root widget of the menu item.
So you can build complex menu items. But think about the performance if you work with menus
with an high number of items.
\author Jens Schneider
*/
class MMSMenuWidget : public MMSWidget {
    private:
    	typedef struct {
    		string			name;
    		class MMSWindow	*window;
    		MMSMenuWidget	*menu;
    		MMSWidget		*separator;
    	} MMSMENUITEMINFOS;

        string          	className;
        MMSMenuWidgetClass  *menuWidgetClass;
        MMSMenuWidgetClass  myMenuWidgetClass;

        MMSFBSurface    *selimage;
        MMSWidget       *itemTemplate;

        int    			item_w;     /* width of an item */
        int    			item_h;     /* height of an item */
        int    			v_items;    /* number of visible vertical items */
        int    			h_items;    /* number of visible horizontal items */

        //! x position of the selected item
        int    			x;
        //! y position of the selected item
        int    			y;
        //! scroll x-offset
        int    			px;
        //! scroll y-offset
        int    			py;

        bool            firstFocus;
        bool            firstSelection;

        bool			zoomsel;		/* should the selected item zoomed? */
        unsigned int 	zoomselwidth;	/* this value will be added to item_w for the selected item */
        unsigned int 	zoomselheight;	/* this value will be added to item_h for the selected item */
        int 			zoomselshiftx;	/* x-move the unselected items around the selected item */
        int 			zoomselshifty;	/* y-move the unselected items around the selected item */

        bool 			smooth_scrolling;
        int  			scrolling_offset;

        bool 			smooth_selection;
        int  			selection_offset_x;
        int  			selection_offset_y;

        unsigned int 	frame_delay;
        unsigned int 	frame_delay_set;


        MMSFBRectangle 	virtualGeom;

        //! this will be used to show/hide the menu and its whole parent window(s)
        //! normally this is the same as widgets rootwindow, but it can also be the a parent from widgets rootwindow
        MMSWindow		*parent_window;

        //! represents additional informations for each menu item
        vector<MMSMENUITEMINFOS>	iteminfos;

        //! if != -1 then currently activated submenu is set
        int 			curr_submenu;

        //! if a submenu does appear, we will save the parent menu here which will used to go back
        MMSMenuWidget	*parent_menu;

        //! if != -1 then the item with this id is set as go-back-item
        //! if the user enters this item, the parent menu (if does exist) will be shown
        int				back_item;

        bool create(MMSWindow *root, string className, MMSTheme *theme);

        void add(MMSWidget *widget);

        void adjustVirtualRect();

        bool getConfig(bool *firstTime = NULL);

        void drawchildren(bool toRedrawOnly = false, bool *backgroundFilled = NULL);
        void recalculateChildren();

        void initParentWindow(void);
        void setRootWindow(MMSWindow *root, MMSWindow *parentroot = NULL);

        void switchArrowWidgets();
        void setSliders();

        void selectItem(MMSWidget *item, bool set, bool refresh = true, bool refreshall = false);

        bool scrollDownEx(unsigned int count, bool refresh, bool test, bool leave_selection);
        bool scrollUpEx(unsigned int count, bool refresh, bool test, bool leave_selection);
        bool scrollRightEx(unsigned int count, bool refresh, bool test, bool leave_selection);
        bool scrollLeftEx(unsigned int count, bool refresh, bool test, bool leave_selection);

        void emitOnReturnForParents(MMSMenuWidget *orw);
        bool callOnReturn();

        bool switchToSubMenu();
        bool switchBackToParentMenu(MMSDIRECTION direction = MMSDIRECTION_NOTSET, bool closeall = false);

    public:
        MMSMenuWidget(MMSWindow *root, string className, MMSTheme *theme = NULL);
        ~MMSMenuWidget();

        MMSWidget *copyWidget();

        void setItemTemplate(MMSWidget *itemTemplate);
        MMSWidget *getItemTemplate();

        MMSWidget *newItem();
        void clear();

        void setFocus(bool set, bool refresh = true, MMSInputEvent *inputevent = NULL);

        bool setSelected(unsigned int item, bool refresh = true, bool *changed = NULL, bool joined = false);
        unsigned int getSelected();

        MMSWidget *getItem(unsigned int item);
        MMSWidget *getSelectedItem();

        unsigned int getSize();

        unsigned int getVItems();
        unsigned int getHItems();

        bool init();
        bool draw(bool *backgroundFilled = NULL);

        bool scrollDown(unsigned int count = 1, bool refresh = true, bool test = false, bool leave_selection = false);
        bool scrollUp(unsigned int count = 1, bool refresh = true, bool test = false, bool leave_selection = false);
        bool scrollRight(unsigned int count = 1, bool refresh = true, bool test = false, bool leave_selection = false);
        bool scrollLeft(unsigned int count = 1, bool refresh = true, bool test = false, bool leave_selection = false);
        bool scrollTo(int posx, int posy, bool refresh = true, bool *changed = NULL,
					  MMSWIDGET_SCROLL_MODE mode = MMSWIDGET_SCROLL_MODE_SETSELECTED, MMSFBRectangle *inputrect = NULL);

        bool setSubMenuName(unsigned int item, const char *name);
        bool setSubMenuName(unsigned int item, string &name);
        bool setBackItem(unsigned int item);
        bool setSeparator(unsigned int item, MMSWidget *widget, bool refresh = true);

        sigc::signal<void, MMSWidget*> *onSelectItem;
        sigc::signal<void, MMSWidget*> *onBeforeScroll;

    public:
        /* theme access methods */
        MMSTaffFile *getTAFF();
        string getItemWidth();
        string getItemHeight();
        unsigned int getItemHMargin();
        unsigned int getItemVMargin();
        unsigned int getCols();
        unsigned int getDimItems();
        int getFixedPos();
        bool getHLoop();
        bool getVLoop();
        unsigned int getTransItems();
        unsigned int getDimTop();
        unsigned int getDimBottom();
        unsigned int getDimLeft();
        unsigned int getDimRight();
        unsigned int getTransTop();
        unsigned int getTransBottom();
        unsigned int getTransLeft();
        unsigned int getTransRight();
        string getZoomSelWidth();
        string getZoomSelHeight();
        string getZoomSelShiftX();
        string getZoomSelShiftY();
        bool getSmoothScrolling();
        string getParentWindow();
        bool getSelImagePath(string &selimagepath);
        bool getSelImageName(string &selimagename);
        bool getSmoothSelection();
        unsigned int getSmoothDelay();

        void setItemWidth(string itemwidth, bool refresh = true);
        void setItemHeight(string itemheight, bool refresh = true);
        void setItemHMargin(unsigned int itemhmargin, bool refresh = true);
        void setItemVMargin(unsigned int itemvmargin, bool refresh = true);
        void setCols(unsigned int cols, bool refresh = true);
        void setDimItems(unsigned int dimitems, bool refresh = true);
        void setFixedPos(int fixedpos, bool refresh = true);
        void setHLoop(bool hloop);
        void setVLoop(bool vloop);
        void setTransItems(unsigned int transitems, bool refresh = true);
        void setDimTop(unsigned int dimtop, bool refresh = true);
        void setDimBottom(unsigned int dimbottom, bool refresh = true);
        void setDimLeft(unsigned int dimleft, bool refresh = true);
        void setDimRight(unsigned int dimright, bool refresh = true);
        void setTransTop(unsigned int transtop, bool refresh = true);
        void setTransBottom(unsigned int transbottom, bool refresh = true);
        void setTransLeft(unsigned int transleft, bool refresh = true);
        void setTransRight(unsigned int transright, bool refresh = true);
        void setZoomSelWidth(string zoomselwidth, bool refresh = true);
        void setZoomSelHeight(string zoomselheight, bool refresh = true);
        void setZoomSelShiftX(string zoomselshiftx, bool refresh = true);
        void setZoomSelShiftY(string zoomselshifty, bool refresh = true);
        void setSmoothScrolling(bool smoothscrolling);
        void setParentWindow(string parentwindow);
        void setSelImagePath(string selimagepath, bool load = true, bool refresh = true);
        void setSelImageName(string selimagename, bool load = true, bool refresh = true);
        void setSmoothSelection(bool smoothselection);
        void setSmoothDelay(unsigned int smoothdelay);

        void updateFromThemeClass(MMSMenuWidgetClass *themeClass);
};

#endif /*MMSMENUWIDGET_H_*/

