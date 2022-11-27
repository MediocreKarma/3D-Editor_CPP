#ifndef SETTINGSMENUINTERFACE_H
#define SETTINGSMENUINTERFACE_H

#include "MyArray.h"
#include "MyVector.h"
#include <AppTools.h>
#include "graphics.h"
#include "winbgim.h"
#include"MenuInterface.h"

namespace SettingsMenuInterface
{
    static const int SETTINGS_WIDTH = 600;
    static const int SETTINGS_HEIGHT = 400;
    static const int BACKGROUND_COLOR = RGB(200, 200, 200);
    static const int LABEL_COLOR = RGB(160, 160, 160);
    static const int BUTTON_COLOR = RGB(160, 60, 160);
    static const size_t LABEL_SIZE = 3;
    static const size_t TEXTBUTTON_SIZE = 3;
    static const size_t FLAG_SIZE = 2;
    static const size_t DROPDOWN_SIZE = 5;
    static const int FONT = 3;
    static const int FONT_SIZE = 1;
    static int resolutionX = 600;
    static int resolutionY = 600;
    static int theme = BLACK;
    static int language = 0;

    void run();
    void drawScreen();
    void createButtons();
    void setTextSettings();
    void changeResolution(const char* resSelect);
    MyArray<TextButton, TEXTBUTTON_SIZE> initThemeButtons();
    MyArray<TextLabel, LABEL_SIZE> initLabels();
    MyArray<ImageButton, FLAG_SIZE> initImageButtons();
    DropdownButton<DROPDOWN_SIZE> buildDropdownButton();
    void drawThemeButtons(MyArray<TextButton, TEXTBUTTON_SIZE>& txtButtons);
    void drawLabels(MyArray<TextLabel, LABEL_SIZE>& labels);
    void drawFlagButtons(MyArray<ImageButton, FLAG_SIZE>& flagButtons);
    bool settingsMenu(MyArray<TextButton, TEXTBUTTON_SIZE>& txtButtons, MyArray<ImageButton, FLAG_SIZE>& flagButtons,
                      TextButton& startButton, DropdownButton<DROPDOWN_SIZE>& ddButton);
}

#endif // SETTINGSMENUINTERFACE_H
