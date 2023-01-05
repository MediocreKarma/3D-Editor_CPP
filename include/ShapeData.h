#ifndef SHAPEDATA_H
#define SHAPEDATA_H

#include <cmath>
#include <iostream>
#include "MyVector.h"
#include <graphics.h>
#include <winbgim.h>
#include "AppTools.h"
#include <cstdio>
#include "MySet.h"
#include "MyHashMap.h"
#include "MyHashSet.h"
#include "Quaternion.h"

struct Point2D {
    Point2D();
    Point2D(const int& x_, const int& y_);
    Point2D(const double& x_, const double& y_);
    Point2D(const Point2D& pct);
    int getX() const;
    int getY() const;
    void setX(const int& x_);
    void setY(const int& y_);
    bool operator == (const Point2D& other);
    Point2D& operator = (const Point2D& other);
    Point2D& operator += (const Point2D& other);
    Point2D& operator -= (const Point2D& other);
    Point2D operator + (const Point2D& p);
    Point2D operator - (const Point2D& p);
    void translate(const Point2D& other);
    void translate(const int& x_, const int& y_);
    friend bool linesIntersect(const Point2D& A, const Point2D& B, const Point2D& C, const Point2D& D);
    //iostream
    void display() const;
    void display(bool endlAfter) const;

    int x;
    int y;
};

class Line2D {
    public:
        Line2D();
        Line2D(const Point2D& P_, const Point2D& Q_);
        Line2D(const int& x0, const int& y0, const int& x1, const int& y1);
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

struct Point3D {
    Point3D();
    Point3D(const double& x_, const double& y_, const double& z_);
    Point3D(const Point3D& pct);
    Point3D(const MyArray<double, 3>& arr);
    double getX() const;
    double getY() const;
    double getZ() const;
    void setX(const double& x_);
    void setY(const double& y_);
    void setZ(const double& z_);
    void setPoint(const Point3D& pct);
    void round();
    MyArray<double, 3> toArray() const;
    void rotateOX(const Point3D& center, const double& alpha);
    void rotateOY(const Point3D& center, const double& alpha);
    void rotateOZ(const Point3D& center, const double& alpha);
    void rotateByAxisVector(const double& angle, const MyArray<double, 3>& axis);
    void rotateByUnitQuat(const Quaternion& quat);
    Point3D rotatedByAxisVector(const double& angle, const MyArray<double, 3>& axis);
    Point3D rotatedByUnitQuat(const Quaternion& quat);
    void translate(const double& xTranslate, const double& yTranslate, const double& zTranslate);
    Point3D& operator += (const Point3D& other);
    Point3D& operator -= (const Point3D& other);
    Point3D operator + (const Point3D& other) const;
    Point3D operator - (const Point3D& other) const;
    Point3D operator * (const double& scalar) const;
    bool operator == (const Point3D& other) const;
    void fprint(FILE* fp);
    bool fscan(FILE* fp);
    double norm() const;
    void normalize();
    //iostream
    void display() const;
    void display(bool endlAfter) const;

    friend bool operator < (const Point3D& x, const Point3D& y);
    friend Point3D cross(const Point3D& p1, const Point3D& p2);
    friend double dot(const Point3D& p1, const Point3D& p2);

    double x;
    double y;
    double z;
};

struct Ray {
    Point3D origin, direction;
    double t;
    Ray() : origin(0, 0, 0), direction(1, 0, 0), t(1) {}
    Ray(const Point3D& p1, const Point3D& p2) : origin(p1), direction(p2 - p1), t(1) {
        if (direction == Point3D(0, 0, 0)) {
            direction = Point3D(1, 0, 0);
        }
        direction.normalize();
    }
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
        double length() const;
        void setP(const Point3D& P_);
        void setQ(const Point3D& Q_);
        void fprint(FILE* fp);
        bool fscan(FILE* fp);

    private:
        Point3D P;
        Point3D Q;
};

class FixedMesh;

struct MeshTransforms {
    MyArray<MyArray<double, 3>, 3> t;
    MeshTransforms() : t() {}
    MyArray<double, 3>& operator[] (size_t index) {
        return t[index];
    }
    const MyArray<double, 3>& operator[] (size_t index) const {
        return t[index];
    }
};

class Mesh {
    public:
        Mesh();
        Mesh(const MyVector<Point3D>& points, const MyVector<MyVector<size_t>>& adjList);
        Mesh(const Mesh& other);
        Mesh& operator = (const FixedMesh& other);

        size_t size() const;
        void erase(const size_t& index);
        void addPoint(const Point3D& point);
        void addPoint(const double& x, const double& y, const double& z);
        void addEdge(const size_t& index1, const size_t& index2);
        Point3D& operator [] (const size_t& index);
        const Point3D& operator [] (const size_t& index) const;
        Mesh& operator = (const Mesh& other);
        MyVector<size_t> adjListAt(const size_t& index) const;
        MyVector<MyVector<size_t>>& adjacencyListReference();
        const MyVector<MyVector<size_t>>& adjacencyList() const;
        void addIndexConnections(const size_t& index, const MyVector<size_t>& adjList);
        void deleteIndexConnection(const size_t& index1, const size_t& index2);
        void translate(const double& xTranslate, const double& yTranslate, const double& zTranslate);
        void translate(const Point3D& pntTranslate);
        Point3D centerPoint() const;
        void updateCenterPoint();
        void fprint(FILE* fp);
        bool fscan(FILE* fp);
        void rotate(const double& angleX, const double& angleY, const double& angleZ);
        void rotateOnAxis(const Point3D& center, const Point3D& axis, const double& angle);
        void rotateByUnitQuat(const Quaternion& quat);
        void rotateDisplayAngle();
        void scaleEven(const double& scaleFactor);
        void scaleAxis(const double& scaleFactor, const size_t& axis);
        void mirror(const size_t& axis);
        double angleX() const;
        double angleY() const;
        double angleZ() const;
        Quaternion quat() const;
        void resetRotation();
        void resetScale();
        MyArray<Point3D, 2> getBoundingBoxCorners() const;
        double scaleX() const;
        double scaleY() const;
        double scaleZ() const;
        Point3D localAxis(int axis) const;
        MyArray<Point3D, 3> localAxes() const;
        MeshTransforms transforms() const;
        void resetTransforms();
        void applyTransforms(const MeshTransforms& transforms);
        void setTransform(size_t transform, size_t axis, const double& value);

    private:
        MyVector<Point3D> m_points;
        MyVector<MyVector<size_t>> m_adjList;
        Point3D m_centerPoint;
        double m_angleX, m_angleY, m_angleZ;
        Quaternion m_quat;
        double m_scaleX, m_scaleY, m_scaleZ;
};

struct IntegerPoint3D {
    int x, y, z;

    IntegerPoint3D();
    IntegerPoint3D(const Point3D& point);
    IntegerPoint3D(const int& x, const int& y, const int& z);
    IntegerPoint3D(const IntegerPoint3D& other);
    IntegerPoint3D& operator = (const Point3D& point3d);
    IntegerPoint3D& operator = (const IntegerPoint3D& iPoint3d);
    friend bool operator < (const IntegerPoint3D& x, const IntegerPoint3D& y);
};

namespace std {
    template<> struct hash<IntegerPoint3D> {
        size_t operator()(const IntegerPoint3D& P) const noexcept {
            size_t hash1 = hash<int>{}(P.x);
            size_t hash2 = hash<int>{}(P.y);
            size_t hash3 = hash<int>{}(P.z);
            return hash1 ^ (hash2 << 1) ^ (hash3 << 2);
        }
    };
}

class FixedMesh {
    private:
        struct ListContainer {
            IntegerPoint3D point;
            CircularButton button2d;

            ListContainer() :
                point(), button2d() {}
            ListContainer(const IntegerPoint3D& p, CircularButton button) :
                point(p), button2d(button) {}
        };

    public:
        using iterator_type = MyList<ListContainer>::iterator;

        FixedMesh();
        FixedMesh(const Mesh& other);
        void addEdge(const IntegerPoint3D& x, const IntegerPoint3D& y);
        void addEdge(iterator_type it1, iterator_type it2);
        void addPoint(const IntegerPoint3D& x);
        void addPoint(const int& x, const int& y, const int& z);
        void erasePoint(const IntegerPoint3D& x);
        void erasePoint(const int& x, const int& y, const int& z);
        void erasePoint(iterator_type it);
        void eraseConnection(const IntegerPoint3D& x, const IntegerPoint3D& y);
        void eraseConnection(iterator_type it1, iterator_type it2);
        void renderPoint(iterator_type pointIt, const Point2D& conversion);
        void drawMesh(const int primaryThemeColor, const int accentColor);
        void draw2DLinesFrom(const IntegerPoint3D& point, const char staticLayer);
        void draw2DLinesFrom(iterator_type it, const char staticLayer);
        void updatePointValue(iterator_type point, const IntegerPoint3D& newValue);
        bool cutLines(const Line2D cuttingLine);
        iterator_type begin();
        iterator_type end();
        iterator_type find(const IntegerPoint3D& x);
        iterator_type hitCollisionIterator(const int x, const int y);
        size_t size();
        size_t countConnections(const IntegerPoint3D& x);
        size_t countConnections(iterator_type it);
        const MyHashSet<iterator_type>& adjacentPoints(const IntegerPoint3D& x);
        const MyHashSet<iterator_type>& adjacentPoints(iterator_type);

    private:
        MyList<ListContainer> m_points;
        MyMap<IntegerPoint3D, iterator_type> m_pointIterators;
        MyHashMap<iterator_type, MyHashSet<iterator_type>> m_adjList;
};

#endif // SHAPEDATA_H
