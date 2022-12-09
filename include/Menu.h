#ifndef MENU_H
#define MENU_H

#include "AppTools.h"
#include "Space3D.h"
#include "FileHandler.h"

class Space3D;

class Menu {
    public:

        Menu(const int& m_theme = 0);
        void clear();
        void draw();
        bool getCommand(const int& xClick, const int& yClick);
        bool newSpace();
        char saveSpace();
        bool openSpace();
        void setSpaceId(Space3D* space);
        void setBorder(const int& x1, const int& y1, const int& x2, const int& y2);

    private:
        int x1, y1, x2, y2;
        int m_theme;
        DropdownButton<4> m_fileButton;
        TextButton m_settingsButton;
        DropdownButton<3> m_helpButton;
        bool m_newSpaceFlag;
        char m_saveSpaceFlag;
        bool m_openSpaceFlag;
        Space3D m_space;
        FileHandler m_fileGetter;

        void drawMenu();
        void initSpace();
        void swapPages();
};

#endif // MENU_H
