#include "Menu.h"

Menu::Menu(const int& theme) :
    m_theme(theme), m_fileButton(40, 13, 80, 26, "File", 120, 80), m_settingsButton(130, 13, 100, 26, "Settings"), m_helpButton(210, 13, 60, 26, "Help", 80, 80) {
    m_fileButton.addOption("New");
    m_fileButton.addOption("Save");
    m_fileButton.addOption("Save as...");
    m_fileButton.addOption("Open");
}

void Menu::drawMenu(const int& x1, const int& y1, const int& x2, const int& y2) {
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(0, 0, x2, y2);
    setlinestyle(SOLID_LINE, 0, 1);
    m_fileButton.drawTextButton(6, 1, LIGHTGRAY);
    m_fileButton.border(BLACK);
    m_settingsButton.drawTextButton(6, 1, RGB(158, 158, 158));
    m_settingsButton.border(BLACK);
    m_helpButton.drawTextButton(6, 1, LIGHTGRAY);
    m_helpButton.border(BLACK);
}

//returns true if Space3D should redraw itself
bool Menu::getCommand(const int& x, const int& y) {
    if (m_fileButton.hitCollision(x, y)) {
        setlinestyle(SOLID_LINE, 0, 1);
        m_fileButton.toggleVisibillity(-1, 6, 1, LIGHTGRAY);
        if (!m_fileButton.isListVisible()) {
            return true;
        }
    }
    return false;
}
