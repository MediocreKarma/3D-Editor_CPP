#include "ShapeData.h"

Point3D::Point3D() :
    x(), y(), z() {}

Point3D::Point3D(const int& x_, const int& y_, const int& z_) :
    x(x_), y(y_), z(z_) {}

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
    double y_ = y * cos(alpha) - z * sin(alpha);
    double z_ = y * sin(alpha) + z * cos(alpha);
    y = y_;
    z = z_;
    translate(center.getX(), center.getY(), center.getZ());
}

void Point3D::rotateOY(const Point3D& center, const double& alpha) {
    translate(center.getX() * (-1), center.getY() * (-1), center.getZ() * (-1));
    double x_ = x * cos(alpha) - z * sin(alpha);
    double z_ = x * sin(alpha) + z * cos(alpha);
    x = x_;
    z = z_;
    translate(center.getX(), center.getY(), center.getZ());
}

void Point3D::rotateOZ(const Point3D& center, const double& alpha) {
    translate(center.getX() * (-1), center.getY() * (-1), center.getZ() * (-1));
    int x_ = x * cos(alpha) - y * sin(alpha);
    int y_ = x * sin(alpha) + y * cos(alpha);
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

void Point3D::fprint(FILE* fp) {
    fprintf(fp, "%i, %i, %i", x, y, z);
}

bool Point3D::fscan(FILE* fp) {
    if (fscanf(fp, "%i, %i, %i", &x, &y, &z) != 3) {
        x = y = z = 0;
        return false;
    }
    return true;
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

int Point2D::getY() const {
    return y;
}

bool Point2D::operator == (const Point2D& other) {
    return x == other.x && y == other.y;
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

constexpr int Section::RADIUS;

Section::Section() :
    m_lines(), m_centerPoint(), m_grabPoint(), m_active(false) {}

Section::Section(const MyVector<Line2D>& lines, const Point2D& centerPoint) :
    m_lines(lines), m_centerPoint(centerPoint.getX(), centerPoint.getY()), m_grabPoint(centerPoint.getX(), centerPoint.getY(), Section::RADIUS), m_active(false) {}

Section::Section(const Section& other) :
    m_lines(other.m_lines), m_centerPoint(other.centerPoint()), m_grabPoint(other.m_grabPoint), m_active(other.m_active) {}


size_t Section::size() const {
    return m_lines.size();
}

Point2D Section::centerPoint() const {
    return m_centerPoint;
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

Point3D Line3D::getP() const {
    return P;
}

Point3D Line3D::getQ() const {
    return Q;
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

double Line3D::getLength() const{
    double dx = abs(Q.getX() - P.getX());
    double dy = abs(Q.getY() - P.getY());
    double dz = abs(Q.getZ() - P.getZ());
    return sqrt(dx*dx + dy*dy + dz*dz);
}

void Line3D::rotateOX(const Point3D& center, const double& angle) {
    P.rotateOX(center, angle);
    Q.rotateOX(center, angle);
}

void Line3D::rotateOY(const Point3D& center, const double& angle) {
    P.rotateOY(center, angle);
    Q.rotateOY(center, angle);
}

void Line3D::rotateOZ(const Point3D& center, const double& angle) {
    P.rotateOZ(center, angle);
    Q.rotateOZ(center, angle);
}

bool Line3D::fscan(FILE* fp) {
    if (!P.fscan(fp)) {
        return false;
    }
    fscanf(fp, " - ");
    if (!Q.fscan(fp)) {
        return false;
    }
    fscanf(fp, "\n");
    return true;
}

void Line3D::fprint(FILE* fp) {
    P.fprint(fp);
    fprintf(fp, " - ");
    Q.fprint(fp);
    fprintf(fp, "\n");
}

Mesh::Mesh() :
    m_edges(), m_centerPoint(), m_angleX(0), m_angleY(0), m_angleZ(0) {}

Mesh::Mesh(const MyVector<Line3D>& edges) :
    m_edges(edges), m_centerPoint(0, 0, 0), m_angleX(0), m_angleY(0), m_angleZ(0) {
    updateCenterPoint();
}

Mesh::Mesh(const Mesh& other) :
    m_edges(other.m_edges), m_centerPoint(other.m_centerPoint), m_angleX(other.m_angleX), m_angleY(other.m_angleY), m_angleZ(other.m_angleZ) {}

void Mesh::updateCenterPoint() {
    for (size_t i = 0; i < size(); ++i) {
        m_centerPoint += m_edges[i].getP();
        m_centerPoint += m_edges[i].getQ();
    }
    m_centerPoint.setX(m_centerPoint.getX() / (2 * (int)size()));
    m_centerPoint.setY(m_centerPoint.getY() / (2 * (int)size()));
    m_centerPoint.setZ(m_centerPoint.getZ() / (2 * (int)size()));
}

size_t Mesh::size() const {
    return m_edges.size();
}

Line3D& Mesh::operator [] (const size_t& index) {
    return m_edges[index];
}

const Line3D& Mesh::operator [] (const size_t& index) const {
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

void Mesh::fprint(FILE* fp) {
    fprintf(fp, "Mesh: %u\n", size());
    fprintf(fp, "%f %f %f\n", m_angleX, m_angleY, m_angleZ);
    for (size_t i = 0; i < size(); ++i) {
        m_edges[i].fprint(fp);
    }
    m_centerPoint.fprint(fp);
    fprintf(fp, "\n");
}

bool Mesh::fscan(FILE* fp) {
    size_t edgesCount = 0;
    if (fscanf(fp, "Mesh: %u\n", &edgesCount) != 1) {
        return false;
    }
    if (fscanf(fp, "%lf %lf %lf", &m_angleX, &m_angleY, &m_angleZ) != 3) {
        return false;
    }
    m_edges.resize(edgesCount);
    for (size_t i = 0; i < edgesCount; ++i) {
        if (!m_edges[i].fscan(fp)) {
            return false;
        }
    }
    fscanf(fp, "\n");
    if (!m_centerPoint.fscan(fp)) {
        return false;
    }
    fscanf(fp, "\n");
    return true;
}

double Mesh::angleX() const {
    return m_angleX;
}

double Mesh::angleY() const {
    return m_angleY;
}

double Mesh::angleZ() const {
    return m_angleZ;
}

void Mesh::rotate(const double& angleX, const double& angleY, const double& angleZ) {
    const double e = 0.000000001;
    for (size_t i = 0; i < size(); ++i) {
        if(abs(angleX) > e) {
            m_edges[i].rotateOX(centerPoint(), angleX);
        }
        if(abs(angleY) > e) {
            m_edges[i].rotateOY(centerPoint(), angleY);
        }
        if(abs(angleZ) > e) {
            m_edges[i].rotateOZ(centerPoint(), angleZ);
        }
    }
    m_angleX += angleX;
    m_angleY += angleY;
    m_angleZ += angleZ;
}
