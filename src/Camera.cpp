#include "Camera.h"
static const double PI = 3.141592653589793238462;

Camera::Camera(const int maxRadius) :
    m_point(0, maxRadius, 0), m_EZ(1), m_quat() {}

Camera::Camera(const Point3D& point, const double EZ_) :
    m_point(point), m_EZ(EZ_), m_quat() {}

Camera::Camera(const Point3D& point, const Quaternion& quat) :
    m_point(point), m_EZ(1), m_quat(quat) {}

void Camera::movePosition(const Point3D& newPosition) {
    m_point = newPosition;
}

void Camera::modifyPosition(const double posX, const double posY, const double posZ) {
    m_point.setX(m_point.getX() + posX);
    m_point.setY(m_point.getY() + posY);
    m_point.setZ(m_point.getZ() + posZ);
}

void Camera::setEZ (const double value) {
    m_EZ = value;
}

void Camera::rotateOnAxis(const size_t axis, const double angle) {
    MyArray<double, 3> canonicalVector;
    canonicalVector.fill(0);
    canonicalVector[axis] = 1;

    MyArray<double, 3> rotatedLocalAxis = Point3D(canonicalVector).rotatedByUnitQuat(m_quat).toArray();
    if(axis == 2) rotatedLocalAxis = canonicalVector;
    Quaternion axisAngleRotation(angle, rotatedLocalAxis);
    axisAngleRotation.convertToUnitQ();
    axisAngleRotation *= m_quat;
    m_quat = axisAngleRotation;
}

void Camera::rotateByUnitQuat(const Quaternion& quat) {
    Quaternion aux(quat);
    aux *= m_quat;
    m_quat = aux;
}

Point3D Camera::position() const {
    return m_point;
}

Quaternion Camera::quat() const {
    return m_quat;
}

double Camera::EZ() const {
    return m_EZ;
}

bool Camera::fscan(FILE* fp) {
    fscanf(fp, "Camera:\n");
    if (!m_point.fscan(fp)) {
        return false;
    }
    if (fscanf(fp, "\n%lf, %lf, %lf, %lf, %lf", &m_quat[0], &m_quat[1], &m_quat[2], &m_quat[3], &m_EZ) != 5) {
        return false;
    }
    return true;
}

void Camera::fprint(FILE* fp) {
    fprintf(fp, "Camera:\n");
    m_point.fprint(fp);
    fprintf(fp, "\n%f, %f, %f, %f, %f", m_quat[0], m_quat[1], m_quat[2], m_quat[3], m_EZ);
}
