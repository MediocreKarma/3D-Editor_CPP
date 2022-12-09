#ifndef SHAPEDATA_H
#define SHAPEDATA_H

#include <math.h>
#include "MyVector.h"
#include <graphics.h>
#include <winbgim.h>
#include "AppTools.h"
#include <cstdio>

class Point2D {
    public:
        Point2D();
        Point2D(const int& x_, const int& y_);
        Point2D(const Point2D& pct);
        int getX() const;
        int getY() const;
        void setX(const int& x_);
        void setY(const int& y_);
        bool operator == (const Point2D& other);

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
        void draw();

    private:
        Point2D P;
        Point2D Q;
};

class Section {
    public:
        static const int RADIUS = 8;

        Section();
        Section(const MyVector<Point2D>& points, const Point2D& centerPoint, const MyVector<MyVector<size_t>>& adjList);
        Section(const Section& other);
        size_t size() const;
        Point2D centerPoint() const;
        void addPoint(const Point2D& line);
        void addPoint(const int& x, const int& y);
        void addEdge(const size_t& index1, const size_t& index2);
        Section& operator = (const Section& other);
        Point2D& operator [] (const size_t& index);
        const Point2D& operator [] (const size_t& index) const;
        void draw(const int& theme, const int& fillColor, const int& borderColor = BLACK);
        void drawButton(const int& fillColor, const int& borderColor = BLACK);
        bool grabButtonCollision(const int& x, const int& y) const;

    private:
        MyVector<Point2D> m_points;
        MyVector<MyVector<size_t>> m_adjList;
        Point2D m_centerPoint;
        CircularButton m_grabPoint;
        bool m_active;
};

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
        void rotateOX(const Point3D& center, const double& alpha);
        void rotateOY(const Point3D& center, const double& alpha);
        void rotateOZ(const Point3D& center, const double& alpha);
        void translate(const int& xTranslate, const int& yTranslate, const int& zTranslate);
        Point3D& operator += (const Point3D& other);
        void fprint(FILE* fp);
        bool fscan(FILE* fp);

    private:
        int x;
        int y;
        int z;
};

class Line3D {
    public:
        Line3D();
        Line3D(const Point3D& P_, const Point3D& Q_);
        Line3D(const Line3D& other);
        Line3D& operator = (const Line3D& other);
        void translate(const int& xTranslate, const int& yTranslate, const int& zTranslate);
        Point3D getP() const;
        Point3D getQ() const;
        double getLength() const;
        void setP(const Point3D& P_);
        void setQ(const Point3D& Q_);
        void fprint(FILE* fp);
        bool fscan(FILE* fp);
        void rotateOX(const Point3D& center, const double& angle);
        void rotateOY(const Point3D& center, const double& angle);
        void rotateOZ(const Point3D& center, const double& angle);

    private:
        Point3D P;
        Point3D Q;
};

class Mesh {
    public:
        Mesh();
        Mesh(const MyVector<Point3D>& points, const MyVector<MyVector<size_t>>& adjList);
        Mesh(const Mesh& other);

        size_t size() const;
        void addPoint(const Point3D& point);
        void addPoint(const int& x, const int& y, const int& z);
        void addEdge(const size_t& index1, const size_t& index2);
        Point3D& operator [] (const size_t& index);
        const Point3D& operator [] (const size_t& index) const;
        Mesh& operator = (const Mesh& other);
        MyVector<size_t> adjListAt(const size_t& index) const;
        const MyVector<MyVector<size_t>>& adjacencyList() const;
        void addIndexConnections(const size_t& index, const MyVector<size_t>& adjList);
        void translate(const int& xTranslate, const int& yTranslate, const int& zTranslate);
        Point3D centerPoint() const;
        void updateCenterPoint();
        void fprint(FILE* fp);
        bool fscan(FILE* fp);
        void rotate(const double& angleX, const double& angleY, const double& angleZ);
        void scaleEven(const double& scaleFactor);
        void scaleAxis(bool isLocal, const double& scaleFactor, const size_t& axis);
        void mirror(bool isLocal, const size_t& axis);
        double angleX() const;
        double angleY() const;
        double angleZ() const;

    private:
        MyVector<Point3D> m_points;
        MyVector<MyVector<size_t>> m_adjList;
        Point3D m_centerPoint;
        double m_angleX, m_angleY, m_angleZ;
};

#endif // SHAPEDATA_H
