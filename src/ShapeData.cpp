#include "ShapeData.h"

Point3D::Point3D() = default;
Point3D::Point3D(const int& x_, const int& y_, const int& z_) :
    x(x_), y(y_), z(z_){}

const int& Point3D::getX() const{
    return x;
}

const int& Point3D::getY() const{
    return y;
}

const int& Point3D::getZ() const{
    return z;
}

Point2D::Point2D() = default;
Point2D::Point2D(const int& x_, const int& y_) :
    x(x_), y(y_){}

const int& Point2D::getX() const{
    return x;
}

const int& Point2D::getY() const{
    return y;
}

const Point3D& Line3D::getP(){
    return P;
}

const Point3D& Line3D::getQ(){
    return Q;
}

Line2D::Line2D() = default;

Line2D::Line2D(const Point2D& P_, const Point2D& Q_):
    P(P_), Q(Q_){}

const Point2D& Line2D::getP() {
    return P;
}

const Point2D& Line2D::getQ() {
    return Q;
}

Line3D::Line3D() = default;

Line3D::Line3D(const Point3D& P_, const Point3D& Q_):
    P(P_), Q(Q_){}

