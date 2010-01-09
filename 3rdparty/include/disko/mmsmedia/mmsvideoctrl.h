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

#ifdef  __HAVE_DIRECTFB__

#ifndef MMSVIDEOCTRL_H_
#define MMSVIDEOCTRL_H_

#include <directfb.h>

#include "mmstools/mmstools.h"

/**
 * @brief   Controls the video device.
 *
 * @ingroup     mmsmedia
 *
 * @author      Jens Schneider (pupeider@gmx.de)
 * @version     1.5.0
 * @date        12/31/2008
 *
 * This class controls global video output settings.
 */
class MMSVideoCtrl {
    private:
        IDirectFB                           *dfb;
        IDirectFBDisplayLayer               *layer;
        static DFBDisplayLayerDescription   desc;
        static DFBColorAdjustment           cadj;

    public:
        /* constructor */
        MMSVideoCtrl();

        /* destructor */
        ~MMSVideoCtrl();

        /* get the description and capabilities of the display layer */
        DFBDisplayLayerDescription getDisplayLayerDescription();

        /* get the current color adjustment of the display layer */
        DFBColorAdjustment getColorAdjustment();

        /* adjust brightness */
        void adjustBrightness(unsigned val = 0x8000);

        /* adjust contrast */
        void adjustContrast(unsigned val = 0x8000);

        /* adjust hue */
        void adjustHue(unsigned val = 0x8000);

        /* adjust saturation */
        void adjustSaturation(unsigned val = 0x8000);
};

#endif /*MMSVIDEOCTRL_H_*/

#endif
