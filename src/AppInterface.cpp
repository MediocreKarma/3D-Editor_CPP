#include "AppInterface.h"
#include <iostream>


AppInterface::AppInterface(const int& appWidth, const int& appHeight, const int& theme, const int& languagePackage) :
    m_appWidth(appWidth), m_appHeight(appHeight), m_theme(theme), m_languagePackage(languagePackage) {}

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

#include <iostream>

void AppInterface::run(){
    initwindow(m_appWidth, m_appHeight, "Editor 3D");
    Space3D space(maxRadius(), m_theme);
    Mesh cube;
    cube.addEdge(Line3D(Point3D(-100,-100,100),Point3D(-100,-100,-100)));
    cube.addEdge(Line3D(Point3D(-100,-100,-100),Point3D(100,-100,-100)));
    cube.addEdge(Line3D(Point3D(100,-100,-100),Point3D(100,-100,100)));
    cube.addEdge(Line3D(Point3D(100,-100,100),Point3D(-100,-100,100)));

    cube.addEdge(Line3D(Point3D(-100,100,100),Point3D(-100,100,-100)));
    cube.addEdge(Line3D(Point3D(-100,100,-100),Point3D(100,100,-100)));
    cube.addEdge(Line3D(Point3D(100,100,-100),Point3D(100,100,100)));
    cube.addEdge(Line3D(Point3D(100,100,100),Point3D(-100,100,100)));

    cube.addEdge(Line3D(Point3D(-100,100,100),Point3D(-100,-100,100)));
    cube.addEdge(Line3D(Point3D(-100,100,-100),Point3D(-100,-100,-100)));
    cube.addEdge(Line3D(Point3D(100,100,-100),Point3D(100,-100,-100)));
    cube.addEdge(Line3D(Point3D(100,100,100),Point3D(100,-100,100)));
    cube.updateCenterPoint();
    space.addMesh(cube);
    cube.translate(300, 300, -300);
    //space.addMesh(cube);
    cube.translate(600, 600, 300);
    //space.addMesh(cube);
    cube.translate(-1200, -600, -300);
    //space.addMesh(cube);
    cube.translate(-600, 600, 300);
    //space.addMesh(cube);

    //FILE* fp = fopen("save.txt", "r");
    //space.fscan(fp);
    space.run(0, 27, m_appWidth, m_appHeight);
    //fclose(fp);
    Menu menu(m_theme);
    menu.drawMenu(0, 0, m_appWidth, 27);
    bool looped = 0;
    while (true) {
        if (ismouseclick(WM_MOUSEMOVE)) {
            clearMouse();
            looped = 1;
        }
        else if (looped) {
            int xClick, yClick;
            getClick(xClick, yClick);
            if (xClick == -1) {
                continue;
            }
            if (menu.getCommand(xClick, yClick)) {
                space.run(0, 27, m_appWidth, m_appHeight);
                continue;
            }
            space.getCommand(xClick, yClick, 0, 27, m_appWidth, m_appHeight);
        }
    }
    /*sp.setAngleX(30);
    sp.setAngleY(60);
    sp.setAngleZ(40);
    */
}
