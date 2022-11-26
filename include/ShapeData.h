#ifndef SHAPEDATA_H
#define SHAPEDATA_H

#include <MyVector.h>

class Point3D {
    public:
        Point3D();
        Point3D(const int& x_, const int& y_, const int& z_);
        const int& getX() const;
        const int& getY() const;
        const int& getZ() const;

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
        const Point3D& getP();
        const Point3D& getQ();

    private:
        Point3D P;
        Point3D Q;
};

class Point2D {
    public:
        Point2D();
        Point2D(const int& x_, const int& y_);
        const int& getX() const;
        const int& getY() const;

    private:
        int x;
        int y;
};

class Line2D {
    public:
        Line2D();
        Line2D(const Point2D& P_, const Point2D& Q_);
        const Point2D& getP();
        const Point2D& getQ();

    private:
        Point2D P;
        Point2D Q;
};

class Section {
    public:
        Section();
        Section(const int& Z);
        void addLine(const Line2D& line);

    private:
        MyVector<Line2D> m_lines;
};

class Mesh {
    public:

    private:
        MyVector<Line3D> m_edges;
};

#endif // SHAPEDATA_H
