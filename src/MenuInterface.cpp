#include "MenuInterface.h"
#include<graphics.h>
#include"ShapeData.h"
#include"Space3D.h"
#include<iostream>
#include"../MyVector.h"

//deocamdata e ceva mega basic, doar deseneaza full screen un cub, ca sa avem macar ceva de prezentat
void MenuInterface::project3D(Space3D& Space, const int& x1, const int& y1, const int& x2, const int& y2){

    Space.draw3D((x1+x2)/2, (y1+y2) / 2, x2-x1,y2-y1);
}

void MenuInterface::run(const int& resX, const int& resY, const int& theme, const int& language){
    initwindow(resX,resY,"Vizualizator 3D");
    MyVector<Line3D> mesh;

    mesh.push_back(Line3D(Point3D(-100,-100,100),Point3D(-100,-100,-100)));
    mesh.push_back(Line3D(Point3D(-100,-100,-100),Point3D(100,-100,-100)));
    mesh.push_back(Line3D(Point3D(100,-100,-100),Point3D(100,-100,100)));
    mesh.push_back(Line3D(Point3D(100,-100,100),Point3D(-100,-100,100)));

    mesh.push_back(Line3D(Point3D(-100,100,100),Point3D(-100,100,-100)));
    mesh.push_back(Line3D(Point3D(-100,100,-100),Point3D(100,100,-100)));
    mesh.push_back(Line3D(Point3D(100,100,-100),Point3D(100,100,100)));
    mesh.push_back(Line3D(Point3D(100,100,100),Point3D(-100,100,100)));



    mesh.push_back(Line3D(Point3D(-100,100,100),Point3D(-100,-100,100)));
    mesh.push_back(Line3D(Point3D(-100,100,-100),Point3D(-100,-100,-100)));
    mesh.push_back(Line3D(Point3D(100,100,-100),Point3D(100,-100,-100)));
    mesh.push_back(Line3D(Point3D(100,100,100),Point3D(100,-100,100)));



    Space3D sp = Space3D(mesh);
    /*sp.setAngleX(30);
    sp.setAngleY(60);
    sp.setAngleZ(40);*/


    project3D(sp,000,000,600,600);
}
