#ifndef SETTINGSMENUINTERFACE_H
#define SETTINGSMENUINTERFACE_H

#include <MyArray.h>
#include <MyVector.h>
#include <graphics.h>
#include <winbgim.h>

namespace SettingsMenuInterface
{
    class RectangleButton {
        RectangleButton(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen);
        void drawButton(const int& color);
        bool isClicked(const int& x, const int& y) const;
    };
    class FlagButton;
   // class ScrollableDropdownList;

    void run();
    void drawScreen();
    //void drawOptions();
    //void checkForSelection();
    //void selectResolution();
}

#endif // SETTINGSMENUINTERFACE_H
