#include "ShapeData.h"

const double PI = 3.1415926535897932384626433832795028841971693993751;
const double err = 0.0000000000000000000001;

Point2D::Point2D() :
    x(), y() {}

Point2D::Point2D(const int& x_, const int& y_) :
    x(x_), y(y_) {}

Point2D::Point2D(const double& x_, const double& y_) :
    x(round(x_)), y(round(y_)) {}

Point2D::Point2D(const Point2D& other) :
    x(other.x), y(other.y) {}

int Point2D::getX() const {
    return x;
}

int Point2D::getY() const {
    return y;
}

void Point2D::setX(const int& x_){
    x = x_;
}

void Point2D::setY(const int& y_){
    y = y_;
}

bool Point2D::operator == (const Point2D& other) {
    return x == other.x && y == other.y;
}

Point2D& Point2D::operator = (const Point2D& other) {
    x = other.x;
    y = other.y;
    return *this;
}

Point2D& Point2D::operator += (const Point2D& other) {
    x += other.x;
    y += other.y;
    return *this;
}

Point2D& Point2D::operator -= (const Point2D& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Point2D Point2D::operator + (const Point2D& p) {
    return Point2D(x + p.x, y + p.y);
}

Point2D Point2D::operator - (const Point2D& p) {
    return Point2D(x - p.x, y - p.y);
}

void Point2D::translate (const Point2D& other) {
    x += other.x;
    y += other.y;
}

void Point2D::translate (const int& x_, const int& y_) {
    x += x_;
    y += y_;
}

void Point2D::display() const {
    std::cout<<x<<" "<<y<<"\n";
}

void Point2D::display(bool endlAfter) const {
    std::cout<<x<<" "<<y<<(endlAfter?"\n":"");
}

Line2D::Line2D() :
    P(), Q() {}

Line2D::Line2D(const Point2D& P_, const Point2D& Q_) :
    P(P_), Q(Q_) {}

Line2D::Line2D(const int& x0, const int& y0, const int& x1, const int& y1) :
    P(x0, y0), Q(x1, y1) {}

Point2D Line2D::getP() const {
    return P;
}

Point2D Line2D::getQ() const {
    return Q;
}

void Line2D::draw() {
    line(P.getX(), P.getY(), Q.getX(), Q.getY());
}

bool linesIntersect(const Point2D& A, const Point2D& B, const Point2D& C, const Point2D& D) {
    Point2D CmP = Point2D(C.getX() - A.getX(), C.getY() - A.getY());
    Point2D r = Point2D(B.getX() - A.getX(), B.getY() - A.getY());
    Point2D s = Point2D(D.getX() - C.getX(), D.getY() - C.getY());
    double CmPxr = CmP.getX() * r.getY() - CmP.getY() * r.getX();
    double CmPxs = CmP.getX() * s.getY() - CmP.getY() * s.getX();
    double rxs = r.getX() * s.getY() - r.getY() * s.getX();
    if (fabs(CmPxr) < 0.0000001){
        return ((C.getX() - A.getX() < 0) != (C.getX() - B.getX() < 0))
            || ((C.getY() - A.getY() < 0) != (C.getY() - B.getY() < 0));
    }
    if (fabs(rxs) < 0.0000001){
        return false;
    }
    double rxsr = 1 / rxs;
    double t = CmPxs * rxsr;
    double u = CmPxr * rxsr;
    return (t >= 0) && (t <= 1) && (u >= 0) && (u <= 1);
}

const int Section::RADIUS;

Section::Section() :
    m_points(), m_adjList(), m_centerPoint(), m_grabPoint(), m_active(false) {}

Section::Section(const MyVector<Point2D>& points, const Point2D& centerPoint, const MyVector<MyVector<size_t>>& adjList) :
    m_points(points), m_adjList(adjList), m_centerPoint(centerPoint), m_grabPoint(centerPoint.getX(), centerPoint.getY(), Section::RADIUS), m_active(false) {}

Section::Section(const Section& other) :
    m_points(other.m_points), m_adjList(other.m_adjList), m_centerPoint(other.m_centerPoint), m_grabPoint(other.m_grabPoint), m_active(other.m_active) {}


size_t Section::size() const {
    return m_points.size();
}

Point2D Section::centerPoint() const {
    return m_centerPoint;
}

void Section::addEdge(const size_t& index1, const size_t& index2) {
    for (size_t i = 0; i < m_adjList[index1].size(); ++i) {
        if (m_adjList[index1][i] == index2) {
            return;
        }
    }
    m_adjList[index1].push_back(index2);
    m_adjList[index2].push_back(index1);
}

void Section::draw(const int& primaryThemeColor, const int& fillColor, const int& borderColor) {
    setcolor(primaryThemeColor);
    for (size_t i = 0; i < size(); ++i) {
        int x0 = m_points[i].getX();
        int y0 = m_points[i].getY();
        if (x0 == -100) {
            continue;
        }
        for (size_t j = 0; j < m_adjList[i].size(); ++j) {
            if (i < m_adjList[i][j]) {
                int x1 = m_points[m_adjList[i][j]].getX();
                int y1 = m_points[m_adjList[i][j]].getY();
                if (x1 == -100) {
                    continue;
                }
                line(x0, y0, x1, y1);
            }
        }
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
    m_points = other.m_points;
    m_adjList = other.m_adjList;
    m_grabPoint = other.m_grabPoint;
    m_centerPoint = other.m_centerPoint;
    m_active = other.m_active;
    return *this;
}

Point2D& Section::operator [] (const size_t& index) {
    return m_points[index];
}

const Point2D& Section::operator [] (const size_t& index) const {
    return m_points[index];
}

Point3D::Point3D() :
    x(), y(), z() {}

Point3D::Point3D(const double& x_, const double& y_, const double& z_) :
    x(x_), y(y_), z(z_) {}

Point3D::Point3D(const Point3D& other) :
    x(other.x), y(other.y), z(other.z) {}

Point3D::Point3D(const MyArray<double, 3>& arr) :
    x(arr[0]), y(arr[1]), z(arr[2]) {}

double Point3D::getX() const {
    return x;
}

double Point3D::getY() const {
    return y;
}

double Point3D::getZ() const {
    return z;
}

void Point3D::setX(const double& x_) {
    x = x_;
}

void Point3D::setY(const double& y_) {
    y = y_;
}

void Point3D::setZ(const double& z_) {
    z = z_;
}

void Point3D::setPoint(const Point3D& pct) {
    x = pct.getX();
    y = pct.getY();
    z = pct.getZ();
}

void Point3D::round() {
    x = ::round(x);
    y = ::round(y);
    z = ::round(z);
}

MyArray<double, 3> Point3D::toArray() const {
    return MyArray<double,3>({x, y, z});
}

void Point3D::rotateByAxisVector(const double& angle, const MyArray<double, 3>& axis) {
    if (fabs(angle) < err) {
        return;
    }
    Quaternion pointQuat(0, toArray());
    Quaternion axisQuat(0, axis);
    axisQuat.normalize();
    Quaternion rotationQuat(angle, axisQuat.complex());
    rotationQuat.convertToUnitQ();
    Quaternion rotInverse = rotationQuat.inverse();
    Quaternion rotatedPoint = rotationQuat * pointQuat * rotInverse;
    *this = Point3D(rotatedPoint.complex());
}

Point3D Point3D::rotatedByAxisVector(const double& angle, const MyArray<double, 3>& axis) {
    if (fabs(angle) < err) {
        return *this;
    }
    Quaternion pointQuat(0, toArray());
    Quaternion axisQuat(0, axis);
    axisQuat.normalize();
    Quaternion rotationQuat(angle, axisQuat.complex());
    rotationQuat.convertToUnitQ();
    Quaternion rotInverse = rotationQuat.inverse();
    Quaternion rotatedPoint = rotationQuat * pointQuat * rotInverse;
    return Point3D(rotatedPoint.complex());
}

void Point3D::rotateByUnitQuat(const Quaternion& quat) {
    Quaternion pointQuat(0, toArray());
    Quaternion aux(quat);
    Quaternion rotatedPct = Quaternion(aux * pointQuat * aux.inverse());
    *this = Point3D(rotatedPct.complex());
}

Point3D Point3D::rotatedByUnitQuat(const Quaternion& quat) {
    Quaternion pointQuat(0, toArray());
    Quaternion aux(quat);
    Quaternion rotatedPct = Quaternion(aux * pointQuat * aux.inverse());
    return Point3D(rotatedPct.complex());
}

bool Point3D::operator == (const Point3D& other) const {
    return fabs(x - other.x) < err && fabs(y - other.y) < err && fabs(z - other.z) < err;
}

Point3D& Point3D::operator += (const Point3D& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Point3D& Point3D::operator -= (const Point3D& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Point3D Point3D::operator + (const Point3D& other) const {
    Point3D pct;
    pct.x = x + other.x;
    pct.y = y + other.y;
    pct.z = z + other.z;
    return pct;
}

Point3D Point3D::operator - (const Point3D& other) const {
    Point3D pct;
    pct.x = x - other.x;
    pct.y = y - other.y;
    pct.z = z - other.z;
    return pct;
}

Point3D Point3D::operator * (const double& scalar) const {
    double cx, cy, cz;
    cx = x * scalar;
    cy = y * scalar;
    cz = z * scalar;
    return Point3D(cx, cy, cz);
}

void Point3D::translate(const double& xTranslate, const double& yTranslate, const double& zTranslate) {
    x += xTranslate;
    y += yTranslate;
    z += zTranslate;
}

void Point3D::fprint(FILE* fp) {
    fprintf(fp, "%f, %f, %f", x, y, z);
}

bool Point3D::fscan(FILE* fp) {
    if (fscanf(fp, "%lf, %lf, %lf", &x, &y, &z) != 3) {
        x = y = z = 0;
        return false;
    }
    return true;
}

void Point3D::display() const {
    std::cout<<x<<" "<<y<<" "<<z<<"\n";
}

void Point3D::display(bool endlAfter) const {
    std::cout<<x<<" "<<y<<" "<<z<<(endlAfter?"\n":"");
}

double Point3D::norm() const {
    return sqrt(x * x + y * y + z * z);
}
void Point3D::normalize() {
    const double n = norm();
    x /= n;
    y /= n;
    z /= n;
}

double dot(const Point3D& p1, const Point3D& p2) {
    return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
}

Point3D cross(const Point3D& p1, const Point3D& p2) {
    double cx, cy, cz;
    cx = p1.y * p2.z - p1.z * p2.y;
    cy = -(p1.x * p2.z - p1.z * p2.x);
    cz = p1.x * p2.y - p1.y * p2.x;
    return Point3D(cx, cy, cz);
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

double Line3D::length() const{
    double dx = abs(Q.getX() - P.getX());
    double dy = abs(Q.getY() - P.getY());
    double dz = abs(Q.getZ() - P.getZ());
    return sqrt(dx*dx + dy*dy + dz*dz);
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
    m_points(), m_adjList(), m_centerPoint(0, 0, 0), m_angleX(0.0), m_angleY(0.0), m_angleZ(0.0), m_quat(1, 0, 0, 0), m_scaleX(1), m_scaleY(1), m_scaleZ(1) {}

Mesh::Mesh(const MyVector<Point3D>& points, const MyVector<MyVector<size_t>>& adjList) :
    m_points(points), m_adjList(adjList), m_centerPoint(0, 0, 0), m_angleX(0.0), m_angleY(0.0), m_angleZ(0.0), m_quat(1, 0, 0, 0), m_scaleX(1), m_scaleY(1), m_scaleZ(1) {
    updateCenterPoint();
}

Mesh::Mesh(const Mesh& other) :
    m_points(other.m_points), m_adjList(other.m_adjList), m_centerPoint(other.m_centerPoint), m_angleX(other.m_angleX), m_angleY(other.m_angleY), m_angleZ(other.m_angleZ), m_quat(other.m_quat),
    m_scaleX(other.m_scaleX), m_scaleY(other.m_scaleY), m_scaleZ(other.m_scaleZ){}

void Mesh::updateCenterPoint() {
    Point3D auxPoint;
    for (size_t i = 0; i < size(); ++i) {
        auxPoint += m_points[i];
    }
    m_centerPoint = Point3D(auxPoint.getX() / (int)size(), auxPoint.getY() / (int)size(), auxPoint.getZ() / (int)size());
}

size_t Mesh::size() const {
    return m_points.size();
}

double Mesh::scaleX() const {
    return m_scaleX;
}

double Mesh::scaleY() const {
    return m_scaleY;
}

double Mesh::scaleZ() const {
    return m_scaleZ;
}

Point3D& Mesh::operator [] (const size_t& index) {
    return m_points[index];
}

const Point3D& Mesh::operator [] (const size_t& index) const {
    return m_points[index];
}

Mesh& Mesh::operator = (const Mesh& other) {
    m_points = other.m_points;
    m_centerPoint = other.m_centerPoint;
    m_adjList = other.m_adjList;
    m_quat = other.m_quat;
    m_scaleX = other.m_scaleX;
    m_scaleY = other.m_scaleY;
    m_scaleZ = other.m_scaleZ;
    return *this;
}

Mesh& Mesh::operator = (const FixedMesh& other) {
    //TODO
}

MyVector<size_t> Mesh::adjListAt(const size_t& index) const {
    return m_adjList[index];
}

MyVector<MyVector<size_t>>& Mesh::adjacencyListReference() {
    return m_adjList;
}

const MyVector<MyVector<size_t>>& Mesh::adjacencyList() const {
    return m_adjList;
}

void Mesh::erase(const size_t& index) {
    m_points.erase(m_points.begin() + index);
    m_adjList.erase(m_adjList.begin() + index);
    for (size_t i = 0; i < m_adjList.size(); ++i) {
        for (size_t j = 0; j < m_adjList[i].size(); ++j) {
            if (m_adjList[i][j] == index) {
                m_adjList[i].erase(m_adjList[i].begin() + j);
                break;
            }
        }
    }
    for (MyVector<size_t>& listInfo : m_adjList) {
        for (size_t& k : listInfo) {
            if (k > index) {
                --k;
            }
        }
    }
}

void Mesh::deleteIndexConnection(const size_t& index1, const size_t& index2) {
    for (size_t i = 0; i < m_adjList[index1].size(); ++i) {
        if (m_adjList[index1][i] == index2) {
            m_adjList[index1].erase(m_adjList[index1].begin() + i);
            break;
        }
    }
    for (size_t i = 0; i < m_adjList[index2].size(); ++i) {
        if (m_adjList[index2][i] == index1) {
            m_adjList[index2].erase(m_adjList[index2].begin() + i);
            break;
        }
    }
}

void Mesh::addIndexConnections(const size_t& index, const MyVector<size_t>& listAtIndex) {
    for (size_t i = 0; i < listAtIndex.size(); ++i) {
        bool alreadyConnected = false;
        for (size_t j = 0; j < m_adjList[index].size(); ++j) {
            if (listAtIndex[i] == m_adjList[index][j]) {
                alreadyConnected = true;
                break;
            }
        }
        if (!alreadyConnected) {
            m_adjList[index].push_back(listAtIndex[i]);
            m_adjList[listAtIndex[i]].push_back(index);
        }
    }
}

void Mesh::addPoint(const double& x, const double& y, const double& z) {
    m_points.push_back(Point3D(x, y, z));
    m_adjList.push_back(MyVector<size_t>());
}

void Mesh::addPoint(const Point3D& point) {
    m_points.push_back(point);
    m_adjList.push_back(MyVector<size_t>());
}

void Mesh::addEdge(const size_t& index1, const size_t& index2) {
    for (size_t i = 0; i < m_adjList[index1].size(); ++i) {
        if (m_adjList[index1][i] == index2) {
            return;
        }
    }
    m_adjList[index1].push_back(index2);
    m_adjList[index2].push_back(index1);
}

void Mesh::translate(const double& xTranslate, const double& yTranslate, const double& zTranslate) {
    for (auto& pnt : m_points) {
        pnt.translate(xTranslate, yTranslate, zTranslate);
    }
    m_centerPoint.translate(xTranslate, yTranslate, zTranslate);
}

void Mesh::translate(const Point3D& pntTranslate) {
    for (auto& pnt : m_points) {
        pnt.translate(pntTranslate.getX(), pntTranslate.getY(), pntTranslate.getZ());
    }
    m_centerPoint.translate(pntTranslate.getX(), pntTranslate.getY(), pntTranslate.getZ());
}

Point3D Mesh::centerPoint() const {
    return m_centerPoint;
}

void Mesh::fprint(FILE* fp) {
    fprintf(fp, "Mesh: %u\n", size());
    fprintf(fp, "%f %f %f %f\n", m_quat[0], m_quat[1], m_quat[2], m_quat[3]);
    for (size_t i = 0; i < size(); ++i) {
        m_points[i].fprint(fp);
        fprintf(fp, "\n");
    }
    for (size_t i = 0; i < size(); ++i) {
        fprintf(fp, "Connections: %u - ", m_adjList[i].size());
        for (size_t j = 0; j < m_adjList[i].size(); ++j) {
            fprintf(fp, "%u, ", m_adjList[i][j]);
        }
        fprintf(fp, "\n");
    }
    m_centerPoint.fprint(fp);
    fprintf(fp, "\n");
}

bool Mesh::fscan(FILE* fp) {
    size_t edgesCount = 0;
    if (fscanf(fp, "Mesh: %u\n", &edgesCount) != 1) {
        return false;
    }
    if (fscanf(fp, "%lf %lf %lf %lf", &m_quat[0], &m_quat[1], &m_quat[2], &m_quat[3]) != 4) {
        return false;
    }
    m_points.resize(edgesCount);
    for (size_t i = 0; i < edgesCount; ++i) {
        if (!m_points[i].fscan(fp)) {
            return false;
        }
        fscanf(fp, "\n");
    }
    m_adjList.resize(edgesCount);
    for (size_t i = 0; i < size(); ++i) {
        size_t iAdjListSize = 0;
        if (fscanf(fp, "Connections: %u - ", &iAdjListSize) != 1) {
            return false;
        }
        m_adjList[i].resize(iAdjListSize);
        for (size_t j = 0; j < iAdjListSize; ++j) {
            if (fscanf(fp, "%u, ", &m_adjList[i][j]) != 1) {
                return false;
            }
        }
        fscanf(fp, "\n");
    }
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

void Mesh::rotateDisplayAngle() {
    MyArray<double, 3> eulerAngles = m_quat.toEuler();
    m_angleX = eulerAngles[0];
    m_angleY = eulerAngles[1];
    m_angleZ = eulerAngles[2];
}

void Mesh::rotate(const double& angleX_, const double& angleY_, const double& angleZ_) {
    const double e = 0.0000000000001;
    if (fabs(angleX_) > e) {
        rotateOnAxis(centerPoint(), Point3D(1, 0, 0), angleX_);
    }
    if (fabs(angleY_) > e) {
        rotateOnAxis(centerPoint(), Point3D(0, 1, 0), angleY_);
    }
    if (fabs(angleZ_) > e) {
        rotateOnAxis(centerPoint(), Point3D(0, 0, 1), angleZ_);
    }
}

void Mesh::rotateOnAxis(const Point3D& center, const Point3D& axis, const double& angle) {
    translate(-center.getX(), -center.getY(), -center.getZ());

    Quaternion rotationQuat(angle, axis.toArray());
    rotationQuat.convertToUnitQ();
    const double e = 0.0000000000001;
    if (fabs(angle) >= e) {
        for (size_t i = 0; i < size(); ++i) {
            m_points[i].rotateByUnitQuat(rotationQuat);
        }
    }
    rotationQuat *= m_quat;
    m_quat = rotationQuat;
    translate(center.getX(), center.getY(), center.getZ());
}


void Mesh::scaleEven(const double& scaleFactor) {
    Point3D center = m_centerPoint;
    translate(-center.x, -center.y, -center.z);
    double translateAmt = scaleFactor - 1;
    if (fabs(translateAmt) > 0.0000001) {
        for (size_t i = 0; i < m_points.size(); i++) {
            m_points[i].x *= scaleFactor;
            m_points[i].y *= scaleFactor;
            m_points[i].z *= scaleFactor;
        }
    }
    translate(center);
    m_scaleX *= scaleFactor;
    m_scaleY *= scaleFactor;
    m_scaleZ *= scaleFactor;
}

void Mesh::scaleAxis(const double& scaleFactor, const size_t& axis) {
    //ALL SCALE IS LOCAL!!
    MyArray<bool, 3> axes = {false, false, false};
    axes[axis] = true;
    Point3D center = m_centerPoint;
    translate(-center.x, -center.y, -center.z);
    Quaternion quat = m_quat;
    resetRotation();
    switch(axis) {
        case 0:
            for (size_t i = 0; i < m_points.size(); i++) {
                m_points[i].x *= scaleFactor;
            }
            m_scaleX *= scaleFactor;
            break;
        case 1:
            for (size_t i = 0; i < m_points.size(); i++) {
                m_points[i].y *= scaleFactor;
            }
            m_scaleY *= scaleFactor;
            break;
        case 2:
            for (size_t i = 0; i < m_points.size(); i++) {
                m_points[i].z *= scaleFactor;
            }
            m_scaleZ *= scaleFactor;
            break;
    }
    translate(center);
    rotateByUnitQuat(quat);
}

void Mesh::mirror(const size_t& axis) {
    scaleAxis(-1, axis);
}

Quaternion Mesh::quat() const {
    return m_quat;
}

void Mesh::resetRotation() {
    Point3D center = m_centerPoint;
    translate(-center.x, -center.y, -center.z);
    const double e = 0.000000001;
    for (Point3D& pct : m_points) {
        pct.rotateByUnitQuat(m_quat.inverse());
        if (fabs(pct.getX() - ::lround(pct.getX())) < e) {
            pct.setX((int)::lround(pct.getX()));
        }
        if (fabs(pct.getY() - ::lround(pct.getY())) < e) {
            pct.setY((int)::lround(pct.getY()));
        }
        if (fabs(pct.getZ() - ::lround(pct.getZ())) < e) {
            pct.setZ((int)::lround(pct.getZ()));
        }
    }
    m_angleX = 0;
    m_angleY = 0;
    m_angleZ = 0;
    m_quat = Quaternion(1, 0, 0, 0);
    translate(center);
}

void Mesh::resetScale() {
    scaleAxis(1 / m_scaleX, 0);
    scaleAxis(1 / m_scaleY, 1);
    scaleAxis(1 / m_scaleZ, 2);
    const double e = 0.000000001;
    for (Point3D& pct : m_points) {
        pct.rotateByUnitQuat(m_quat.inverse());
        if (fabs(pct.getX() - ::lround(pct.getX())) < e) {
            pct.setX((int)::lround(pct.getX()));
        }
        if (fabs(pct.getY() - ::lround(pct.getY())) < e) {
            pct.setY((int)::lround(pct.getY()));
        }
        if (fabs(pct.getZ() - ::lround(pct.getZ())) < e) {
            pct.setZ((int)::lround(pct.getZ()));
        }
    }
}

void Mesh::rotateByUnitQuat(const Quaternion& quat) {
    Point3D center(m_centerPoint);
    translate(-center.getX(), -center.getY(), -center.getZ());
    for (auto& pnt : m_points) {
        pnt.rotateByUnitQuat(quat);
    }
    m_quat = quat * m_quat;
    rotateDisplayAngle();
    translate(center.getX(), center.getY(), center.getZ());
}

Point3D Mesh::localAxis(int axis) const {
    MyArray<int, 3> axisArray{0, 0, 0};
    axisArray[axis] = true;
    Point3D aux(axisArray[0], axisArray[1], axisArray[2]);
    aux.rotateByUnitQuat(m_quat);
    return aux;
}

MyArray<Point3D, 3> Mesh::localAxes() const {
    return MyArray<Point3D, 3>{localAxis(0), localAxis(1), localAxis(2)};
}

MyArray<Point3D, 2> Mesh::getBoundingBoxCorners() const {
    if (!m_points.size()) {
        return MyArray<Point3D, 2>{Point3D(0, 0, 0), Point3D(0, 0, 0)};
    }
    Point3D bottomLeft = m_points[0], topRight = m_points[0];
    for (auto& point : m_points) {
        if (point.getX() < bottomLeft.getX()) {
            bottomLeft.setX(point.getX());
        }
        if (point.getY() < bottomLeft.getY()) {
            bottomLeft.setY(point.getY());
        }
        if (point.getZ() < bottomLeft.getZ()) {
            bottomLeft.setZ(point.getZ());
        }
        if (point.getX() > topRight.getX()) {
            topRight.setX(point.getX());
        }
        if (point.getY() > topRight.getY()) {
            topRight.setY(point.getY());
        }
        if (point.getZ() > topRight.getZ()) {
            topRight.setZ(point.getZ());
        }
    }
    return MyArray<Point3D, 2>{bottomLeft, topRight};
}

MeshTransforms Mesh::transforms() const {
    MeshTransforms transforms;
    transforms.t[0][0] = m_scaleX;
    transforms.t[0][1] = m_scaleY;
    transforms.t[0][2] = m_scaleZ;
    transforms.t[1][0] = m_angleX * 180 / PI;
    transforms.t[1][1] = m_angleY * 180 / PI;
    transforms.t[1][2] = m_angleZ * 180 / PI;
    transforms.t[2][0] = m_centerPoint.x;
    transforms.t[2][1] = m_centerPoint.y;
    transforms.t[2][2] = m_centerPoint.z;
    return transforms;
}

void Mesh::resetTransforms() {
    Point3D center(m_centerPoint);
    translate(-center.x, -center.y, -center.z);
    resetRotation();
    resetScale();
}

void Mesh::applyTransforms(const MeshTransforms& transforms) {
    resetTransforms();
    for (size_t i = 0; i < 3; ++i) {
        scaleAxis(transforms[0][i], i);
    }
    Quaternion quat(transforms[1][0] * PI / 180, transforms[1][1] * PI / 180, transforms[1][2] * PI / 180);
    quat.display();
    rotateByUnitQuat(quat);
    translate(Point3D(transforms[2]));
}

void Mesh::setTransform(size_t transform, size_t axis, const double& value) {
    switch (transform) {
        case 0: {
            double currentValue = 1;
            switch (axis) {
                case 0:
                    currentValue = m_scaleX;
                    break;
                case 1:
                    currentValue = m_scaleY;
                    break;
                case 2:
                    currentValue = m_scaleZ;
                    break;
                default:;
            }
            scaleAxis(value / currentValue, axis);
            break;
        }
        case 1: {
            MyArray<double, 3> eulerAngles = m_quat.toEuler();
            double val = value;
            if (val > PI / 2) {
                val = - 2 * PI + value;
            }
            eulerAngles[axis] = val;
            Quaternion finalQuat = Quaternion(eulerAngles[0], eulerAngles[1], eulerAngles[2]);
            resetRotation();
            rotateByUnitQuat(finalQuat);
            break;
        }
        case 2: {
            Point3D center(m_centerPoint);
            switch (axis) {
                case 0:
                    translate(value - center.x, 0, 0);
                    break;
                case 1:
                    translate(0, value - center.y, 0);
                    break;
                case 2:
                    translate(0, 0, value - center.z);
                    break;
                default:;
            }
        }
        default:;
    }
}

IntegerPoint3D::IntegerPoint3D() :
    x(0), y(0), z(0) {}

IntegerPoint3D::IntegerPoint3D(const Point3D& point3d) : IntegerPoint3D() {
    *this = point3d;
}

IntegerPoint3D::IntegerPoint3D(const int& x_, const int& y_, const int& z_) :
    x(x_), y(y_), z(z_) {}

IntegerPoint3D::IntegerPoint3D(const IntegerPoint3D& other) :
    x(other.x), y(other.y), z(other.z) {}

IntegerPoint3D& IntegerPoint3D::operator = (const Point3D& point3d) {
    if (point3d.x > 0) {
        x = (int)(point3d.x + 0.5);
    }
    else {
        x = (int)(point3d.x - 0.5);
    }
    if (point3d.y > 0) {
        y = (int)(point3d.y + 0.5);
    }
    else {
        y = (int)(point3d.y - 0.5);
    }
    if (point3d.z > 0) {
        z = (int)(point3d.z + 0.5);
    }
    else {
        z = (int)(point3d.z - 0.5);
    }
    return *this;
}

IntegerPoint3D& IntegerPoint3D::operator = (const IntegerPoint3D& rhs) {
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    return *this;
}

bool operator < (const IntegerPoint3D& a, const IntegerPoint3D& b) {
    return a.x < b.x || (a.x == b.x && a.y < b.y) ||
            (a.x == b.x && a.y == b.y && a.z < b.z);
}

FixedMesh::FixedMesh() :
    m_points(), m_pointIterators(), m_adjList() {}

FixedMesh::FixedMesh(const Mesh& other) : FixedMesh() {
    Point3D centerPoint = other.centerPoint();
    MyVector<IntegerPoint3D> helperPoints(other.size());
    for (size_t i = 0; i < other.size(); ++i) {
        Point3D point = other[i];
        point.translate(-centerPoint.x, -centerPoint.y, -centerPoint.z);
        helperPoints[i] = point;
        m_points.emplace_front(helperPoints[i], CircularButton());
        m_pointIterators.insert(helperPoints[i], m_points.begin());
    }
    const MyVector<MyVector<size_t>>& adjList = other.adjacencyList();
    for (size_t i = 0; i < adjList.size(); ++i) {
        iterator_type it = m_pointIterators[helperPoints[i]];
        for (size_t j : adjList[i]) {
            if (helperPoints[i] < helperPoints[j]) {
                m_adjList[it].insert(m_pointIterators[helperPoints[j]]);
            }
        }
    }
}

void FixedMesh::addEdge(const IntegerPoint3D& x, const IntegerPoint3D& y) {
    auto it1 = m_pointIterators.find(x);
    auto it2 = m_pointIterators.find(y);
    if (it1 == m_pointIterators.end() || it2 == m_pointIterators.end()) {
        return;
    }
    addEdge(it1->value, it2->value);
}

void FixedMesh::addEdge(iterator_type it1, iterator_type it2) {
    if (it1->point < it2->point) {
        m_adjList[it1].insert(it2);
    }
    else {
        m_adjList[it2].insert(it1);
    }
}

void FixedMesh::addPoint(const IntegerPoint3D& x) {
    m_points.emplace_front(x, CircularButton());
    m_pointIterators.insert(x, m_points.begin());
}

void FixedMesh::addPoint(const int& x, const int& y, const int& z) {
    addPoint(IntegerPoint3D(x, y, z));
}

void FixedMesh::erasePoint(const int& x, const int& y, const int& z) {
    erasePoint(IntegerPoint3D(x, y, z));
}

void FixedMesh::erasePoint(const IntegerPoint3D& x) {
    auto it = m_pointIterators.find(x);
    if (it != m_pointIterators.end()) {
        erasePoint(it->value);
    }
}

void FixedMesh::erasePoint(iterator_type it) {
    for (auto& container : m_adjList) {
        if (container.value.contains(it)) {
        }
        container.value.erase(it);
    }
    m_adjList.erase(it);
    m_pointIterators.erase(it->point);
    m_points.erase(it);
}

void FixedMesh::eraseConnection(const IntegerPoint3D& x, const IntegerPoint3D& y) {
    auto it1 = m_pointIterators.find(x);
    auto it2 = m_pointIterators.find(y);
    if (it1 == m_pointIterators.end() || it2 == m_pointIterators.end()) {
        return;
    }
    eraseConnection(it1->value, it2->value);
}

void FixedMesh::eraseConnection(iterator_type it1, iterator_type it2) {
    if (it1->point < it2->point) {
        m_adjList[it1].erase(it2);
    }
    else {
        m_adjList[it2].erase(it1);
    }
}

void FixedMesh::renderPoint(iterator_type pointIt, const Point2D& conversion) {
    pointIt->button2d = CircularButton(conversion.x, conversion.y, 5);
}

void FixedMesh::drawMesh(const int secondaryThemeColor, const int accentColor) {
    setcolor(secondaryThemeColor);
    setlinestyle(SOLID_LINE, 0, 1);
    for (iterator_type it = m_points.begin(); it != m_points.end(); ++it) {
        Point2D P(it->button2d.getX(), it->button2d.getY());
        for (iterator_type it2 : m_adjList[it]) {
            Point2D Q(it2->button2d.getX(), it2->button2d.getY());
            line(P.x, P.y, Q.x, Q.y);
        }
    }
    for (auto& data : m_points) {
        data.button2d.drawLabel(accentColor, accentColor);
    }
}

void FixedMesh::draw2DLinesFrom(const IntegerPoint3D& point, const char staticLayer) {
    auto it = m_pointIterators.find(point);
    if (it != m_pointIterators.end()) {
        draw2DLinesFrom(it->value, staticLayer);
    }
}

void FixedMesh::draw2DLinesFrom(iterator_type it, const char staticLayer) {
    IntegerPoint3D point = it->point;
    switch (staticLayer) {
    case 'x':
        for (iterator_type itOther : m_adjList[it]) {
            line(point.y, point.z, itOther->point.y, itOther->point.z);
        }
        return;
    case 'y':
        for (iterator_type itOther : m_adjList[it]) {
            line(point.x, point.z, itOther->point.x, itOther->point.z);
        }
        return;
    case 'z':
        for (iterator_type itOther : m_adjList[it]) {
            line(point.x, point.y, itOther->point.x, itOther->point.y);
        }
        return;
    default:;
    }
}

void FixedMesh::updatePointValue(iterator_type it, const IntegerPoint3D& newValue) {
    m_pointIterators.insert(newValue, it);
    m_pointIterators.erase(it->point);
    it->point = newValue;
}

bool FixedMesh::cutLines(const Line2D cuttingLine) {
    bool anyCuts = false;
    for (iterator_type it1 = m_points.begin(); it1 != m_points.end(); ++it1) {
        MyList<iterator_type> deletableIterators;
        for (iterator_type it2 : m_adjList[it1]) {
            Point2D P1(it1->button2d.getX(), it1->button2d.getY());
            Point2D P2(it2->button2d.getX(), it2->button2d.getY());
            if (linesIntersect(cuttingLine.getP(), cuttingLine.getQ(), P1, P2)) {
                deletableIterators.push_back(it2);
                anyCuts = true;
            }
        }
        for (iterator_type it2 : deletableIterators) {
            eraseConnection(it1, it2);
        }
    }
    return anyCuts;
}


FixedMesh::iterator_type FixedMesh::begin() {
    return m_points.begin();
}

FixedMesh::iterator_type FixedMesh::end() {
    return m_points.end();
}

size_t FixedMesh::size() {
    return m_points.size();
}

size_t FixedMesh::countConnections(const IntegerPoint3D& x) {
    auto it = m_pointIterators.find(x);
    if (it == m_pointIterators.end()) {
        throw std::invalid_argument("point not found");
    }
    return countConnections(it->value);
}

size_t FixedMesh::countConnections(iterator_type it) {
    return m_adjList[it].size();
}

FixedMesh::iterator_type FixedMesh::find(const IntegerPoint3D& x) {
    auto it = m_pointIterators.find(x);
    if (it == m_pointIterators.end()) {
        return m_points.end();
    }
    return it->value;
}

FixedMesh::iterator_type FixedMesh::hitCollisionIterator(const int x, const int y) {
    for (iterator_type pointIt = m_points.begin(); pointIt != m_points.end(); ++pointIt) {
        if (pointIt->button2d.hitCollision(x, y)) {
            return pointIt;
        }
    }
    return m_points.end();
}

const MyHashSet<FixedMesh::iterator_type>& FixedMesh::adjacentPoints(const IntegerPoint3D& x) {
    auto it = m_pointIterators.find(x);
    if (it == m_pointIterators.end()) {
        throw std::invalid_argument("point not found");
    }
    return adjacentPoints(it->value);
}

const MyHashSet<FixedMesh::iterator_type>& FixedMesh::adjacentPoints(iterator_type it) {
    return m_adjList[it];
}

bool FixedMesh::contains(const IntegerPoint3D& x) const {
    return m_pointIterators.contains(x);
}
