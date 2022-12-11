#include "ObjectCreator.h"

Level::Level() :
    m_points(), m_height() {}

Level::Level(const MyVector<Point2D>& points, const int& height_) :
    m_points(points), m_height(height_) {}

Point2D& Level::operator [] (const size_t& index) {
    return m_points[index];
}

int Level::height() const {
    return m_height;
}

size_t Level::size() const {
    return m_points.size();
}

void Level::changeHeight(const int& height_) {
    m_height = height_;
}

void Level::addPoint(const Point2D& point) {
    m_points.push_back(point);
}

void Level::removePoint(const size_t& index) {
    for (size_t i = index + 1; i < size(); ++i) {
        m_points[i - 1] = m_points[i];
    }
    m_points.pop_back();
}

ObjectCreator::ObjectCreator() :
    m_levels(), m_points(), m_adjList(), m_cam(Point3D(0, -500, 0), 0.0, 0.0, 0.0), x0(0), y0(0), x1(800), y1(800) {}

ObjectCreator::ObjectCreator(const Mesh& mesh) :
    m_levels(), m_points(), m_adjList(mesh.adjacencyList()), m_cam(Point3D(0, -500, 0), 0.0, 0.0, 0.0), x0(0), y0(0), x1(800), y1(800) {
    for (size_t i = 0; i < mesh.size(); ++i) {
        m_points.push_back(projectPoint(mesh[i]));
        bool levelExists = false;
        for (size_t j = 0; j < m_levels.size(); ++j) {
            if (m_levels[i].height() == mesh[i].getZ()) {
                levelExists = true;
                m_levels[i].addPoint(Point2D(mesh[i].getX(), mesh[i].getY()));
            }
        }
        if (!levelExists) {
            m_levels.push_back(Level());
            m_levels.back().changeHeight(mesh[i].getZ());
            m_levels.back().addPoint(Point2D(mesh[i].getX(), mesh[i].getY()));
        }
    }
}

Point2D ObjectCreator::projectPoint(const Point3D& pct) const {
    const int xCenter = (x0 + x1) / 2;
    const int yCenter = (y0 + y1) / 2;
    const int yLen = (y1 - y0);
    const int xr = pct.getX() - m_cam.position().getX();
    const int yr = pct.getY() - m_cam.position().getY();
    const int zr = pct.getZ() - m_cam.position().getZ();
    double aX = m_cam.angleX();
    double aY = m_cam.angleY();
    double aZ = m_cam.angleZ();
    double EZ = m_cam.EZ();
    int dx = cos(aY) * (sin(aZ) * yr + cos(aZ) * xr) - sin(aY) * zr;
    int dy = sin(aX) * (cos(aY) * zr + sin(aY) * (sin(aZ) * yr + cos(aZ) * xr)) + cos(aX) * (cos(aZ) * yr - sin(aZ) * xr);
    int dz = cos(aX) * (cos(aY) * zr + sin(aY) * (sin(aZ) * yr + cos(aZ) * xr)) - sin(aX) * (cos(aZ) * yr - sin(aZ) * xr);
    if (dy <= 0) {
        return Point2D(-100, -100);
    }
    int xprim = EZ * dx * yLen / dy + xCenter;
    int yprim = EZ * dz * yLen / dy * -1 + yCenter;
    return Point2D(xprim, yprim);
}

Mesh ObjectCreator::run() {


    return Mesh();
}
