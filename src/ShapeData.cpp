#include "ShapeData.h"

Point3D::Point3D() :
    x(), y(), z() {}

Point3D::Point3D(const int& x_, const int& y_, const int& z_) :
    x(x_), y(y_), z(z_){}

Point3D::Point3D(const Point3D& other) :
    x(other.x), y(other.y), z(other.z) {}

int Point3D::getX() const {
    return x;
}

int Point3D::getY() const {
    return y;
}

int Point3D::getZ() const {
    return z;
}

void Point3D::setX(const int& x_) {
    x = x_;
}

void Point3D::setY(const int& y_) {
    y = y_;
}

void Point3D::setZ(const int& z_) {
    z = z_;
}

void Point3D::setPoint(const Point3D& pct) {
    x = pct.getX();
    y = pct.getY();
    z = pct.getZ();
}

void Point3D::rotateOX(const double& alpha) {
    double beta = alpha * 3.14 / 180;
    int y_ = y * cos(beta) - z * sin(beta);
    int z_ = y * sin(beta) + z * cos(beta);
    y = y_;
    z = z_;
}

void Point3D::rotateOY(const double& alpha) {
    double beta = alpha * 3.14 / 180;
    int x_ = x * cos(beta) - z * sin(beta);
    int z_ = x * sin(beta) + z * cos(beta);
    x = x_;
    z = z_;
}

void Point3D::rotateOZ(const double& alpha) {
    double beta = alpha * 3.14 / 180;
    int x_ = x * cos(beta) - y * sin(beta);
    int y_ = x * sin(beta) + y * cos(beta);
    x = x_;
    y = y_;
}

Point3D& Point3D::operator += (const Point3D& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

void Point3D::translate(const int& xTranslate, const int& yTranslate, const int& zTranslate) {
    x += xTranslate;
    y += yTranslate;
    z += zTranslate;
}

Point2D::Point2D() :
    x(), y() {}

Point2D::Point2D(const int& x_, const int& y_) :
    x(x_), y(y_) {}

Point2D::Point2D(const Point2D& other) :
    x(other.x), y(other.y) {}

int Point2D::getX() const {
    return x;
}

int Point2D::getY() const{
    return y;
}

Line2D::Line2D() :
    P(), Q() {}

Line2D::Line2D(const Point2D& P_, const Point2D& Q_) :
    P(P_), Q(Q_){}

Point2D Line2D::getP() {
    return P;
}

Point2D Line2D::getQ() {
    return Q;
}

Section::Section() :
    m_lines(), m_centerPoint(), m_active(false) {}

Section::Section(const MyVector<Line2D>& lines) :
    m_lines(lines), m_centerPoint(), m_active(false) {}

size_t Section::size() const {
    return m_lines.size();
}

void Section::addLine(const Line2D& line) {
    m_lines.push_back(line);
}

Section& Section::operator = (const Section& other) {
    m_lines = other.m_lines;
    m_centerPoint = other.m_centerPoint;
    m_active = other.m_active;
    return *this;
}

Line2D& Section::operator [] (const size_t& index) {
    return m_lines[index];
}

Line3D::Line3D() :
    P(), Q() {}

Line3D::Line3D(const Point3D& P_, const Point3D& Q_) :
    P(P_), Q(Q_) {}

Line3D::Line3D(const Line3D& other) :
    P(other.P), Q(other.Q) {}

Line3D& Line3D::operator = (const Line3D& other) {
    P = other.P;
    Q = other.Q;
    return *this;
}

void Line3D::setP(const Point3D& P_) {
    P.setPoint(P_);
}

void Line3D::setQ(const Point3D& Q_) {
    Q.setPoint(Q_);
}

void Line3D::translate(const int& xTranslate, const int& yTranslate, const int& zTranslate) {
    P.translate(xTranslate, yTranslate, zTranslate);
    Q.translate(xTranslate, yTranslate, zTranslate);
}

Point3D Line3D::getP(){
    return P;
}

Point3D Line3D::getQ(){
    return Q;
}

Mesh::Mesh() :
    m_edges(), m_centerPoint(), m_active(false) {}

Mesh::Mesh(const MyVector<Line3D>& edges) :
    m_edges(edges), m_centerPoint(0, 0, 0), m_active(false) {
    for (size_t i = 0; i < size(); ++i) {
        m_centerPoint += m_edges[i].getP();
        m_centerPoint += m_edges[i].getQ();
    }
    m_centerPoint.setX(m_centerPoint.getX() / (2 * size()));
    m_centerPoint.setY(m_centerPoint.getY() / (2 * size()));
    m_centerPoint.setZ(m_centerPoint.getZ() / (2 * size()));
}

Mesh::Mesh(const Mesh& other) :
    m_edges(other.m_edges), m_centerPoint(other.m_centerPoint), m_active(false) {}

size_t Mesh::size() const {
    return m_edges.size();
}

Line3D& Mesh::operator [] (const size_t& index) {
    return m_edges[index];
}

Mesh& Mesh::operator = (const Mesh& other) {
    m_edges = other.m_edges;
    return *this;
}

void Mesh::addEdge(const Line3D& edge) {
    m_edges.push_back(edge);
}

void Mesh::translate(const int& xTranslate, const int& yTranslate, const int& zTranslate) {
    for (size_t i = 0; i < size(); ++i) {
        m_edges[i].translate(xTranslate, yTranslate, zTranslate);
    }
}
