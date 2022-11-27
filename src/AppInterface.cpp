#include "AppInterface.h"

//deocamdata e ceva mega basic, doar deseneaza full screen un cub, ca sa avem macar ceva de prezentat

AppInterface::AppInterface(const int& appWidth, const int& appHeight, const int& theme, const int& languagePackage) :
    m_appWidth(appWidth), m_appHeight(appHeight), m_theme(theme), m_languagePackage(languagePackage) {}

double AppInterface::maxRadius() const {
    return m_maxRadius;
}

void AppInterface::project3D(Space3D& Space, const int& x1, const int& y1, const int& x2, const int& y2) {
    Space.draw3D((x1 + x2) / 2, (y1 + y2) / 2, x2 - x1, y2 - y1);
}

void AppInterface::run(){
    initwindow(m_appWidth, m_appHeight, "Vizualizator 3D");
    Mesh mesh;
    MyVector<Mesh> meshes;

    mesh.addEdge(Line3D(Point3D(-100,-100,100),Point3D(-100,-100,-100)));

    mesh.addEdge(Line3D(Point3D(-100,-100,-100),Point3D(100,-100,-100)));
    mesh.addEdge(Line3D(Point3D(100,-100,-100),Point3D(100,-100,100)));
    mesh.addEdge(Line3D(Point3D(100,-100,100),Point3D(-100,-100,100)));

    mesh.addEdge(Line3D(Point3D(-100,100,100),Point3D(-100,100,-100)));
    mesh.addEdge(Line3D(Point3D(-100,100,-100),Point3D(100,100,-100)));
    mesh.addEdge(Line3D(Point3D(100,100,-100),Point3D(100,100,100)));
    mesh.addEdge(Line3D(Point3D(100,100,100),Point3D(-100,100,100)));

    mesh.addEdge(Line3D(Point3D(-100,100,100),Point3D(-100,-100,100)));
    mesh.addEdge(Line3D(Point3D(-100,100,-100),Point3D(-100,-100,-100)));
    mesh.addEdge(Line3D(Point3D(100,100,-100),Point3D(100,-100,-100)));
    mesh.addEdge(Line3D(Point3D(100,100,100),Point3D(100,-100,100)));
    meshes.push_back(mesh);
    Space3D sp(meshes, maxRadius());
    /*
    sp.setAngleX(30);
    sp.setAngleY(60);
    sp.setAngleZ(40);
    */
    project3D(sp, 0, 0, 600, 600);
}
