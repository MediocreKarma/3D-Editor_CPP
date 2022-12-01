#include "AppInterface.h"
#include <iostream>

AppInterface::AppInterface(const int& appWidth, const int& appHeight, const int& theme, const int& languagePackage) :
    m_appWidth(appWidth), m_appHeight(appHeight), m_theme(theme), m_languagePackage(languagePackage) {}

double AppInterface::maxRadius() const {
    return m_maxRadius;
}

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
    /*cube.translate(300, 300, -300);
    space.addMesh(cube);
    cube.translate(600, 600, 300);
    space.addMesh(cube);
    cube.translate(-1200, -600, -300);
    space.addMesh(cube);
    cube.translate(-600, 600, 300);
    space.addMesh(cube);*/
    space.run(0, 0, m_appWidth, m_appHeight);
    int x, y;
    while (true) {
        if (ismouseclick(WM_MOUSEMOVE)) {
            getmouseclick(WM_MOUSEMOVE, x, y);
        }
        else if (space.insideWorkArea(x, y, 0, 0, m_appWidth, m_appHeight)) {
            space.getCommand(0, 0, m_appWidth, m_appHeight);
        }
    }
}
