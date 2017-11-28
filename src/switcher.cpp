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


//////////////////////////////////////////////////////////////////////////
//									//
//  Include files							//
//									//
//////////////////////////////////////////////////////////////////////////

#include "switcher.h"

#ifdef HAVE_STRERROR
#include <string.h>
#endif
#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

#include <qfile.h>
#include <qdir.h>
#include <qstandardpaths.h>
#include <qtimer.h>

#include <kconfigskeleton.h>
#include <kwindowsystem.h>
#include <kdirwatch.h>

#include "debug.h"
#include "settings.h"

//////////////////////////////////////////////////////////////////////////
//									//
//  Debugging switches							//
//									//
//////////////////////////////////////////////////////////////////////////

#define DEBUG_CHANGE

//////////////////////////////////////////////////////////////////////////
//									//
//  Constructor/destructor					 	//
//									//
//////////////////////////////////////////////////////////////////////////

Switcher::Switcher(QObject *pnt)
    : QObject(pnt)
{
    qDebug();

    getKeyboard()->initKeyboard();

    connect(KWindowSystem::self(), SIGNAL(currentDesktopChanged(int)), SLOT(slotDesktopChanged(int)));
    QTimer::singleShot(0, this, SLOT(slotDesktopChanged()));	// set for current desktop

    //connect(KDirWatch::self(), SIGNAL(dirty(const QString &)), SLOT(slotFileChanged(const QString &)));
}


Switcher::~Switcher()
{
}

//////////////////////////////////////////////////////////////////////////
//									//
//									//
//////////////////////////////////////////////////////////////////////////

void Switcher::slotDesktopChanged(int desktop) {
    if (!Settings::enableSwitcher()) return;

    if (desktop == 0) desktop = KWindowSystem::currentDesktop();
#ifdef DEBUG_CHANGE
    qDebug() << "to" << desktop;
#endif // DEBUG_CHANGE

    KConfigSkeletonItem *ski = Settings::self()->wallpaperForDesktopItem();
    Q_ASSERT(ski != NULL);
    const KConfigGroup grp = Settings::self()->config()->group(ski->group());
    QColor color = grp.readEntry(QString::number(desktop), "");
    setColor(color);
}

void Switcher::setColor(QColor color) {
    MSIKeyboard::Color keyboardColor { (unsigned char) color.red(), (unsigned char) color.green(), (unsigned char) color.blue() };
    getKeyboard()->setColor(MSIKeyboard::RegionLeft, keyboardColor);
    getKeyboard()->setColor(MSIKeyboard::RegionMiddle, keyboardColor);
    getKeyboard()->setColor(MSIKeyboard::RegionRight, keyboardColor);
}

MSIKeyboard *Switcher::getKeyboard() {
    if (Switcher::keyboard == nullptr) {
        Switcher::keyboard = new MSIKeyboard();
    }
    return Switcher::keyboard;
}


void Switcher::slotFileChanged(const QString &file)
{
#ifdef DEBUG_CHANGE
    qDebug() << file;
#endif // DEBUG_CHANGE
    slotDesktopChanged(0);				// update for current desktop
}

MSIKeyboard* Switcher::keyboard;

