##########################################################################
##									##
##  This file is part of Wallpaper Switcher, a virtual desktop		##
##  wallpaper (background image) switcher for KDE Plasma 5.		##
##									##
##  Copyright (c) 2016 Jonathan Marten <jjm@keelhaul.me.uk>		##
##  Home page:  http://github.com/martenjj/wallpaperswitch		##
##									##
##  This program is free software; you can redistribute it and/or	##
##  modify it under the terms of the GNU General Public License as	##
##  published by the Free Software Foundation; either version 2 of	##
##  the License, or (at your option) any later version.			##
##									##
##  It is distributed in the hope that it will be useful, but		##
##  WITHOUT ANY WARRANTY; without even the implied warranty of		##
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	##
##  GNU General Public License for more details.			##
##									##
##  You should have received a copy of the GNU General Public		##
##  License along with this program; see the file COPYING for further	##
##  details.  If not, see http://www.gnu.org/licenses/			##
##									##
##########################################################################

project(backlight-switch)

#########################################################################
#									#
#  Requirements								#
#									#
#########################################################################

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Core Widgets Quick)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS I18n CoreAddons Config ConfigWidgets WidgetsAddons IconThemes XmlGui WindowSystem KIO Notifications DBusAddons Crash)

check_function_exists(strerror HAVE_STRERROR)
if (HAVE_STRERROR)
  add_definitions(-DHAVE_STRERROR)
  check_include_file(errno.h HAVE_ERRNO_H)
  if (HAVE_ERRNO_H)
  add_definitions(-DHAVE_ERRNO_H)
  endif (HAVE_ERRNO_H)
endif (HAVE_STRERROR)

#########################################################################
#									#
#  Building the target							#
#									#
#########################################################################

include_directories("${CMAKE_CURRENT_BINARY_DIR}/..")

set(backlightswitch_SRCS
  main.cpp
  backlightswitch.cpp
  preferencesdialogue.cpp
  switcher.cpp
  systemtraywidget.cpp
  MSIKeyboard.cpp
)
kconfig_add_kcfg_files(backlightswitch_SRCS settings.kcfgc)

add_executable(backlight-switch ${backlightswitch_SRCS})
add_dependencies(backlight-switch vcsversion)

target_link_libraries(backlight-switch
  Qt5::Core
  Qt5::Widgets
  KF5::I18n
  KF5::CoreAddons
  KF5::ConfigCore KF5::ConfigWidgets
  KF5::WidgetsAddons
  KF5::IconThemes
  KF5::XmlGui
  KF5::WindowSystem
  KF5::KIOCore KF5::KIOFileWidgets
  KF5::Notifications
  KF5::DBusAddons
  KF5::Crash
  ${LIBUSB_LIBRARIES}
)

#########################################################################
#									#
#  Installation								#
#									#
#########################################################################

install(TARGETS backlight-switch DESTINATION ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
INSTALL(FILES ../udev/90-msi-keyboard.rules DESTINATION /etc/udev/rules.d)
