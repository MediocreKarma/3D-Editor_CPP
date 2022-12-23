#include "AppInterface.h"
#include <iostream>

AppInterface::AppInterface(const int& appWidth, const int& appHeight, const int& theme, const int& languagePackage) :
    m_menu(), m_appWidth(appWidth), m_appHeight(appHeight), m_theme(theme), m_languagePackage(languagePackage) {}

void AppInterface::setSettings(const int& appWidth, const int& appHeight, const int& theme, const int& languagePackage) {
    m_appWidth = appWidth;
    m_appHeight = appHeight;
    m_theme = theme;
    m_languagePackage = languagePackage;
}

void AppInterface::run() {
    initwindow(m_appWidth, m_appHeight, "Editor 3D");
    setactivepage(0);
    setvisualpage(1);
    m_menu.setSettings(m_theme, m_appWidth, m_appHeight);
    m_menu.setBorder(0, 0, m_appWidth, 27);
    m_menu.draw();
    while (true) {
        int xClick, yClick, xRightClick, yRightClick;
        getmouseclick(WM_LBUTTONDOWN, xClick, yClick);
        getmouseclick(WM_RBUTTONDOWN, xRightClick, yRightClick);
        if (m_menu.getCommand(xClick, yClick)) {
            m_menu.draw();
        }
        else if (m_menu.getRightClickCommand(xRightClick, yRightClick)) {
            m_menu.draw();
        }
        else if (m_menu.returnToSettingsFlag()){
            return;
        }
    }
}
