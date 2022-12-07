#include "AppInterface.h"
#include <iostream>

AppInterface::AppInterface(const int& appWidth, const int& appHeight, const int& theme, const int& languagePackage) :
    m_appWidth(appWidth), m_appHeight(appHeight), m_theme(theme), m_languagePackage(languagePackage), m_fileGetter() {}

double AppInterface::maxRadius() const {
    return m_maxRadius;
}

void getClick(int& xClick, int& yClick) {
    getmouseclick(WM_LBUTTONDOWN, xClick, yClick);
}

void AppInterface::clearMouse() {
    int x, y;
    getmouseclick(WM_MOUSEMOVE, x, y);
    //getmouseclick(WM_LBUTTONUP, x, y);
    //getmouseclick(WM_MOUSEMOVE, x, y);
}

void AppInterface::saveSpace3D(Space3D& space, const char& saveType) {
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
        MyArray<char, 32> errorMessage = "Bad file path!";
        setbkcolor(LIGHTRED);
        setcolor(BLACK);
        outtextxy(m_appWidth / 2 - textwidth(errorMessage.data()) / 2, m_appHeight / 2 - textheight(errorMessage.data()) / 2, errorMessage.data());
        Sleep(2000);
    }
    else {
        space.fprint(fp);
        space.setLinkedFileName(savepath);
    }

    fclose(fp);
}

void AppInterface::openSpace3D(Space3D& space) {
    int getCurrentWindowNumber = getcurrentwindow();
    MyArray<char, 512> openpath = m_fileGetter.userOpenPath();
    setcurrentwindow(getCurrentWindowNumber);
    if (!openpath[0]) {
        return;
    }
    FILE* fp = fopen(openpath.data(), "r");
    if (!fp) {
        MyArray<char, 32> errorMessage = "File not found!";
        setbkcolor(LIGHTRED);
        setcolor(BLACK);
        outtextxy(m_appWidth / 2 - textwidth(errorMessage.data()) / 2, m_appHeight / 2 - textheight(errorMessage.data()) / 2, errorMessage.data());
        Sleep(2000);
    }
    else if (!space.fscan(fp)){
        MyArray<char, 32> errorMessage = "Bad file!";
        setbkcolor(LIGHTRED);
        setcolor(BLACK);
        outtextxy(m_appWidth / 2 - textwidth(errorMessage.data()) / 2, m_appHeight / 2 - textheight(errorMessage.data()) / 2, errorMessage.data());
        Sleep(2000);
        fclose(fp);
    }
    else {
        space.setLinkedFileName(openpath);
    }
    fclose(fp);
}

void AppInterface::run() {
    initwindow(m_appWidth, m_appHeight, "Editor 3D");
    Space3D space(maxRadius(), m_theme);
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
    space.addMesh(cube);
    cube.translate(0, 600, 0);
    space.addMesh(cube);
    cube.translate(0, 600, 0);
    space.addMesh(cube);
    cube.translate(0, 600, 0);
    space.addMesh(cube);
    cube.translate(0, 600, 0);
    space.addMesh(cube);
    space.setCorners(0, 27, m_appWidth, m_appHeight);
    space.run();
    Menu menu(m_theme);
    menu.drawMenu(0, 0, m_appWidth, 27);
    while (true) {
        if (ismouseclick(WM_MOUSEMOVE)) {
            clearMouse();
        }
        else {
            int xClick, yClick;
            getClick(xClick, yClick);
            if (menu.getCommand(xClick, yClick)) {
                char saveType = menu.saveSpace();
                if (menu.newSpace()) {
                    space = Space3D(maxRadius(), m_theme);
                }
                else if (saveType) {
                    saveSpace3D(space, saveType);
                }
                else if (menu.openSpace()) {
                    openSpace3D(space);
                }
                space.run();
                continue;
            }
            if (space.getCommand(xClick, yClick)) {
                menu.drawMenu(0, 0, m_appWidth, 27);
            }
        }
    }
}
