/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MSI_KEYBOARD_LIGHTS_MSIKEYBOARD_H
#define MSI_KEYBOARD_LIGHTS_MSIKEYBOARD_H

#include <hidapi.h>

class MSIKeyboard {
public:
    enum Region {
        RegionLeft = 1,
        RegionMiddle = 2,
        RegionRight = 3
    };

    typedef struct Color_t {
        unsigned char red;
        unsigned char green;
        unsigned char blue;
    } Color;

    MSIKeyboard();
    ~MSIKeyboard();

    bool initKeyboard();
    bool setColor(Region region, unsigned char red, unsigned  char green, unsigned char blue);
    bool setColor(Region region, Color color);

private:
    int MAX_STR = 255;
    hid_device *handle;
};


#endif //MSI_KEYBOARD_LIGHTS_MSIKEYBOARD_H
