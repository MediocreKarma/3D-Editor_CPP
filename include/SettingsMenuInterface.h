#ifndef SETTINGSMENUINTERFACE_H
#define SETTINGSMENUINTERFACE_H

#include "MyArray.h"
#include "MyVector.h"
#include "AppTools.h"
#include "graphics.h"
#include "winbgim.h"
#include "AppInterface.h"
#include <stdio.h>

namespace SettingsMenuInterface
{
    static const int SETTINGS_WIDTH = 600;
    static const int SETTINGS_HEIGHT = 400;
    static const int BACKGROUND_COLOR = RGB(48, 47, 77);
    static const int HIGHLIGHT_COLOR = RGB(242, 195, 107);
    static const int NO_COLOR = -1;
    static const int LABEL_COLOR = RGB(108, 106, 175);
    static const int BUTTON_COLOR = RGB(182, 191, 232);
    static const int DROPLIST_COLOR = RGB(235, 235, 235);
    static const size_t ACCENT_COLOR_SIZE = ColorSchemes::ACCENTCOLOR_COUNT;
    static const size_t LABEL_SIZE = 4;
    static const size_t TEXTBUTTON_SIZE = 3;
    static const size_t FLAG_SIZE = 2;
    static const size_t DROPDOWN_SIZE = 5;
    static const size_t THEME_SIZE = 2;
    static const MyArray<int, FLAG_SIZE> themeFillColors = {
        RGB(182, 191, 232),
        RGB(132, 141, 182)
    };
    static const MyArray<MyArray<int, 2>, DROPDOWN_SIZE> resOptions = {
        {1920, 1080},
        {1600, 900},
        {1280, 720},
        {1000, 750},
        {800, 600}
    };
    static const int FONT = 3;
    static const int FONT_SIZE = 1;

    void run();
    void update();
    void checkSavedSettings();
    void saveSelection();
    void drawScreen();
    void createButtons();
    void setTextSettings();
    MyArray<TextButton, TEXTBUTTON_SIZE> initThemeButtons();
    MyArray<TextLabel, LABEL_SIZE> initLabels();
    MyArray<ImageButton, FLAG_SIZE> initImageButtons();
    DropdownButton<DROPDOWN_SIZE> buildDropdownButton();
    DropdownButton<DROPDOWN_SIZE> buildAccentColorDropdownButton();
    void drawThemeButtons(MyArray<TextButton, TEXTBUTTON_SIZE>& txtButtons);
    void drawLabels(MyArray<TextLabel, LABEL_SIZE>& labels);
    void drawFlagButtons(MyArray<ImageButton, FLAG_SIZE>& flagButtons);
    void settingsMenu(MyArray<TextButton, TEXTBUTTON_SIZE>& txtButtons, MyArray<ImageButton, FLAG_SIZE>& flagButtons,
                      TextButton& startButton, DropdownButton<DROPDOWN_SIZE>& ddButton,
                      DropdownButton<DROPDOWN_SIZE>& accentButton, MyArray<TextLabel, LABEL_SIZE>& labels);
    void initScreen();
    void draw(MyArray<TextButton, TEXTBUTTON_SIZE>& themeButtons, MyArray<ImageButton, FLAG_SIZE>& flagButtons,
                TextButton& startButton,  DropdownButton<DROPDOWN_SIZE>& ddButton, DropdownButton<DROPDOWN_SIZE>& accentButton, MyArray<TextLabel, LABEL_SIZE>& labels);
}

#endif // SETTINGSMENUINTERFACE_H
