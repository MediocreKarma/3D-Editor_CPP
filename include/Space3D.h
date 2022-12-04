#ifndef SPACE3D_H
#define SPACE3D_H

#include "MyVector.h"
#include "ShapeData.h"
#include <graphics.h>
#include "MyArray.h"
#include <math.h>
#include "Camera.h"
#include<math.h>
#include "SettingsMenuInterface.h"

class Space3D
{
    public:
        Space3D(const double& maxRadius = 2500, const int& theme = 0);
        Space3D(MyVector<Mesh>& meshes, const double& maxRadius = 2500);
        void run(const int& x0, const int& y0, const int& x1, const int& y1);
        void addMesh(const Mesh& mesh);
        void draw(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen);
        bool insideWorkArea(const int& x, const int& y, const int& xCenter, const int& yCenter, const int& xLen, const int& yLen) const;
        bool insideWorkArea(const Point2D& point, const int& x0, const int& y0, const int& x1, const int& y1) const;
        void getCommand(const int& xClick, const int& yClick, const int& x0, const int& y0, const int& x1, const int& y1);
        void dragAndDrop(const int& xDrag, const int& yDrag, const int& x0, const int& y0, const int& x1, const int& y1);
        bool fscan(FILE* fp);
        void fprint(FILE* fp);

    private:
        int m_theme;
        int m_selected;
        bool m_spinballSelected;
        MyVector<Mesh> m_meshes;
        MyVector<Section> m_sections;
        MyVector<bool> m_updated;
        Camera m_cam;
        CircularButton m_buttonOX;
        CircularButton m_buttonOY;
        CircularButton m_buttonOZ;
        DonutButton m_donutOX;
        DonutButton m_donutOY;
        DonutButton m_donutOZ;
        Button m_spinballButton;

        size_t size() const;
        void getDrag(int& xDrag, int& yDrag) const;
        double findRotation(const int& xDrag, const int& yDrag, const DonutButton& angleDonut, CircularButton& button);
        bool checkAxisRotation(const int& x, const int& y);
        Point2D moveInsideWorkArea(const Point2D& P, const Point2D& Q, const int& xBorder, const int& yBorder);
        void drawSpinball(const int& x, const int& y);
        void showAngleOptions(const int& x, const int& y);
        Point2D projectPoint(const Point3D& pct, const int& xCenter, const int& yCenter, const int& /*xLen*/, const int& yLen) const;
        Point3D normalisePoint(const Point3D& pct) const;
        Section projectSection(const Mesh& mesh, const int& xCenter, const int& yCenter, const int& xLen, const int& yLen);
        void render(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen);
        void rotateOX(Mesh& lines, const double& alpha);
        void rotateOY(Mesh& lines, const double& alpha);
        void rotateOZ(Mesh& lines, const double& alpha);
        void selectMesh(const size_t& index);
        void highlightMesh();
        bool isDragAndDrop(int& xDrag, int& yDrag, const int& x0, const int& y0, const int& x1, const int& y1) const;
};

#endif // SPACE3D_H
