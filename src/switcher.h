/////////////////////////////////////////////////// -*- mode:c++; -*- ////
//									//
//  This file is part of Wallpaper Switcher, a virtual desktop		//
//  wallpaper (background image) switcher for KDE Plasma 5.		//
//									//
//  Copyright (c) 2016 Jonathan Marten <jjm@keelhaul.me.uk>		//
//  Home page:  http://github.com/martenjj/wallpaperswitch		//
//									//
//  This program is free software; you can redistribute it and/or	//
//  modify it under the terms of the GNU General Public License as	//
//  published by the Free Software Foundation; either version 2 of	//
//  the License, or (at your option) any later version.			//
//									//
//  It is distributed in the hope that it will be useful, but		//
//  WITHOUT ANY WARRANTY; without even the implied warranty of		//
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	//
//  GNU General Public License for more details.			//
//									//
//  You should have received a copy of the GNU General Public		//
//  License along with this program; see the file COPYING for further	//
//  details.  If not, see http://www.gnu.org/licenses/			//
//									//
//////////////////////////////////////////////////////////////////////////

#ifndef SWITCHER_H
#define SWITCHER_H

#include <qobject.h>
#include <QtGui/QColor>
#include "MSIKeyboard.h"

class QDir;



class Switcher : public QObject
{
    Q_OBJECT

public:
    explicit Switcher(QObject *pnt = NULL);
    virtual ~Switcher();

    static MSIKeyboard *getKeyboard();
    void setColor(QColor color);
    void fadeColor(QColor from, QColor to, int duration);

public slots:
    void slotDesktopChanged(int desktop = 0);
    void fadeEffect();

private:
    static MSIKeyboard *keyboard;

    /// current keyboard backlight color
    QColor currentColor;
    /// start color for fade effect
    QColor fadeFrom;
    /// destination color for fade effect
    QColor fadeTo;
    /// current step in fade effect
    int fadeStep;
    /// numer ob steps in fade effect
    int fadeLength;
    /// time in ms for single step
    int stepDuration;
};



#endif							// SWITCHER_H
