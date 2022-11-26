#ifndef SHAPEDATA_H
#define SHAPEDATA_H

#include <MyVector.h>

class Point3D {
    public:
        Point3D();
        Point3D(const int& x_, const int& y_, const int& z_);

    private:
        int x;
        int y;
        int z;
};

class Coord3D {
    public:
        Coord3D();
        Coord3D(const int& a_, const int& b_);

    private:
        int a;
        int b;
};

class Line3D {
    public:
        Line3D();
        Line3D(const Point3D& P_, const Point3D& Q_);

    private:
        Point3D P;
        Point3D Q;
};

class Point2D {
    public:
        Point2D();
        Point2D(const int& x_, const int& y_);

    private:
        int x;
        int y;
};

class Line2D {
    public:
        Line2D();
        Line2D(const Point2D& P_, const Point2D& Q_);

    private:
        Point2D P;
        Point2D Q;
};

class Section {
    public:
        Section();
        void addLine(const Line2D& line);

    private:
        MyVector<Line2D> m_lines;
};

#endif // SHAPEDATA_H
