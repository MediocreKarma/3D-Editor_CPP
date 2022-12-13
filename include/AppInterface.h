#ifndef APPINTERFACE_H
#define APPINTERFACE_H

#include <graphics.h>
#include <winbgim.h>
#include "Menu.h"

class AppInterface {
    public:
        AppInterface(const int& appWidth = 800, const int& appHeight = 600, const int& theme = 0, const int& languagePackage = 0);
        void run();
        void setSettings(const int& appWidth, const int& appHeight, const int& theme, const int& languagePackage);

    private:
        Menu m_menu;
        int m_appWidth;
        int m_appHeight;
        int m_theme;
        int m_languagePackage;
};

#endif // APPINTERFACE_H
