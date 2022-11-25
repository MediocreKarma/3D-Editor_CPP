#ifndef SETTINGSMENUINTERFACE_H
#define SETTINGSMENUINTERFACE_H

#include "MyArray.h"
#include "MyVector.h"
#include "graphics.h"
#include "winbgim.h"

namespace SettingsMenuInterface
{
    static const int SETTINGS_WIDTH = 600;
    static const int SETTINGS_HEIGHT = 400;
    static const char* SETTINGS_TITLE = "Settings";
    static const int BACKGROUND_COLOR = RGB(200, 200, 200);
    static const int LABEL_COLOR = RGB(160, 160, 160);
    static const int BUTTON_COLOR = RGB(160, 60, 160);
    static int TEXT_COLOR = BLACK;
    static int LIMBA = 0;

    class TextLabel {
    public:
        TextLabel();
        TextLabel(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const MyArray<char, 256>& text);
        void drawTextLabel();
    protected:
        int m_xCenter;
        int m_yCenter;
        int m_xLen;
        int m_yLen;
        MyArray<char, 256>m_text;
    };

    class RectangleButton {
    public:
        RectangleButton();
        RectangleButton(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen);
        void drawButton(const int& color);
        bool isClicked(const int& x, const int& y) const;
    protected:
        int m_xCenter;
        int m_yCenter;
        int m_xLen;
        int m_yLen;
    };

    class TextButton : public RectangleButton {
    public:
        TextButton();
        TextButton(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const MyArray<char, 256>& text);
        void drawTextButton();
    private:
        MyArray<char, 256> m_text;
    };

   // class ScrollableDropdownList;

    void run();
    void drawScreen();
    void setTextSettings();
    //void drawOptions();
    void checkClick();
    //void checkForSelection();
    //void selectResolution();
}

#endif // SETTINGSMENUINTERFACE_H
