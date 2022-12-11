#ifndef OBJECTCREATOR_H
#define OBJECTCREATOR_H

#include "ShapeData.h"
#include "MyVector.h"
#include "Camera.h"

class Level {
    public:
        Level();
        Level(const MyVector<Point2D>& points, const int& height);
        Point2D& operator [] (const size_t& index);
        int height() const;
        size_t size() const;
        void changeHeight(const int& height);
        void addPoint(const Point2D& point);
        void removePoint(const Point2D& point);
        void removePoint(const size_t& index);

    private:
        MyVector<Point2D> m_points;
        int m_height;
};

class ObjectCreator {
    public:
        ObjectCreator();
        ObjectCreator(const Mesh& editedMesh);
        Mesh run();


    private:
        MyVector<Level> m_levels;
        MyVector<Point2D> m_points;
        MyVector<MyVector<size_t>> m_adjList;
        Camera m_cam;
        int x0;
        int y0;
        int x1;
        int y1;

        Point2D projectPoint(const Point3D& pct) const;
};

#endif // OBJECTCREATOR_H
