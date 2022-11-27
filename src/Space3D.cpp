#include "Space3D.h"
#include<graphics.h>
#include"ShapeData.h"
#include"MyArray.h"
#include"MenuInterface.h"
#include<iostream>
#include<math.h>

Space3D::Space3D() = default;

Space3D::Space3D(MyVector<Line3D>& mesh){
    m_mesh = mesh;
}

void Space3D::rotateOX(MyVector<Line3D>& lines, double alpha){
    for(size_t i = 0; i < lines.size(); i++){
        Point3D aux1 = Point3D(lines[i].getP());
        Point3D aux2 = Point3D(lines[i].getQ());
        aux1.rotateOX(alpha);
        aux2.rotateOX(alpha);
        lines[i].setP(aux1);
        lines[i].setQ(aux2);
    }
}

void Space3D::rotateOY(MyVector<Line3D>& lines, double alpha){
    for(size_t i = 0; i < lines.size(); i++){
        Point3D aux1 = Point3D(lines[i].getP());
        Point3D aux2 = Point3D(lines[i].getQ());
        aux1.rotateOY(alpha);
        aux2.rotateOY(alpha);
        lines[i].setP(aux1);
        lines[i].setQ(aux2);
    }
}

void Space3D::rotateOZ(MyVector<Line3D>& lines, double alpha){
    for(size_t i = 0; i < lines.size(); i++){
        Point3D aux1 = Point3D(lines[i].getP());
        Point3D aux2 = Point3D(lines[i].getQ());
        aux1.rotateOZ(alpha);
        aux2.rotateOZ(alpha);
        lines[i].setP(aux1);
        lines[i].setQ(aux2);
    }
}

/*
tot ce am scris aici este doar ca sa vad daca merge alg de proiectie
nimic nu e permanent, cred
*/

Point2D Space3D::projectPunct(const Point3D& pct, const int& xMij, const int& yMij, const int& xLung, const int& yLung) const{
    double xr = pct.getX();
    double zr = pct.getZ();
    double yr = pct.getY();
    double scale = (float(xLung) / getmaxx()) > float(yLung) / getmaxy() ? float(xLung) / getmaxx() : float(yLung) / getmaxy();
    double aa = (MenuInterface::razaMax - pct.getY()) / MenuInterface::razaMax;
    return Point2D((xr * aa * scale + double(xMij))  , (yMij + yLung/2 - (zr * aa  * scale + double(yMij))));
}

MyVector<Line2D> Space3D::project3D(const int& xMij, const int& yMij, const int& xLung, const int& yLung){
    rotateOY(m_mesh,30);
    rotateOX(m_mesh,60);
    MyVector<Line2D> projection;
    for(size_t i = 0; i < m_mesh.size(); i++){
        const Point3D P13D = Point3D(m_mesh[i].getP());
        const Point3D P23D = Point3D(m_mesh[i].getQ());
        const Line2D le = Line2D(projectPunct(P13D, xMij, yMij, xLung, yLung),projectPunct(P23D,xMij,yMij, xLung, yLung));
        projection.push_back(le);
    }
    return projection;
    rotateOY(m_mesh, -30);
    rotateOX(m_mesh, -60);
}

void Space3D::draw3D(const int& xMij, const int& yMij, const int& xLung, const int& yLung){
    MyVector<Line2D> projection = project3D(xMij, yMij, xLung, yLung);
    for(size_t i = 0; i < projection.size(); i++){
        line(projection[i].getP().getX(), projection[i].getP().getY(), projection[i].getQ().getX(),projection[i].getQ().getY());
    }
}

