#include "ShapeData.h"
#include<math.h>
Point3D::Point3D() = default;
Point3D::Point3D(const int& x_, const int& y_, const int& z_) :
    x(x_), y(y_), z(z_){}

Point3D::Point3D(const Point3D& pct):
    x(pct.getX()), y(pct.getY()), z(pct.getZ()){}

const int& Point3D::getX() const{
    return x;
}

const int& Point3D::getY() const{
    return y;
}

const int& Point3D::getZ() const{
    return z;
}

void Point3D::setX(const int& x_){
    x = x_;
}

void Point3D::setY(const int& y_){
    y = y_;
}

void Point3D::setZ(const int& z_){
    z = z_;
}

void Point3D::setPoint(const Point3D& pct){
    x = pct.getX();
    y = pct.getY();
    z = pct.getZ();
}

void Point3D::rotateOX(double& alpha){
    double beta = alpha * 3.14 / 180;
    int y_ = y * cos(beta) - z * sin(beta);
    int z_ = y * sin(beta) + z * cos(beta);
    y = y_;
    z = z_;
}

void Point3D::rotateOY(double& alpha){
    double beta = alpha * 3.14 / 180;
    int x_ = x * cos(beta) - z * sin(beta);
    int z_ = x * sin(beta) + z * cos(beta);
    x = x_;
    z = z_;
}

void Point3D::rotateOZ(double& alpha){
    double beta = alpha * 3.14 / 180;
    int x_ = x * cos(beta) - y * sin(beta);
    int y_ = x * sin(beta) + y * cos(beta);
    x = x_;
    y = y_;
}

Point2D::Point2D() = default;

Point2D::Point2D(const int& x_, const int& y_) :
    x(x_), y(y_){}

Point2D::Point2D(const Point2D& pct) :
    x(pct.getX()), y(pct.getY()){}

const int& Point2D::getX() const{
    return x;
}

const int& Point2D::getY() const{
    return y;
}

const Point2D& Line2D::getP() {
    return P;
}

const Point2D& Line2D::getQ() {
    return Q;
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

Line3D::Line3D() = default;

Line3D::Line3D(const Point3D& P_, const Point3D& Q_):
    P(P_), Q(Q_){}

void Line3D::setP(const Point3D& P_){
    P.setPoint(P_);
}

void Line3D::setQ(const Point3D& Q_){
    Q.setPoint(Q_);
}
