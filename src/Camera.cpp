#include "Camera.h"

static const double PI = 3.141592653589793238462;

Camera::Camera(const int& maxRadius) :
    m_point(0, maxRadius, 0), m_angleX(0), m_angleY(0), m_angleZ(0), m_EZ(-1) {}

Camera::Camera(const Point3D& point, const double& angleX_, const double& angleY_, const double& angleZ_, const double& EZ_) :
    m_point(point), m_angleX(angleX_), m_angleY(angleY_), m_angleZ(angleZ_), m_EZ(EZ_) {}

void Camera::movePosition(const Point3D& newPosition) {
    m_point = newPosition;
}

void Camera::moveAngle(const double& angleX, const double& angleY, const double& angleZ) {
    m_angleX = fmod(angleX, 2 * PI);
    m_angleY = fmod(angleY, 2 * PI);
    m_angleZ = fmod(angleZ, 2 * PI);
}

void Camera::modifyAngles(const double& angleX, const double& angleY, const double& angleZ) {
    moveAngle(m_angleX + angleX, m_angleY + angleY, m_angleZ + angleZ);
}

Point3D Camera::position() const {
    return m_point;
}

double Camera::angleX() const {
    return m_angleX;
}

double Camera::angleY() const {
    return m_angleY;
}

double Camera::angleZ() const {
    return m_angleZ;
}

double Camera::EZ() const {
    return m_EZ;
}
