#ifndef SETTINGSMENUINTERFACE_H
#define SETTINGSMENUINTERFACE_H

#include "../MyArray.h"
#include "../MyVector.h"
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
    static int FONT = 3;
    static int FONT_SIZE = 1;

    void run();
    void drawScreen();
    void setTextSettings();
    void checkClick();
}

#endif // SETTINGSMENUINTERFACE_H
