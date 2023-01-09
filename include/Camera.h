#ifndef CAMERA_H
#define CAMERA_H
#include "ShapeData.h"
#include <cstdio>
#include "Quaternion.h"

class Camera {
    public:
        Camera(const int maxRadius = 5000);
        Camera(const Point3D& point, const double& EZ = 1);
        Camera(const Point3D& point, const Quaternion& quat);

        Point3D position() const;
        double angleX() const;
        double angleY() const;
        double angleZ() const;
        Quaternion quat() const;
        double EZ() const;
        void modifyAngles(const double& angleX, const double& angleY, const double& angleZ);
        void moveAngle(const double& angleX, const double& angleY, const double& angleZ);
        void rotateOnAxis(const size_t axis, const double& angle);
        void rotateByUnitQuat(const Quaternion& quat);
        void movePosition(const Point3D& newPosition);
        bool fscan(FILE* fp);
        void fprint(FILE* fp);
        void modifyPosition(const double& posX, const double& posY, const double& posZ);
        void setEZ(const double& value);

    private:
        Point3D m_point;
        double m_EZ;
        Quaternion m_quat;
};
#endif // CAMERA_H
