#include "Space3D.h"
#include<graphics.h>
#include"ShapeData.h"
#include"MyArray.h"
#include"MenuInterface.h"
#include<iostream>

Space3D::Space3D() = default;

Space3D::Space3D(MyVector<Line3D> mesh) : m_mesh(mesh){}

/*
nimic de aici nu e bun
habar n am ce am facut sincer

Point2D Space3D::projectPunct(const Point3D& pct, const int& xMij, const int& yMij) const{
    float aa = (MenuInterface::razaMax - pct.getY()) / MenuInterface::razaMax;
    return Point2D(pct.getX() * aa + xMij, getmaxy() - (pct.getY() * aa + yMij));
}

MyVector<Line2D> Space3D::project3D(const int& xMij, const int& yMij){
    //lucram cu m_mesh.
    MyVector<Line2D> projection;
    std::cout<<"am intrat aici";
    for(size_t i = 0; i < m_mesh.size(); i++){
        Line2D linie(projectPunct(m_mesh[i].getP(), xMij, yMij), projectPunct(m_mesh[i].getQ(), xMij, yMij));
        std::cout<<"added line:"<<i<<"\n";
        projection.push_back(linie);
        std::cout<<projection[i].getP().getX()<<"\n";
    }
    return projection;
}

void Space3D::draw(const int& xMij, const int& yMij){
    MyVector<Line2D> projection = project3D(xMij, yMij);
    std::cout<<"defined projection\n";
    for(size_t i = 0; i < projection.size(); i++)
    {
        line(projection[i].getP().getX(), projection[i].getP().getY(), projection[i].getQ().getX(),projection[i].getQ().getY());
    }
}
*/
