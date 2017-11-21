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

#include <cstdio>
#include "MSIKeyboard.h"

MSIKeyboard::MSIKeyboard() {

}

MSIKeyboard::~MSIKeyboard() {

}

bool MSIKeyboard::initKeyboard() {
    wchar_t wstr[MAX_STR];

    hid_init();
    //handle = hid_open(6000, 65280, NULL);
    handle = hid_open(0x1770, 0xff00, NULL);

    if (handle == nullptr) {
        printf("failed to open device\n");
        return false;
    }

    hid_get_manufacturer_string(handle, wstr, MAX_STR);
    printf("Manufacturer String: %ls\n", wstr);
    hid_get_product_string(handle, wstr, MAX_STR);
    printf("Product String: %ls\n", wstr);
    hid_get_serial_number_string(handle, wstr, MAX_STR);
    printf("Serial Number String: %ls", wstr);
    printf("\n");

    return true;
}

bool MSIKeyboard::setColor(MSIKeyboard::Region region, MSIKeyboard::Color color) {
    return setColor(region, color.red, color.green, color.blue);
}

bool MSIKeyboard::setColor(MSIKeyboard::Region region, unsigned char red, unsigned char green, unsigned char blue) {
    if (handle == nullptr) {
        printf("invalid device\n");
        return false;
    }

    unsigned char buf[8];
    buf[0] = 1;
    buf[1] = 2;
    buf[2] = 64;
    buf[3] = region;
    buf[4] = red;
    buf[5] = green;
    buf[6] = blue;
    buf[7] = 0;

    int ret = hid_send_feature_report(handle, buf, 8);
    if (ret == -1) {
        printf("failed to set color: %d\n", ret);
        return false;
    }

    return true;
}

