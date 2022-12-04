#ifndef CAMERA_H
#define CAMERA_H
#include "ShapeData.h"
#include <cstdio>

class Camera {
    public:
        Camera(const int& maxRadius = 5000);
        Camera(const Point3D& point, const double& angleX, const double& angleY, const double& angleZ, const double& EZ = 1);

        Point3D position() const;
        double angleX() const;
        double angleY() const;
        double angleZ() const;
        double EZ() const;
        void modifyAngles(const double& angleX, const double& angleY, const double& angleZ);
        void moveAngle(const double& angleX, const double& angleY, const double& angleZ);
        void movePosition(const Point3D& newPosition);
        bool fscan(FILE* fp);
        void fprint(FILE* fp);

    private:
        Point3D m_point;
        double m_angleX, m_angleY, m_angleZ;
        double m_EZ;
};
#endif // CAMERA_H
