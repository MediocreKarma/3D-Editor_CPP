#ifndef MENU_H
#define MENU_H

#include "AppTools.h"
#include "Space3D.h"
#include "FileHandler.h"
#include "ObjectCreator.h"
#include <cstdlib>
#include <cstdio>

class Menu {
    public:

        Menu(const int theme = 0, int language = 0, const int appWidth = 1280, const int appHeight = 720);
        void setSettings(const int theme = 0, int language = 0, const int appWidth = 1280, const int appHeight = 720);
        void draw();
        bool getCommand(const int xClick, const int yClick);
        bool getRightClickCommand(const int x, const int y);
        void setBorder(const int x1, const int y1, const int x2, const int y2);
        bool returnToSettingsFlag();

    private:
        int x1, y1, x2, y2;
        int m_appWidth;
        int m_appHeight;
        int m_theme;
        int m_language;
        DropdownButton<4> m_fileButton;
        TextButton m_settingsButton;
        DropdownButton<3> m_helpButton;
        Space3D m_space;
        FileHandler m_fileGetter;
        bool m_settingsMenuFlag;

        bool getKeyCommand();
        void drawMenu();
        void initSpace();
        void swapPages();
        void saveSpace3D(Space3D& space, const char saveType);
        void openSpace3D(Space3D& space);
};

#endif // MENU_H
