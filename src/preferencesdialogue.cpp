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

#include "preferencesdialogue.h"

#include <qcheckbox.h>
#include <qlabel.h>
#include <qgridlayout.h>
#include <qpushbutton.h>
#include <qfiledialog.h>
#include <qimagereader.h>
#include <qtreewidget.h>
#include <qwhatsthis.h>
#include <qmimedatabase.h>
#include <qmimetype.h>

#include <QStandardItem>

#include <klocalizedstring.h>
#include <kconfigskeleton.h>
#include <kaboutapplicationdialog.h>
#include <kaboutdata.h>
#include <kstandardguiitem.h>
#include <kwindowsystem.h>
#include <krecentdirs.h>
#include <QtWidgets/QColorDialog>

#include "debug.h"
#include "settings.h"

//////////////////////////////////////////////////////////////////////////
//									//
//  PreferencesDialogue -- The container dialogue			//
//									//
//////////////////////////////////////////////////////////////////////////

PreferencesDialogue::PreferencesDialogue(bool soloMode, QWidget *pnt)
    : KPageDialog(pnt)
{
    setObjectName("PreferencesDialogue");
    setFaceType(KPageDialog::Auto);

    if (soloMode)
    {
        setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::Close|QDialogButtonBox::Help);
        KStandardGuiItem::assign(button(QDialogButtonBox::Close), KStandardGuiItem::Quit);
        connect(button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &PreferencesDialogue::saveSettings);
    }
    else
    {
        setStandardButtons(QDialogButtonBox::Ok|QDialogButtonBox::Cancel|QDialogButtonBox::Help);
        connect(button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &PreferencesDialogue::saveSettings);
        setWindowTitle(i18n("Settings"));
    }

    button(QDialogButtonBox::Help)->setText(i18nc("@action:button", "About..."));
    connect(button(QDialogButtonBox::Help), &QPushButton::clicked, this, &PreferencesDialogue::slotAbout);

    mWallpaperPage = new PreferencesWallpaperPage(this);
    KPageWidgetItem *page = addPage(mWallpaperPage, i18n("Backlight"));
    page->setIcon(QIcon::fromTheme("user-desktop"));

    setMinimumSize(500, 240);

    loadSettings();
    QString savedSize = Settings::preferencesDialogueSize();
    if (!savedSize.isEmpty()) restoreGeometry(QByteArray::fromBase64(savedSize.toLatin1()));
    int savedIndex = Settings::preferencesPageIndex();
    if (savedIndex>=0)
    {
        KPageView *view = qobject_cast<KPageView *>(pageWidget());
        Q_ASSERT(view!=NULL);
        view->setCurrentPage(view->model()->index(savedIndex, 0));
    }
}


PreferencesDialogue::~PreferencesDialogue()
{
    Settings::setPreferencesDialogueSize(QString::fromLocal8Bit(saveGeometry().toBase64()));
    KPageView *view = qobject_cast<KPageView *>(pageWidget());
    Q_ASSERT(view!=NULL);
    Settings::setPreferencesPageIndex(view->currentPage().row());

    Settings::self()->save();
}


void PreferencesDialogue::setBacklightColor(const QColor &color)
{
    PreferencesWallpaperPage *wpPage = qobject_cast<PreferencesWallpaperPage *>(mWallpaperPage);
    Q_ASSERT(wpPage!=NULL);
    wpPage->setBacklightColor(color);
}

//////////////////////////////////////////////////////////////////////////
//									//
//  loadSettings/saveSettings -- Load or save the dialogue settings	//
//  from the application configuration.					//
//									//
//////////////////////////////////////////////////////////////////////////

void PreferencesDialogue::loadSettings()
{
    qDebug();
    mWallpaperPage->loadSettings();
}


void PreferencesDialogue::saveSettings()
{
    qDebug();
    mWallpaperPage->saveSettings();
    Settings::self()->save();
}

//////////////////////////////////////////////////////////////////////////
//									//
//  PreferencesWallpaperPage -- Settings for the wallpaper switcher	//
//									//
//////////////////////////////////////////////////////////////////////////

PreferencesWallpaperPage::PreferencesWallpaperPage(QWidget *pnt)
    : PreferencesPage(pnt)
{
    QGridLayout *gl = new QGridLayout(this);
    setLayout(gl);

    KConfigSkeletonItem *ski = Settings::self()->enableSwitcherItem();
    Q_ASSERT(ski!=NULL);
    mEnableSwitcherCheck = new QCheckBox(ski->label(), this);
    mEnableSwitcherCheck->setToolTip(ski->toolTip());
    connect(mEnableSwitcherCheck, SIGNAL(toggled(bool)), SLOT(slotUpdateButtonStates()));
    gl->addWidget(mEnableSwitcherCheck, 0, 0, 1, -1, Qt::AlignLeft);

    mWallpaperList = new QTreeWidget(this);
    mWallpaperList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mWallpaperList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    mWallpaperList->setUniformRowHeights(true);
    mWallpaperList->setSortingEnabled(false);
    mWallpaperList->setSelectionMode(QAbstractItemView::SingleSelection);
    mWallpaperList->setSelectionBehavior(QAbstractItemView::SelectRows);
    mWallpaperList->setRootIsDecorated(false);
    mWallpaperList->setIconSize(QSize(32, 32));
    mWallpaperList->setTextElideMode(Qt::ElideMiddle);

    QStringList headers;
    headers << i18n("Desktop") << i18n("Color");
    mWallpaperList->setHeaderLabels(headers);
    mWallpaperList->setColumnCount(2);

    connect(mWallpaperList, SIGNAL(itemSelectionChanged()), SLOT(slotUpdateButtonStates()));
    connect(mWallpaperList, SIGNAL(itemDoubleClicked(QTreeWidgetItem *,int)), SLOT(slotSetBacklightColor(QTreeWidgetItem *)));
    gl->addWidget(mWallpaperList, 1, 0, 1, -1);

    mSetWallpaperButton = new QPushButton(this);
    mSetWallpaperButton->setText(i18nc("@action:button", "Set backlight color..."));
    mSetWallpaperButton->setIcon(QIcon::fromTheme("view-catalog"));

    connect(mSetWallpaperButton, SIGNAL(clicked(bool)), SLOT(slotSetBacklightColor()));
    gl->addWidget(mSetWallpaperButton, 2, 0, Qt::AlignLeft);

    gl->setRowStretch(1, 1);
}


static void setItemColor(QTreeWidgetItem *item, const QColor &color)
{
    item->setText(1, color.name());
    //if (!file.isEmpty()) item->setIcon(1, QIcon(file));
}


void PreferencesWallpaperPage::loadSettings()
{
    mEnableSwitcherCheck->setChecked(Settings::enableSwitcher());

    KConfigSkeletonItem *ski = Settings::self()->wallpaperForDesktopItem();
    Q_ASSERT(ski!=NULL);
    const KConfigGroup grp = Settings::self()->config()->group(ski->group());

    mWallpaperList->clear();

    const int numDesktops = KWindowSystem::numberOfDesktops();
    for (int i = 1; i<=numDesktops; ++i)
    {
        const QString name = KWindowSystem::desktopName(i);
        qDebug() << i << name;

        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(0, i18n("%1 - %2", i, name));
        item->setData(0, Qt::UserRole, i);
        setItemColor(item, grp.readEntry(QString::number(i), ""));

        mWallpaperList->addTopLevelItem(item);
    }

    slotUpdateButtonStates();
}


void PreferencesWallpaperPage::saveSettings()
{
    qDebug();
    Settings::setEnableSwitcher(mEnableSwitcherCheck->isChecked());

    KConfigSkeletonItem *ski = Settings::self()->wallpaperForDesktopItem();
    Q_ASSERT(ski!=NULL);
    KConfigGroup grp = Settings::self()->config()->group(ski->group());

    for (int i = 0; i<mWallpaperList->topLevelItemCount(); ++i)
    {
        const QTreeWidgetItem *item = mWallpaperList->topLevelItem(i);
        grp.writeEntry(QString::number(i+1), item->text(1));
    }
}


void PreferencesWallpaperPage::slotUpdateButtonStates()
{
    const bool enabled = mEnableSwitcherCheck->isChecked();

    mWallpaperList->setEnabled(enabled);
    mSetWallpaperButton->setEnabled(enabled && !mWallpaperList->selectedItems().isEmpty());
}


void PreferencesWallpaperPage::slotSetBacklightColor(QTreeWidgetItem *item)
{
    if (item==NULL)					// by button click
    {							// set for current selection
        QList<QTreeWidgetItem *> selItems = mWallpaperList->selectedItems();
        if (selItems.count()!=1) return;
        item = selItems.first();
    }

    const int desktopNum = item->data(0, Qt::UserRole).toInt();
    qDebug() << "for desktop" << desktopNum;

    // read previous color
    KConfigSkeletonItem *ski = Settings::self()->wallpaperForDesktopItem();
    Q_ASSERT(ski!=NULL);
    const KConfigGroup grp = Settings::self()->config()->group(ski->group());
    QColor prevColor = grp.readEntry(QString::number(desktopNum), "");

    // show color picker dialog
    QColor color = QColorDialog::getColor(prevColor);

    // save color
    setItemColor(item, color);

    saveSettings();
    Settings::self()->save();
}


void PreferencesDialogue::slotAbout()
{
    KAboutApplicationDialog d(KAboutData::applicationData(), this);
    d.exec();
}
