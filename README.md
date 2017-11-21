Backlight Switcher for KDE Plasma 5
===================================

This little system tray application changes the backlight color of 
MSI keyboards depending on the current vitual desktop.

You need a steelseries keyboard to use this tool.


Building and installing
-----------------------

The application is built and installed in much the same way as any
other KDE application.  Assuming that you have Qt 5, KDE Frameworks 5
and the Plasma desktop installed, go to a suitable build location
(e.g. your home directory) and do:

     git clone https://github.com/ofenrohr/backlight-switch.git
     cd backlight-switch
     mkdir build
     cd build
     cmake ..
     make
     sudo make install

Building requires CMake and also KDE Frameworks development headers
and libraries.  Depending on your distro, you may need to install
additional packages for these.  If you are running Kubuntu then see
the README.kubuntu file for a list of packages required.


Running and configuration
-------------------------

After the files are installed above, start the switcher either from
whichever desktop launcher you use (it will appear in the "Utilities"
category), or by typing the command 'backlight-switch' in a terminal or
KRunner (summoned by typing Alt-F2).  The "desktop" icon will appear
in the system tray.

If this is the first time that the application has been run the the
configuration dialogue will appear automatically.  Otherwise, click
the right mouse button over the system tray icon and select the
"Configure..." option from the menu.  Turn on the "Enable Switching" 
check box if necessary.

Each currently configured virtual desktop will be listed there.  To
select the backlight color for a desktop, double click the entry or
select it and click the "Set Color" button.  From the color picker
dialogue that appears, choose a color.  Repeat the same for each
virtual desktop.

Switching can be enabled or disabled using the "Enable Switching"
option on the system tray popup menu.


Problems?
---------

If you are able to build and install the application, but it will not
run or it is not able to change the keyboard backlight color, then try
starting it from a terminal window within the Plasma desktop.  Observe
the debugging messages for anything about missing files or any other
problems.

If neither of this resolves the problem then please raise an issue in
GitHub (http://github.com/ofenrohr/backlight-switch).  Do not raise a
bug against KDE Plasma.


Original application
--------------------

Thanks to Jonathan Marten for creating the wallpaperswitcher application 
that this application is built upon.
