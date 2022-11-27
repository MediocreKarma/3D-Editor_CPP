#ifndef SPACE3D_H
#define SPACE3D_H

#include "MyVector.h"
#include "ShapeData.h"
#include <graphics.h>
#include "MyArray.h"
#include <math.h>

class Space3D
{
    public:
        Space3D();
        Space3D(MyVector<Mesh>& meshes, const double& maxRadius);
        void draw3D(const int& xMij, const int& yMij, const int& xLung, const int& yLung);

    protected:

    private:
        double m_maxRadius;
        MyVector<Mesh> m_meshes;

        void rotateOX(Mesh& lines, const double& alpha);
        void rotateOY(Mesh& lines, const double& alpha);
        void rotateOZ(Mesh& lines, const double& alpha);

        Point2D projectPoint(const Point3D& pct, const int& xMij, const int& yMij, const int& xLung, const int& yLung) const;
        MyVector<Line2D> project3D(const int& xMij, const int& yMij, const int& xLung, const int& yLung); //linie3d => linie2d
};

#endif // SPACE3D_H
