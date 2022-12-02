#ifndef SHAPEDATA_H
#define SHAPEDATA_H

#include<math.h>
#include "MyVector.h"
#include <graphics.h>
#include <winbgim.h>
#include "AppTools.h"

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
        void draw();

    private:
        Point2D P;
        Point2D Q;
};

class Section {
    public:
        static constexpr int RADIUS = 8;

        Section();
        Section(const MyVector<Line2D>& lines, const Point2D& centerPoint);
        Section(const Section& other);
        size_t size() const;
        Point2D centerPoint() const;
        void addLine(const Line2D& line);
        Section& operator = (const Section& other);
        Line2D& operator [] (const size_t& index);
        void draw(const int& theme, const int& fillColor, const int& borderColor = BLACK);
        void drawButton(const int& fillColor, const int& borderColor = BLACK);
        bool grabButtonCollision(const int& x, const int& y) const;

    private:
        MyVector<Line2D> m_lines;
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
        Point3D getP();
        Point3D getQ();
        float getLength() const;
        void setP(const Point3D& P_);
        void setQ(const Point3D& Q_);

    private:
        Point3D P;
        Point3D Q;
};

class Mesh {
    public:
        Mesh();
        Mesh(const MyVector<Line3D>& edges);
        Mesh(const Mesh& other);

        size_t size() const;
        MyVector<Line3D> getEdges() const;
        void addEdge(const Line3D& edge);
        Line3D& operator [] (const size_t& index);
        Mesh& operator = (const Mesh& other);
        void translate(const int& xTranslate, const int& yTranslate, const int& zTranslate);
        Point3D centerPoint() const;
        void updateCenterPoint();

    private:
        MyVector<Line3D> m_edges;
        Point3D m_centerPoint;
};

class Camera {
    public:
        Camera(const int& maxRadius);
        Camera(const Point3D& point, const int& angleX, const int& angleY, const int& angleZ);

        Point3D getPoint() const;
        float getAngleX() const;
        float getAngleY() const;
        float getAngleZ() const;
        float getEZ() const;

    private:
        Point3D m_point;
        float m_angleX, m_angleY, m_angleZ;
        float m_EZ;
};
#endif // SHAPEDATA_H
