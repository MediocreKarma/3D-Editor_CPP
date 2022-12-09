#include "Menu.h"
#include <iostream>

Menu::Menu(const int& theme) :
    x1(), y1(), x2(), y2(), m_theme(theme), m_fileButton(40, 13, 80, 26, "File", 120, 80), m_settingsButton(130, 13, 100, 26, "Settings"), m_helpButton(210, 13, 60, 26, "Help", 80, 80),
    m_newSpaceFlag(false), m_saveSpaceFlag(false), m_openSpaceFlag(false), m_space(), m_fileGetter() {
    m_fileButton.addOption("New");
    m_fileButton.addOption("Save");
    m_fileButton.addOption("Save as...");
    m_fileButton.addOption("Open");
    initSpace();
    Mesh cube;
    cube.addPoint(-100, -100, -100);
    cube.addPoint(-100, -100, 100);
    cube.addPoint(-100, 100, -100);
    cube.addPoint(100, -100, -100);
    cube.addPoint(-100, 100, 100);
    cube.addPoint(100, -100, 100);
    cube.addPoint(100, 100, -100);
    cube.addPoint(100, 100, 100);
    cube.addIndexConnections(0, {1, 2, 3});
    cube.addIndexConnections(1, {0, 4, 5});
    cube.addIndexConnections(2, {0, 4, 6});
    cube.addIndexConnections(3, {0, 5, 6});
    cube.addIndexConnections(4, {1, 2, 7});
    cube.addIndexConnections(5, {1, 3, 7});
    cube.addIndexConnections(6, {2, 3, 7});
    cube.updateCenterPoint();
    cube.translate(500, 0, 0);
    m_space.addMesh(cube);
    cube.translate(0, 600, 0);
    m_space.addMesh(cube);
    cube.translate(0, 600, 0);
    m_space.addMesh(cube);
    cube.translate(0, 600, 0);
    m_space.addMesh(cube);
    cube.translate(0, 600, 0);
    m_space.addMesh(cube);
}

void Menu::setBorder(const int& x1_, const int& y1_, const int& x2_, const int& y2_) {
    x1 = x1_;
    y1 = y1_;
    x2 = x2_;
    y1 = y2_;
}

void Menu::drawMenu() {
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(0, 0, x2, y2);
    bar(0, 0, x1, y1);
    setlinestyle(SOLID_LINE, 0, 1);
    m_fileButton.drawTextButton(3, 1, LIGHTGRAY);
    m_fileButton.border(BLACK);
    m_settingsButton.drawTextButton(3, 1, RGB(158, 158, 158));
    m_settingsButton.border(BLACK);
    m_helpButton.drawTextButton(3, 1, LIGHTGRAY);
    m_helpButton.border(BLACK);
    if (m_fileButton.isListVisible()) {
        m_fileButton.showList(3, 1, LIGHTGRAY);
    }
}

void Menu::initSpace() {
    m_space = Space3D(-2500, m_theme);
    m_space.setCorners(0, 27, 1280, 720);
}

void Menu::draw() {
    m_space.run();
    drawMenu();
    swapbuffers();
}

//returns true if Space3D should redraw itself
bool Menu::getCommand(const int& x, const int& y) {
    if (m_fileButton.isListVisible()) {
        int pos = m_fileButton.listHitCollision(x, y);
        if (pos >= 0) {
            if (pos == 0) {
                initSpace();
            }
            else if (pos == 1 || pos == 2) {
                //m_saveSpaceFlag = pos;
            }
            else if (pos == 3) {
                //m_openSpaceFlag = 1;
            }
            m_fileButton.setVisibility(false);
            return true;
        }
    }
    if (m_fileButton.hitCollision(x, y)) {
        m_fileButton.setVisibility(!m_fileButton.isListVisible());
        return 1;
    }
    if (m_space.getCommand(x, y)) {
        return true;
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
