#ifndef SETTINGSMENUINTERFACE_H
#define SETTINGSMENUINTERFACE_H

#include <MyArray.h>
#include <MyVector.h>
#include <graphics.h>
#include <winbgim.h>

namespace SettingsMenuInterface
{
    static const int SETTINGS_WIDTH = 600;
    static const int SETTINGS_HEIGHT = 400;
    static const char* SETTINGS_TITLE = "Settings";
    static const int BACKGROUND_COLOR = RGB(200, 200, 200);

    class RectangleButton {
    public:
        RectangleButton();
        RectangleButton(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen);
        void drawButton(const int& color);
        bool isClicked(const int& x, const int& y) const;
    private:
        int m_xCenter;
        int m_yCenter;
        int m_xLen;
        int m_yLen;
    };
    class FlagButton : RectangleButton {

    };
   // class ScrollableDropdownList;

    void run();
    void drawScreen();
    //void drawOptions();
    //void checkForSelection();
    //void selectResolution();
}

#endif // SETTINGSMENUINTERFACE_H
