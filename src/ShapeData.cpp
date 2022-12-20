#include "ShapeData.h"
#include "Quaternion.h"
#include <iostream>
const double PI = 3.14159265359;
const double err = 0.000000000000000000000;

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

bool Point2D::operator == (const Point2D& other) {
    return x == other.x && y == other.y;
}

Line2D::Line2D() :
    P(), Q() {}

Line2D::Line2D(const Point2D& P_, const Point2D& Q_) :
    P(P_), Q(Q_) {}

Line2D::Line2D(const int& x0, const int& y0, const int& x1, const int& y1) :
    P(x0, y0), Q(x1, y1) {}

Point2D Line2D::getP() {
    return P;
}

Point2D Line2D::getQ() {
    return Q;
}

void Line2D::draw() {
    line(P.getX(), P.getY(), Q.getX(), Q.getY());
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
    x(arr[0]), y(arr[1]), z(arr[2]) {
        /*if (fabs(x) < err) {
            x = 0;
        }
        if (fabs(y) < err) {
            y = 0;
        }
        if (fabs(z) < err) {
            z = 0;
        }*/
    }

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

MyArray<double, 3> Point3D::toArray() const {
    return MyArray<double,3>({x, y, z});
}

/*void Point3D::rotateOX(const Point3D& center, const double& alpha) {
    translate(-center.getX(), -center.getY(), -center.getZ());
    Quaternion rotationQuat(alpha, {1, 0, 0});
    rotationQuat.convertToUnitQ();
    (*this) = rotateByUnitQuat(rotationQuat);
    translate(center.getX(), center.getY(), center.getZ());
}

void Point3D::rotateOY(const Point3D& center, const double& alpha) {
    translate(-center.getX(), -center.getY(), -center.getZ());
    Quaternion rotationQuat(alpha, {0, 1, 0});
    rotationQuat.convertToUnitQ();
    (*this) = rotateByUnitQuat(rotationQuat);
    translate(center.getX(), center.getY(), center.getZ());
}

void Point3D::rotateOZ(const Point3D& center, const double& alpha) {
    translate(-center.getX(), -center.getY(), -center.getZ());
    Quaternion rotationQuat(-alpha, {0, 0, 1});
    rotationQuat.convertToUnitQ();
    (*this) = rotateByUnitQuat(rotationQuat);
    translate(center.getX(), center.getY(), center.getZ());
}*/

Point3D Point3D::rotateByAxisVector(const double& angle, const MyArray<double, 3>& axis) {
    //takes axis (can be local or global), rotates around it
    Quaternion pointQuat(0, toArray());
    Quaternion axisQuat(0, axis);
    axisQuat.normalize();
    Quaternion rotationQuat(angle, axisQuat.complex());
    rotationQuat.convertToUnitQ();
    Quaternion rotInverse = rotationQuat.inverse();
    Quaternion rotatedPoint = rotationQuat * pointQuat * rotInverse;
    return Point3D(rotatedPoint.complex());
}

Point3D Point3D::rotateByUnitQuat(const Quaternion& quat) {
    Quaternion pointQuat(0, toArray());
    Quaternion aux(quat);
    Quaternion rotatedPct = Quaternion(aux * pointQuat * aux.inverse());
    return Point3D(rotatedPct.complex());
}

Point3D& Point3D::operator += (const Point3D& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
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
    m_points(), m_adjList(), m_centerPoint(0, 0, 0), m_angleX(0.0), m_angleY(0.0), m_angleZ(0.0), m_quat(1, 0, 0, 0) {}

Mesh::Mesh(const MyVector<Point3D>& points, const MyVector<MyVector<size_t>>& adjList) :
    m_points(points), m_adjList(adjList), m_centerPoint(0, 0, 0), m_angleX(0.0), m_angleY(0.0), m_angleZ(0.0), m_quat(1, 0, 0, 0) {
    updateCenterPoint();
}

Mesh::Mesh(const Mesh& other) :
    m_points(other.m_points), m_adjList(other.m_adjList), m_centerPoint(other.m_centerPoint), m_angleX(other.m_angleX), m_angleY(other.m_angleY), m_angleZ(other.m_angleZ), m_quat(other.quat()) {}

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
    return *this;
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

void Mesh::addIndexConnections(const size_t& index, const MyVector<size_t>& listAtIndex) { // O(n^2) -> O(n) daca implementam hashset-uri :D
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
            std::cout << "here";
            return;
        }
    }
    m_adjList[index1].push_back(index2);
    m_adjList[index2].push_back(index1);
}

void Mesh::translate(const double& xTranslate, const double& yTranslate, const double& zTranslate) {
    for (size_t i = 0; i < size(); ++i) {
        m_points[i].translate(xTranslate, yTranslate, zTranslate);
    }
    m_centerPoint.translate(xTranslate, yTranslate, zTranslate);
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
    //vom modifica in rotire pe axe globale (sau locale mai tarziu).
    //nu stiu ce facem totusi cu unghiurile
    //n am reusit sa mi dau seama cum functioneaza butoanele tale
    //ca altfel as fi facut eu modificarile pe care voiam sa le fac

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
            m_points[i] = m_points[i].rotateByUnitQuat(rotationQuat);
        }
    }
    rotationQuat*=m_quat;
    m_quat = rotationQuat;

    translate(center.getX(), center.getY(), center.getZ());
}

void Mesh::scaleEven(const double& scaleFactor) {
    Point3D center = centerPoint();
    int translateAmt = scaleFactor - 1;
    for (size_t i = 0; i < m_points.size(); i++) {
        m_points[i].translate(translateAmt * (m_points[i].getX() - center.getX()), translateAmt * (m_points[i].getY() - center.getY()), translateAmt * (m_points[i].getZ() - center.getZ()));
    }
}

void Mesh::scaleAxis(bool isLocal, const double& scaleFactor, const size_t& axis) {
    //TODO: adapt to quaternions
    MyArray<bool, 3> axes = {false, false, false};
    axes[axis] = 1;
    Point3D center = centerPoint();
    double translateAmt = scaleFactor - 1;

    MyArray<double, 3> angles = {m_angleX, m_angleY, m_angleZ};

    if (isLocal == true) {
        rotate(-angles[0], -angles[1], -angles[2]);
    }

    for (size_t i = 0; i < m_points.size(); i++) {
        m_points[i].translate(axes[0] * translateAmt * (m_points[i].getX() - center.getX()), axes[1] * translateAmt * (m_points[i].getY() - center.getY()), axes[2] * translateAmt * (m_points[i].getZ() - center.getZ()));
    }

    if (isLocal == true) {
        rotate(angles[0], angles[1], angles[2]);
    }
}

void Mesh::mirror(bool isLocal, const size_t& axis) {
    scaleAxis(isLocal, -1, axis);
}

Quaternion Mesh::quat() const {
    return m_quat;
}

void Mesh::resetRotation() {
    MyArray<double, 3> aux = Point3D(centerPoint()).toArray();
    translate(-aux[0], -aux[1], -aux[2]);
    const double e = 0.000000001;
    for (Point3D& pct : m_points) {
        pct = pct.rotateByUnitQuat(m_quat.inverse());
        if (fabs(pct.getX() - round(pct.getX())) < e) {
            pct.setX((int)round(pct.getX()));
        }
        if (fabs(pct.getY() - round(pct.getY())) < e) {
            pct.setY((int)round(pct.getY()));
        }
        if (fabs(pct.getZ() - round(pct.getZ())) < e) {
            pct.setZ((int)round(pct.getZ()));
        }
    }
    m_angleX = 0;
    m_angleY = 0;
    m_angleZ = 0;
    m_quat = Quaternion(1, 0, 0, 0);
    translate(aux[0], aux[1], aux[2]);
}
