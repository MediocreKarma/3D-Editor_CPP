#ifndef MENU_H
#define MENU_H

#include "AppTools.h"
#include "SettingsMenuInterface.h"


class Menu {
    public:
        Menu(const int& m_theme = 0);
        void clear(const int& x1, const int& y1, const int& x2, const int& y2);
        void drawMenu(const int& x1, const int& y1, const int& x2, const int& y2);
        bool getCommand(const int& xClick, const int& yClick);
        bool newSpace();
        char saveSpace();
        bool openSpace();

    private:
        int m_theme;
        DropdownButton<4> m_fileButton;
        TextButton m_settingsButton;
        DropdownButton<3> m_helpButton;
        bool m_newSpaceFlag;
        char m_saveSpaceFlag;
        bool m_openSpaceFlag;
};

#endif // MENU_H
