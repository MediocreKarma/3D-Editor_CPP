#include "Menu.h"

Menu::Menu(const int& theme) :
    m_theme(theme), m_fileButton(40, 13, 80, 26, "File", 120, 80), m_settingsButton(130, 13, 100, 26, "Settings"), m_helpButton(210, 13, 60, 26, "Help", 80, 80),
    m_newSpaceFlag(false), m_saveSpaceFlag(false), m_openSpaceFlag(false) {
    m_fileButton.addOption("New");
    m_fileButton.addOption("Save");
    m_fileButton.addOption("Save as...");
    m_fileButton.addOption("Open");
}

void Menu::drawMenu(const int& x1, const int& y1, const int& x2, const int& y2) {
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(0, 0, x2, y2);
    bar(0, 0, x1, y1);
    setlinestyle(SOLID_LINE, 0, 1);
    m_fileButton.drawTextButton(6, 1, LIGHTGRAY);
    m_fileButton.border(BLACK);
    m_settingsButton.drawTextButton(6, 1, RGB(158, 158, 158));
    m_settingsButton.border(BLACK);
    m_helpButton.drawTextButton(6, 1, LIGHTGRAY);
    m_helpButton.border(BLACK);
}

#include <iostream>

//returns true if Space3D should redraw itself
bool Menu::getCommand(const int& x, const int& y) {
    if (m_fileButton.isListVisible()) {
        int pos = m_fileButton.listHitCollision(x, y);
        if (pos >= 0) {
            if (pos == 0) {
                m_newSpaceFlag = 1;
            }
            else if (pos == 1 || pos == 2) {
                m_saveSpaceFlag = pos;
            }
            else if (pos == 3) {
                m_openSpaceFlag = 1;
            }
            m_fileButton.hideList(-1);
            return true;
        }
    }
    if (m_fileButton.hitCollision(x, y)) {
        setlinestyle(SOLID_LINE, 0, 1);
        m_fileButton.toggleVisibillity(-1, 6, 1, LIGHTGRAY);
        if (!m_fileButton.isListVisible()) {
            return true;
        }
    }
    return false;
}

bool Menu::newSpace() {
    if (m_newSpaceFlag) {
        m_newSpaceFlag = false;
        return true;
    }
    return false;
}

char Menu::saveSpace() {
    if (m_saveSpaceFlag) {
        char returnVal = m_saveSpaceFlag;
        m_saveSpaceFlag = 0;
        return returnVal;
    }
    return false;
}

bool Menu::openSpace() {
    if (m_openSpaceFlag) {
        m_openSpaceFlag = false;
        return true;
    }
    return false;
}
