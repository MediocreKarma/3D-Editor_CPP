#include "Menu.h"
#include <iostream>
#include "SettingsMenuInterface.h"

Menu::Menu(const int& theme, const int& appWidth, const int& appHeight) :
    x1(), y1(), x2(), y2(), m_appWidth(appWidth), m_appHeight(appHeight), m_theme(theme), m_fileButton(40, 13, 80, 26, "File", 120, 80),
    m_settingsButton(130, 13, 100, 26, "Settings"), m_helpButton(210, 13, 60, 26, "Help", 80, 80), m_space(-2500, m_theme, this), m_fileGetter(), m_settingsMenuFlag(false) {
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

void Menu::setSettings(const int& theme, const int& appWidth, const int& appHeight) {
    m_theme = theme;
    m_appWidth = appWidth;
    m_appHeight = appHeight;
    m_space.setCorners(0, 27, m_appWidth, m_appHeight);
    m_space.setTheme(theme);
    m_space.update();
    m_space.render();
}

void Menu::setBorder(const int& x1_, const int& y1_, const int& x2_, const int& y2_) {
    x1 = x1_;
    y1 = y1_;
    x2 = x2_;
    y2 = y2_;
}

void Menu::saveSpace3D(Space3D& space, const char& saveType) {
    if (space.isLinkedWithFile() && saveType != 2) {
        FILE* fp = fopen(space.getLinkedFileName().data(), "w");
        space.fprint(fp);
        fclose(fp);
        return;
    }
    int getCurrentWindowNumber = getcurrentwindow();
    MyArray<char, 512> savepath = m_fileGetter.userSavePath();
    setcurrentwindow(getCurrentWindowNumber);
    if (!savepath[0]) {
        return;
    }
    FILE* fp = fopen(savepath.data(), "w");
    if (!fp) {
        showerrorbox("Bad file path!");
    }
    else {
        space.fprint(fp);
        space.setLinkedFileName(savepath);
    }
    fclose(fp);
}

void Menu::openSpace3D(Space3D& space) {
    int getCurrentWindowNumber = getcurrentwindow();
    MyArray<char, 512> openpath = m_fileGetter.userOpenPath();
    setcurrentwindow(getCurrentWindowNumber);
    if (!openpath[0]) {
        return;
    }
    FILE* fp = fopen(openpath.data(), "r");
    Space3D tempSpace(-2500, m_theme, this);
    tempSpace.setCorners(0, 27, m_appWidth, m_appHeight);
    if (!fp) {
        showerrorbox("File not found!");
    }
    else if (!tempSpace.fscan(fp)){
        showerrorbox("Bad file!");
        fclose(fp);
    }
    else {
        space = tempSpace;
        space.setLinkedFileName(openpath);
    }
    fclose(fp);
}

void Menu::drawMenu() {
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(0, 0, x1, y1);
    bar(0, 0, x2, y2);
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
    m_space = Space3D(-2500, m_theme, this);
    m_space.setCorners(0, 27, m_appWidth, m_appHeight);
}

void Menu::draw() {
    m_space.run();
    drawMenu();
    swapbuffers();
}

bool Menu::getCommand(const int& x, const int& y) {
    if (x == -1) {
        return m_space.getKeyCommand();
    }
    if (m_fileButton.isListVisible()) {
        int pos = m_fileButton.listHitCollision(x, y);
        if (pos >= 0) {
            if (pos == 0) {
                initSpace();
            }
            else if (pos == 1 || pos == 2) {
                saveSpace3D(m_space, pos);
            }
            else if (pos == 3) {
                openSpace3D(m_space);
            }
            m_fileButton.setVisibility(false);
            return true;
        }
    }
    if (m_fileButton.hitCollision(x, y)) {
        m_fileButton.setVisibility(!m_fileButton.isListVisible());
        return 1;
    }
    if (m_settingsButton.hitCollision(x, y)) {
        closegraph();
        m_settingsMenuFlag = true;
        return false;
    }
    if (m_helpButton.hitCollision(x, y)) {
        Mesh aux = m_space.meshAt(0);
        aux.resetRotation();
        ObjectCreator objCreator(aux, m_theme);
        Mesh newMesh = objCreator.run();
    }
    if (m_space.getCommand(x, y)) {
        return true;
    }
    return false;
}

bool Menu::returnToSettingsFlag() {
    if (m_settingsMenuFlag) {
        m_settingsMenuFlag = false;
        return true;
    }
    return false;
}
