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
#ifndef MMSWIDGET_H_
#define MMSWIDGET_H_

#include "mmstools/mmstafffile.h"
#include "mmstools/mmslogger.h"
#include "mmsgui/theme/mmstheme.h"
#include "mmsgui/mmsguitools.h"
#include <sigc++/sigc++.h>


//example from: http://libsigc.sourceforge.net/libsigc2/docs/reference/html/classsigc_1_1signal_1_1accumulated.html
//! this accumulator calculates the arithmetic mean value
struct arithmetic_mean_accumulator
{
  typedef double result_type;
  template<typename T_iterator>
  result_type operator()(T_iterator first, T_iterator last) const
  {
    result_type value_ = 0;
    int n_ = 0;
    for (; first != last; ++first, ++n_)
      value_ += *first;
    return value_ / n_;
  }
};

//example from: http://libsigc.sourceforge.net/libsigc2/docs/reference/html/classsigc_1_1signal_1_1accumulated.html
//! this accumulator stops signal emission when a slot returns zero
struct interruptable_accumulator
{
  typedef bool result_type;
  template<typename T_iterator>
  result_type operator()(T_iterator first, T_iterator last) const
  {
    int n_ = 0;
    for (; first != last; ++first, ++n_)
      if (!*first) return false;
    return true;
  }
};

//! bool accumulator
/*!
with this accumulator the emit() method of a callback ends with
 - true,  if the no callback methods are connected or all connected callback methods returns true
 - false, if at least one connected callback method returns false
*/
struct bool_accumulator
{
  typedef bool result_type;
  template<typename T_iterator>
  result_type operator()(T_iterator first, T_iterator last) const
  {
    bool ret_ = true;
    int n_ = 0;
    for (; first != last; ++first, ++n_)
      if (!*first) ret_ = false;
    return ret_;
  }
};


//! bool accumulator (not)
/*!
with this accumulator the emit() method of a callback ends with
 - false, if the no callback methods are connected or all connected callback methods returns false
 - true,  if at least one connected callback method returns true
*/
struct neg_bool_accumulator
{
  typedef bool result_type;
  template<typename T_iterator>
  result_type operator()(T_iterator first, T_iterator last) const
  {
    bool ret_ = false;
    int n_ = 0;
    for (; first != last; ++first, ++n_)
      if (*first) ret_ = true;
    return ret_;
  }
};



/*
#ifndef DFBCHECK
    #define DFBCHECK( x... ) \
    {\
         DFBResult err = x;\
         if (err != DFB_OK) {\
              fprintf( stderr, "%s <%d>:\n\t", __FILE__, __LINE__ );\
              DirectFBErrorFatal( #x, err );\
         }\
    }
#endif*/

/*typedef void(*GUIINPUTCALLBACK)(DFBInputDeviceKeySymbol);

typedef struct {
	DFBInputDeviceKeySymbol key;
	GUIINPUTCALLBACK cb;
} INPUT_CB;*/

MMS_CREATEERROR(MMSWidgetError);


//! The available types of widgets.
typedef enum {
	//! A MMSHBoxWidget can contain 0 to n widgets. The widgets will be arranged in one horizontal row (0..n columns).
    MMSWIDGETTYPE_HBOX = 0,
	//! A MMSVBoxWidget can contain 0 to n widgets. The widgets will be arranged in one vertical column (0..n rows).
    MMSWIDGETTYPE_VBOX,
    //! A MMSButtonWidget can get the focus and therefore can process inputs.
    MMSWIDGETTYPE_BUTTON,
    //! A MMSImageWidget cannot get the focus but can be selected.
    MMSWIDGETTYPE_IMAGE,
    //! A MMSLabelWidget cannot get the focus but can be selected. It displays one line of text.
    MMSWIDGETTYPE_LABEL,
    //! A MMSMenuWidget can get the focus and therefore can process inputs. It displays one- or two-dimensional menus.
    MMSWIDGETTYPE_MENU,
    //! A MMSProgressBarWidget cannot get the focus but can be selected.
    MMSWIDGETTYPE_PROGRESSBAR,
    //! A MMSTextBoxWidget can get the focus and therefore can process inputs. It displays a formated multiline text.
    MMSWIDGETTYPE_TEXTBOX,
    //! A MMSArrowWidget cannot get the focus but can be selected.
    MMSWIDGETTYPE_ARROW,
    //! A MMSSliderWidget cannot get the focus but can be selected.
    MMSWIDGETTYPE_SLIDER,
    //! A MMSInputWidget can get the focus and therefore can process inputs. You can display and edit one line of text.
    MMSWIDGETTYPE_INPUT,
    //! A MMSCheckBoxWidget can get the focus and therefore can process inputs. You can display an on/off switch.
    MMSWIDGETTYPE_CHECKBOX,
	//! A MMSGapWidget is a spacer. It cannot have any children.
    MMSWIDGETTYPE_GAP
} MMSWIDGETTYPE;


//! define the window class
class MMSWindow;

extern string MMSWidget_inputmode;


typedef int MMSWIDGET_SCROLL_MODE;

#define MMSWIDGET_SCROLL_MODE_SETSELECTED	0x01
#define MMSWIDGET_SCROLL_MODE_SETPRESSED	0x02
#define MMSWIDGET_SCROLL_MODE_RMPRESSED		0x04


//! This class is the base class for all widgets.
/*!
This class includes the base functionality available for all widgets within MMSGUI.
This class cannot be constructed. Only widgets which are derived from this class can be constructed.
\author Jens Schneider
*/
class MMSWidget {
    private:
        //! type of the widget
        MMSWIDGETTYPE 		type;

        //! describes attributes for drawable widgets
        typedef struct {
        	//! access to the theme which is used
            MMSTheme            *theme;
            //! defaults from code
            MMSWidgetClass      *baseWidgetClass;
            //! settings from theme which overides defaults from code
            MMSWidgetClass      *widgetClass;
            //! settings from widget which overides settings from theme and defaults from code
            MMSWidgetClass      myWidgetClass;

            //! pointer to a widget thread if needed
            class MMSWidgetThread *widgetthread;

            //! background image used for unselected state
            MMSFBSurface        *bgimage;
            //! background image used for selected state
            MMSFBSurface        *selbgimage;
            //! background image used for unselected-pressed state
            MMSFBSurface        *bgimage_p;
            //! background image used for selected-pressed state
            MMSFBSurface        *selbgimage_p;
            //! background image used for unselected-inactive state
            MMSFBSurface        *bgimage_i;
            //! background image used for selected-inactive state
            MMSFBSurface        *selbgimage_i;

            //! border images used for unselected state
            MMSFBSurface		*borderimages[MMSBORDER_IMAGE_NUM_SIZE];
            //! geometry for border images (unselected state)
            MMSFBRectangle		bordergeom[MMSBORDER_IMAGE_NUM_SIZE];
            //! geometry for border images already set (unselected state)
            bool				bordergeomset;

            //! border images used for selected state
            MMSFBSurface		*borderselimages[MMSBORDER_IMAGE_NUM_SIZE];
            //! geometry for border images (selected state)
            MMSFBRectangle		borderselgeom[MMSBORDER_IMAGE_NUM_SIZE];
            //! geometry for border images already set (selected state)
            bool				borderselgeomset;

            //! widget which represents e.g. an arrow which selected state will be switched to true if content of widget can be scrolled up
            MMSWidget	*upArrowWidget;
            //! widget which represents e.g. an arrow which selected state will be switched to true if content of widget can be scrolled down
            MMSWidget	*downArrowWidget;
            //! widget which represents e.g. an arrow which selected state will be switched to true if content of widget can be scrolled left
            MMSWidget	*leftArrowWidget;
            //! widget which represents e.g. an arrow which selected state will be switched to true if content of widget can be scrolled right
            MMSWidget	*rightArrowWidget;
            //! the arrow widgets are initial drawn?
            bool		initialArrowsDrawn;

            //! widget to which is to navigate if user press cursor up
            MMSWidget	*navigateUpWidget;
            //! widget to which is to navigate if user press cursor down
            MMSWidget	*navigateDownWidget;
            //! widget to which is to navigate if user press cursor left
            MMSWidget	*navigateLeftWidget;
            //! widget to which is to navigate if user press cursor right
            MMSWidget	*navigateRightWidget;

            //! vertical slider widget
            MMSWidget	*vSliderWidget;
            //! horizontal slider widget
            MMSWidget	*hSliderWidget;

            //! store last input event here
            MMSInputEvent	last_inputevent;

            //! store input rectangle, this is the rectangle on which the button was pressed
            MMSFBRectangle	pressed_inputrect;

            //! current scroll posx
            unsigned int scrollPosX;
            //! current scroll posy
            unsigned int scrollPosY;
            //! horizontal scroll distance
            unsigned int scrollDX;
            //! vertical scroll distance
            unsigned int scrollDY;

            //! widget which status should changed just like this widget
            MMSWidget	*joinedWidget;

        } MMSWIDGET_DRAWABLE_ATTRIBUTES;

        //! save attributes for drawable widgets
        MMSWIDGET_DRAWABLE_ATTRIBUTES	*da;

        //! is widget initialized?
        bool	initialized;

        //! id of the widget
        int		id;

        //! name of the widget
        string	name;

        //! size of the widget
    	string	sizehint;

    	//! optional & application specific pointer to any data
    	void *bindata;

    	//! window to which the widget is connected
        MMSWindow	*rootwindow;

        //! the toplevel parent window
        MMSWindow	*parent_rootwindow;

        //! is widget drawable?
        bool drawable;

        //! should parent widget be drawn before drawing this widget?
        bool needsparentdraw;

        //! initial setting: focusable flag
        bool focusable_initial;

        //! initial setting: selectable flag
        bool selectable_initial;

        //! initial setting: clickable flag
        bool clickable_initial;

        //! children allowed?
        bool canhavechildren;

        //! children's selected state can be changed?
        bool canselectchildren;

        //! visible?
        bool visible;

        //! focused?
        bool focused;

        //! selected?
        bool selected;

        //! activated?
        bool activated;

        //! button pressed?
        bool pressed;

        //! brightness of the widget 0..255, default 255
        unsigned char brightness;

        //! opacity of the widget 0..255, default 255
        unsigned char opacity;

        //! widget is using a subsurface or has an own surface?
        bool has_own_surface;


        bool loadArrowWidgets();
        virtual void switchArrowWidgets();

        bool create(MMSWindow *root, bool drawable, bool needsparentdraw, bool focusable, bool selectable,
                    bool canhavechildren, bool canselectchildren, bool clickable);

        virtual bool init();
        virtual bool draw(bool *backgroundFilled = NULL);
        void drawMyBorder();
        bool drawDebug();

        void startWidgetThread(int delay);

    public:
        MMSWidget();
        virtual ~MMSWidget();
        MMSWIDGETTYPE getType();

        void copyWidget(MMSWidget *newWidget);
        virtual MMSWidget *copyWidget() = 0;

        MMSWidget* getChild(unsigned int atpos = 0);
        MMSWidget* disconnectChild(unsigned int atpos = 0);
        MMSWidget* searchForWidget(string name);
        MMSWidget* searchForWidgetType(MMSWIDGETTYPE type);
        MMSWidget* operator[](string name);

        virtual void add(MMSWidget *widget);
        MMSWindow *getRootWindow(MMSWindow **parentroot = NULL);
        virtual void setGeometry(MMSFBRectangle geom);
        MMSFBRectangle getGeometry();
        MMSFBRectangle getRealGeometry();
        MMSFBRectangle getInnerGeometry();
        MMSFBRectangle getSurfaceGeometry();

        MMSFBSurface *getSurface();

        int getId();
        string getName();
        void   setName(string name);
        void setParent(MMSWidget *parent);
        MMSWidget *getParent();

        void getJoinedWigdets(MMSWidget **caller_stack);

        virtual void setFocus(bool set, bool refresh = true, MMSInputEvent *inputevent = NULL);
        bool isFocused();
        virtual bool setSelected(bool set, bool refresh = true, bool *changed = NULL, bool joined = false);
        bool isSelected();
        void unsetFocusableForAllChildren(bool refresh);

        void setActivated(bool set, bool refresh = true);
        bool isActivated();

        void setPressed(bool set, bool refresh = true);
        bool isPressed();

        void setASelected(bool set, bool refresh = true);
        void setPSelected(bool set, bool refresh = true);
        void setISelected(bool set, bool refresh = true);

        bool isDrawable();
        bool needsParentDraw(bool checkborder = true);
        bool canHaveChildren();
        bool canSelectChildren();

        void setBinData(void *data);
        void *getBinData();
        bool setSizeHint(string &hint);
        string getSizeHint();
        bool isGeomSet();
        void setGeomSet(bool set);

        bool isVisible();
        virtual void setVisible(bool visible, bool refresh = true);

        unsigned char getBrightness();
        void setBrightness(unsigned char brightness, bool refresh = true);

        unsigned char getOpacity();
        void setOpacity(unsigned char opacity, bool refresh = true);

        MMSWidget *getNavigateUpWidget();
        MMSWidget *getNavigateDownWidget();
        MMSWidget *getNavigateLeftWidget();
        MMSWidget *getNavigateRightWidget();
        void setNavigateUpWidget(MMSWidget *upwidget);
        void setNavigateDownWidget(MMSWidget *downwidget);
        void setNavigateRightWidget(MMSWidget *rightwidget);
        void setNavigateLeftWidget(MMSWidget *leftwidget);

        bool canNavigateUp();
        bool canNavigateDown();
        bool canNavigateLeft();
        bool canNavigateRight();

        virtual bool scrollDown(unsigned int count = 1, bool refresh = true, bool test = false, bool leave_selection = false);
        virtual bool scrollUp(unsigned int count = 1, bool refresh = true, bool test = false, bool leave_selection = false);
        virtual bool scrollRight(unsigned int count = 1, bool refresh = true, bool test = false, bool leave_selection = false);
        virtual bool scrollLeft(unsigned int count = 1, bool refresh = true, bool test = false, bool leave_selection = false);
        virtual bool scrollTo(int posx, int posy, bool refresh = true, bool *changed = NULL,
							  MMSWIDGET_SCROLL_MODE mode = MMSWIDGET_SCROLL_MODE_SETSELECTED, MMSFBRectangle *inputrect = NULL);

        sigc::signal<void, MMSWidget*> *onSelect;
        sigc::signal<void, MMSWidget*, bool> *onFocus;
        sigc::signal<void, MMSWidget*> *onReturn;

        //! Set one or more callbacks for the onClick event.
        /*!
        The connected callbacks will be called after the button release event.

        A callback method must be defined like this:

        	void myclass::mycallbackmethod(MMSWidget *widget, int posx, int posy, int widget_width, int widget_height);

        	Parameters:

        		widget -> is the pointer to the widget

        		posx -> x-position of the maus pointer within the widget

        		posy -> y-position of the maus pointer within the widget

        		widget_width -> width of the widget

        		widget_height -> height of the widget

        To connect your callback to onClick do this:

        	mywidget->onClick->connect(sigc::mem_fun(myobject,&myclass::mycallbackmethod));
        */
        sigc::signal<void, MMSWidget*, int, int, int, int> *onClick;

    protected:
        virtual void drawchildren(bool toRedrawOnly = false, bool *backgroundFilled = NULL);
        virtual void setRootWindow(MMSWindow *root, MMSWindow *parentroot = NULL);
        virtual void recalculateChildren();
        virtual void handleInput(MMSInputEvent *inputevent);

        virtual bool callOnReturn() { return true; }

        bool geomset;


        bool toRedraw;
        bool redrawChildren;
        void markChildren2Redraw();
        virtual MMSWidget *getDrawableParent(bool mark2Redraw = false, bool markChildren2Redraw = false,
                                             bool checkborder = true, vector<MMSWidget*> *wlist = NULL, bool followpath = false);
        void refresh();

        MMSFBSurface *windowSurface;

        MMSFBSurface *surface;
        MMSFBRectangle surfaceGeom;

        virtual void setSurfaceGeometry(unsigned int width = 0, unsigned int height = 0);
        virtual void setInnerGeometry();

        bool setScrollSize(unsigned int dX = 8, unsigned int dY = 8);
        bool setScrollPos(int posX = 0, int posY = 0, bool refresh = true, bool test = false);
        MMSFBRectangle getVisibleSurfaceArea();
        void updateWindowSurfaceWithSurface(bool useAlphaChannel);

        MMSWidget *parent;
        vector<MMSWidget *> children;

        MMSFBRectangle geom;
        MMSFBRectangle innerGeom;

    public:
        /* theme access methods */
        bool 	getBgColor(MMSFBColor &bgcolor);
        bool 	getSelBgColor(MMSFBColor &selbgcolor);
        bool	getBgColor_p(MMSFBColor &bgcolor_p);
        bool	getSelBgColor_p(MMSFBColor &selbgcolor_p);
        bool	getBgColor_i(MMSFBColor &bgcolor_i);
        bool	getSelBgColor_i(MMSFBColor &selbgcolor_i);
        bool    getBgImagePath(string &bgimagepath);
        bool    getBgImageName(string &bgimagename);
        bool    getSelBgImagePath(string &selbgimagepath);
        bool    getSelBgImageName(string &selbgimagename);
        bool    getBgImagePath_p(string &bgimagepath_p);
        bool    getBgImageName_p(string &bgimagename_p);
        bool    getSelBgImagePath_p(string &selbgimagepath_p);
        bool    getSelBgImageName_p(string &selbgimagename_p);
        bool    getBgImagePath_i(string &bgimagepath_i);
        bool    getBgImageName_i(string &bgimagename_i);
        bool    getSelBgImagePath_i(string &selbgimagepath_i);
        bool    getSelBgImageName_i(string &selbgimagename_i);
        bool 	getMargin(unsigned int &margin);
        bool 	getFocusable(bool &focusable, bool check_selectable = true);
        bool 	getSelectable(bool &selectable);
        bool	getUpArrow(string &uparrow);
        bool	getDownArrow(string &downarrow);
        bool	getLeftArrow(string &leftarrow);
        bool	getRightArrow(string &rightarrow);
        bool	getData(string &data);
        bool	getNavigateUp(string &navigateup);
        bool	getNavigateDown(string &navigatedown);
        bool	getNavigateLeft(string &navigateleft);
        bool	getNavigateRight(string &navigateright);
        bool	getVSlider(string &vslider);
        bool	getHSlider(string &hslider);
        bool 	getImagesOnDemand(bool &imagesondemand);
        bool 	getBlend(unsigned int &blend);
        bool 	getBlendFactor(double &blendfactor);
        bool 	getScrollOnFocus(bool &scrollonfocus);
        bool 	getClickable(bool &clickable);
        bool 	getReturnOnScroll(bool &returnonscroll);
        bool	getInputMode(string &inputmode);
        bool 	getInputModeEx(string &inputmode);
        bool	getJoinedWidget(string &joinedwidget);

        bool	getBorderColor(MMSFBColor &color);
        bool 	getBorderSelColor(MMSFBColor &selcolor);
        bool 	getBorderImagePath(string &imagepath);
        bool	getBorderImageNames(MMSBORDER_IMAGE_NUM num, string &imagename);
        bool	getBorderSelImagePath(string &selimagepath);
        bool	getBorderSelImageNames(MMSBORDER_IMAGE_NUM num, string &selimagename);
        bool	getBorderThickness(unsigned int &thickness);
        bool	getBorderMargin(unsigned int &margin);
        bool 	getBorderRCorners(bool &rcorners);

        bool setBgColor(MMSFBColor bgcolor, bool refresh = true);
        bool setSelBgColor(MMSFBColor selbgcolor, bool refresh = true);
        bool setBgColor_p(MMSFBColor bgcolor_p, bool refresh = true);
        bool setSelBgColor_p(MMSFBColor selbgcolor_p, bool refresh = true);
        bool setBgColor_i(MMSFBColor bgcolor_i, bool refresh = true);
        bool setSelBgColor_i(MMSFBColor selbgcolor_i, bool refresh = true);
        bool setBgImagePath(string bgimagepath, bool load = true, bool refresh = true);
        bool setBgImageName(string bgimagename, bool load = true, bool refresh = true);
        bool setSelBgImagePath(string selbgimagepath, bool load = true, bool refresh = true);
        bool setSelBgImageName(string selbgimagename, bool load = true, bool refresh = true);
        bool setBgImagePath_p(string bgimagepath_p, bool load = true, bool refresh = true);
        bool setBgImageName_p(string bgimagename_p, bool load = true, bool refresh = true);
        bool setSelBgImagePath_p(string selbgimagepath_p, bool load = true, bool refresh = true);
        bool setSelBgImageName_p(string selbgimagename_p, bool load = true, bool refresh = true);
        bool setBgImagePath_i(string bgimagepath_i, bool load = true, bool refresh = true);
        bool setBgImageName_i(string bgimagename_i, bool load = true, bool refresh = true);
        bool setSelBgImagePath_i(string selbgimagepath_i, bool load = true, bool refresh = true);
        bool setSelBgImageName_i(string selbgimagename_i, bool load = true, bool refresh = true);
        bool setMargin(unsigned int margin, bool refresh = true);
        bool setFocusable(bool focusable, bool refresh = true);
        bool setSelectable(bool selectable, bool refresh = true);
        bool setUpArrow(string uparrow, bool refresh = true);
        bool setDownArrow(string downarrow, bool refresh = true);
        bool setLeftArrow(string leftarrow, bool refresh = true);
        bool setRightArrow(string rightarrow, bool refresh = true);
        bool setData(string data);
        bool setNavigateUp(string navigateup);
        bool setNavigateDown(string navigatedown);
        bool setNavigateLeft(string navigateleft);
        bool setNavigateRight(string navigateright);
        bool setVSlider(string vslider);
        bool setHSlider(string hslider);
        bool setImagesOnDemand(bool imagesondemand);
        bool setBlend(unsigned int blend, bool refresh = true);
        bool setBlendFactor(double blendfactor, bool refresh = true);
        bool setScrollOnFocus(bool scrollonfocus);
        bool setClickable(bool clickable);
        bool setReturnOnScroll(bool returnonscroll);
        bool setInputMode(string inputmode);
        bool setJoinedWidget(string joinedwidget);

        bool setBorderColor(MMSFBColor bordercolor, bool refresh = true);
        bool setBorderSelColor(MMSFBColor borderselcolor, bool refresh = true);
        bool setBorderImagePath(string borderimagepath, bool load = true, bool refresh = true);
        bool setBorderImageNames(string imagename_1, string imagename_2, string imagename_3, string imagename_4,
                                 string imagename_5, string imagename_6, string imagename_7, string imagename_8,
                                 bool load = true, bool refresh = true);
        bool setBorderSelImagePath(string borderselimagepath, bool load = true, bool refresh = true);
        bool setBorderSelImageNames(string selimagename_1, string selimagename_2, string selimagename_3, string selimagename_4,
                                    string selimagename_5, string selimagename_6, string selimagename_7, string selimagename_8,
                                    bool load = true, bool refresh = true);
        bool setBorderThickness(unsigned int borderthickness, bool refresh = true);
        bool setBorderMargin(unsigned int bordermargin, bool refresh = true);
        bool setBorderRCorners(bool borderrcorners, bool refresh = true);

        void updateFromThemeClass(MMSWidgetClass *themeClass);

    /* friends */
    friend class MMSWindow;
    friend class MMSHBoxWidget;
    friend class MMSVBoxWidget;
    friend class MMSLabelWidget;
    friend class MMSButtonWidget;
    friend class MMSImageWidget;
    friend class MMSProgressBarWidget;
    friend class MMSMenuWidget;
    friend class MMSTextBoxWidget;
    friend class MMSArrowWidget;
    friend class MMSSliderWidget;
    friend class MMSInputWidget;
    friend class MMSCheckBoxWidget;
    friend class MMSGapWidget;
};

#include "mmswindow.h"

#endif /*MMSWIDGET_H_*/
