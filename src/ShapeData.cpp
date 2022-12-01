#include "../include/ShapeData.h"

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

void Point3D::rotateOX(const Point3D& center, const double& alpha) {
    translate(center.getX() * (-1), center.getY() * (-1), center.getZ() * (-1));
    double beta = alpha * 3.14 / 180;
    int y_ = y * cos(beta) - z * sin(beta);
    int z_ = y * sin(beta) + z * cos(beta);
    y = y_;
    z = z_;
    translate(center.getX(), center.getY(), center.getZ());
}

void Point3D::rotateOY(const Point3D& center, const double& alpha) {
    translate(center.getX() * (-1), center.getY() * (-1), center.getZ() * (-1));
    double beta = alpha * 3.14 / 180;
    int x_ = x * cos(beta) - z * sin(beta);
    int z_ = x * sin(beta) + z * cos(beta);
    x = x_;
    z = z_;
    translate(center.getX(), center.getY(), center.getZ());
}

void Point3D::rotateOZ(const Point3D& center, const double& alpha) {
    translate(center.getX() * (-1), center.getY() * (-1), center.getZ() * (-1));
    double beta = alpha * 3.14 / 180;
    int x_ = x * cos(beta) - y * sin(beta);
    int y_ = x * sin(beta) + y * cos(beta);
    x = x_;
    y = y_;
    translate(center.getX(), center.getY(), center.getZ());
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

Point3D Line3D::getP(){
    return P;
}

Point3D Line3D::getQ(){
    return Q;
}

// DEPRECATED
/*Point2D Point3D::project(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const Camera& cam, const double& scale) const {
    double aa = (radius - y) / radius;
    return Point2D( x * scale * aa + xCenter, yLen / 2 - z * scale * aa);

    int xr, yr, zr; //temporare; a.x - cam.x
    Point3D P = cam.getPoint();
    xr = x - P.getX();
    yr = y - P.getY();
    zr = z - P.getZ();
    int dx, dy, dz; //rotire euler pct 3D.
    int aX, aY, aZ;
    aX = cam.getAngleX();
    aY = cam.getAngleY();
    aZ = cam.getAngleZ();
    dx = cos(aX) * (sin(aZ * yr) + cos(aZ) * xr ) - sin(cam.getAngleY()) * zr;
    dy = sin(aX) * (cos(aY) * zr + sin(aY) * (sin(aZ) * yr + cos(aZ) * xr)) + cos(aX) * (cos(aZ) * yr - sin(aZ) * xr);
    dz = cos(aX) * (cos(aY) * zr + sin(aY) * (sin(aZ) * yr + cos(aZ) * xr)) - sin(aX) * (cos(aZ) * yr - sin(aZ) * xr);
    int FOV = 1;
    int xprim = xLen/2 * dx / dy;
    int yprim = yLen/2 * dz / dy;
    return Point2D(xprim,yprim);

    //PENTRU e
    x' = (half width of viewport) * x / z
    y' = (half height of viewport) * y / z

}*/

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

void Line2D::draw() {
    line(P.getX(), P.getY(), Q.getX(), Q.getY());
}

Section::Section() :
    m_lines(), m_grabPoint(), m_active(false) {}

Section::Section(const MyVector<Line2D>& lines, const Point2D& centerPoint) :
    m_lines(lines), m_grabPoint(centerPoint.getX(), centerPoint.getY(), Section::RADIUS), m_active(false) {}

Section::Section(const Section& other) :
    m_lines(other.m_lines), m_grabPoint(other.m_grabPoint), m_active(other.m_active) {}

constexpr int Section::RADIUS;

size_t Section::size() const {
    return m_lines.size();
}

void Section::addLine(const Line2D& line) {
    m_lines.push_back(line);
}

void Section::draw(const int& primaryThemeColor, const int& fillColor, const int& borderColor) {
    setcolor(primaryThemeColor);
    for (size_t i = 0; i < size(); ++i) {
        m_lines[i].draw();
    }
    drawButton(fillColor, borderColor);
}

bool Section::grabButtonCollision(const int& x, const int& y) const {
    return m_grabPoint.hitCollision(x, y);
}

void Section::drawButton(const int& fillColor, const int& borderColor) {
    m_grabPoint.drawLabel(fillColor, borderColor);
}

Section& Section::operator = (const Section& other) {
    m_lines = other.m_lines;
    m_grabPoint = other.m_grabPoint;
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

/*int Line2D::getLength(){
    //pitagora in 2d
}

int Line3D::getLength() const{
    //pitagora in 3d.
}*/

Mesh::Mesh() :
    m_edges(), m_centerPoint() {}

Mesh::Mesh(const MyVector<Line3D>& edges) :
    m_edges(edges), m_centerPoint(0, 0, 0) {
    updateCenterPoint();
}

Mesh::Mesh(const Mesh& other) :
    m_edges(other.m_edges), m_centerPoint(other.m_centerPoint) {}

void Mesh::updateCenterPoint() {
    for (size_t i = 0; i < size(); ++i) {
        m_centerPoint += m_edges[i].getP();
        m_centerPoint += m_edges[i].getQ();
    }
    m_centerPoint.setX(m_centerPoint.getX() / (2 * size()));
    m_centerPoint.setY(m_centerPoint.getY() / (2 * size()));
    m_centerPoint.setZ(m_centerPoint.getZ() / (2 * size()));
}

size_t Mesh::size() const {
    return m_edges.size();
}

Line3D& Mesh::operator [] (const size_t& index) {
    return m_edges[index];
}

Mesh& Mesh::operator = (const Mesh& other) {
    m_edges = other.m_edges;
    m_centerPoint = other.m_centerPoint;
    return *this;
}

void Mesh::addEdge(const Line3D& edge) {
    m_edges.push_back(edge);
}

void Mesh::translate(const int& xTranslate, const int& yTranslate, const int& zTranslate) {
    for (size_t i = 0; i < size(); ++i) {
        m_edges[i].translate(xTranslate, yTranslate, zTranslate);
    }
    m_centerPoint.translate(xTranslate, yTranslate, zTranslate);
}

Point3D Mesh::centerPoint() const {
    return m_centerPoint;
}

MyVector<Line3D> Mesh::getEdges() const {
    return m_edges;
}
/*Section Mesh::project(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const double& radius, const double& scale) {
    MyVector<Line2D> lines;
    lines.reserve(size());
    for(size_t i = 0; i < size(); i++) {
        const Point2D P = m_edges[i].getP().project(xCenter, yCenter, xLen, yLen, radius, scale);
        const Point2D Q = m_edges[i].getQ().project(xCenter, yCenter, xLen, yLen, radius, scale);
        lines.push_back(Line2D(P, Q));
    }
    return Section(lines, m_centerPoint.project(xCenter, yCenter, xLen, yLen, radius, scale));
}*/

Camera::Camera(const int& maxRadius) :
    m_point(0, maxRadius, 0), m_angleX(0), m_angleY(0), m_angleZ(0), m_EZ(-1) {}

Camera::Camera(const Point3D& point, const int& alpha, const int& beta, const int& theta) :
    m_point(point), m_angleX(alpha), m_angleY(beta), m_angleZ(theta), m_EZ(-1) {}

Point3D Camera::getPoint() const {
    return m_point;
}

float Camera::getAngleX() const {
    return m_angleX;
}

float Camera::getAngleY() const {
    return m_angleY;
}

float Camera::getAngleZ() const {
    return m_angleZ;
}

float Camera::getEZ() const {
    return m_EZ;
}
