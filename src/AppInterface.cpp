#include "AppInterface.h"
#include <iostream>

AppInterface::AppInterface(const int& appWidth, const int& appHeight, const int& theme, const int& languagePackage) :
    m_appWidth(appWidth), m_appHeight(appHeight), m_theme(theme), m_languagePackage(languagePackage) {}

double AppInterface::maxRadius() const {
    return m_maxRadius;
}

void AppInterface::getKeyPress(char& c) {
    if(kbhit()) {
        c = getch();
    }

}

void AppInterface::getClick(int& xClick, int& yClick) {
    getmouseclick(WM_LBUTTONDOWN, xClick, yClick);
}

void AppInterface::run() {
    initwindow(m_appWidth, m_appHeight, "Editor 3D");
    setactivepage(0);
    setvisualpage(1);
    Menu menu(m_theme, m_appWidth, m_appHeight);
    menu.setBorder(0, 0, m_appWidth, 27);
    menu.draw();
    while (true) {
        int xClick, yClick;
        getmouseclick(WM_LBUTTONDOWN, xClick, yClick);
        if (menu.getCommand(xClick, yClick)) {
            menu.draw();
        }
    }
}
