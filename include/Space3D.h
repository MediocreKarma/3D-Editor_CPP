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
        Space3D(const double& maxRadius = 2500, const int& theme = 0);
        Space3D(MyVector<Mesh>& meshes, const double& maxRadius = 2500);
        void run(const int& x0, const int& y0, const int& x1, const int& y1);
        void addMesh(const Mesh& mesh);
        void draw(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen);
        bool insideWorkArea(const int& x, const int& y, const int& xCenter, const int& yCenter, const int& xLen, const int& yLen) const;
        void getCommand(const int& x0, const int& y0, const int& x1, const int& y1);
        void dragAndDrop(const int& xDrag, const int& yDrag, const int& x0, const int& y0, const int& x1, const int& y1);

    private:
        double m_maxRadius;
        int m_theme;
        int m_selected;
        MyVector<Mesh> m_meshes;
        MyVector<Section> m_sections;
        MyVector<bool> m_updated;
        Camera m_cam;

        size_t size() const;
        Point2D projectPoint(const Point3D& pct, const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const double& scale) const;
        Section projectSection(Mesh& mesh, const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const double& scale);
        void render(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const double& scale = 1);
        void rotateOX(Mesh& lines, const double& alpha);
        void rotateOY(Mesh& lines, const double& alpha);
        void rotateOZ(Mesh& lines, const double& alpha);
        void selectMesh(const size_t& index);
        void highlightMesh();
        bool isDragAndDrop(int& xDrag, int& yDrag) const;

};

#endif // SPACE3D_H
