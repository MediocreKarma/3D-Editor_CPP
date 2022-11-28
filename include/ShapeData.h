#ifndef SHAPEDATA_H
#define SHAPEDATA_H

#include<math.h>
#include <MyVector.h>

class Point3D {
    public:
        Point3D();
        Point3D(const int& x_, const int& y_, const int& z_);
        Point3D(const Point3D& pct);
        int getX() const;
        int getY() const;
        int getZ() const;
        void setX(const int& x_);
        void setY(const int& y_);
        void setZ(const int& z_);
        void setPoint(const Point3D& pct);
        void rotateOX(const double& alpha);
        void rotateOY(const double& alpha);
        void rotateOZ(const double& alpha);
        void translate(const int& xTranslate, const int& yTranslate, const int& zTranslate);
        Point3D& operator += (const Point3D& other);

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
        Line3D(const Line3D& other);
        Line3D& operator = (const Line3D& other);
        void translate(const int& xTranslate, const int& yTranslate, const int& zTranslate);
        Point3D getP();
        Point3D getQ();
        void setP(const Point3D& P_);
        void setQ(const Point3D& Q_);

    private:
        Point3D P;
        Point3D Q;
};

class Point2D {
    public:
        Point2D();
        Point2D(const int& x_, const int& y_);
        Point2D(const Point2D& pct);
        int getX() const;
        int getY() const;
        void setX(const int& x_);
        void setY(const int& y_);

    private:
        int x;
        int y;
};

class Line2D {
    public:
        Line2D();
        Line2D(const Point2D& P_, const Point2D& Q_);
        Point2D getP();
        Point2D getQ();

    private:
        Point2D P;
        Point2D Q;
};

class Section {
    public:
        Section();
        Section(const MyVector<Line2D>& lines);
        size_t size() const;
        void addLine(const Line2D& line);
        Section& operator = (const Section& other);
        Line2D& operator [] (const size_t& index);

    private:
        MyVector<Line2D> m_lines;
        Point2D m_centerPoint;
        bool m_active;
};

class Mesh {
    public:
        Mesh();
        Mesh(const MyVector<Line3D>& edges);
        Mesh(const Mesh& other);

        size_t size() const;
        void addEdge(const Line3D& edge);
        Line3D& operator [] (const size_t& index);
        Mesh& operator = (const Mesh& other);
        void translate(const int& xTranslate, const int& yTranslate, const int& zTranslate);

    private:
        MyVector<Line3D> m_edges;
        Point3D m_centerPoint;
        bool m_active;
};

#endif // SHAPEDATA_H
