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
        void getCommand(const int& x0, const int& y0, const int& x1, const int& y1);
        void dragAndDrop(const int& xDrag, const int& yDrag, const int& x0, const int& y0, const int& x1, const int& y1);
        void fprint(FILE* fp);
        bool fscan(FILE* fp);
        //void drawAxisButton(const int& x1, const int& y1, const int& x2, const int& y2, const int& thickness, const int& radius);

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
        MyArray<AxisButton,3> m_axisButtons;

        size_t size() const;
        void getDrag(int& xDrag, int& yDrag) const;
        double findRotation(const int& xDrag, const int& yDrag, const DonutButton& angleDonut, CircularButton& button);
        bool checkAxisRotation(const int& x, const int& y);
        void drawSpinball(const int& x, const int& y);
        void showAngleOptions(const int& x, const int& y);
        Point2D projectPoint(const Point3D& pct, const int& xCenter, const int& yCenter, const int& /*xLen*/, const int& yLen) const;
        Point3D normalisePoint(const Point3D& pct) const;
        Section projectSection(const Mesh& mesh, const int& xCenter, const int& yCenter, const int& xLen, const int& yLen);
        void render(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen);
        void rotateOX(Mesh& lines, const double& alpha);
        void rotateOY(Mesh& lines, const double& alpha);
        void rotateOZ(Mesh& lines, const double& alpha);
        void selectMesh(const size_t& index, const int& x0, const int& y0, const int& x1, const int& y1);
        void highlightMesh(const int& x0, const int& y0, const int& x1, const int& y1);
        bool isDragAndDrop(int& xDrag, int& yDrag, const int& x0, const int& y0, const int& x1, const int& y1) const;
        void setAxisButtons(const int& x0, const int& y0, const int& x1, const int& y1);
        void drawAxes();
        int getDraggedAxis(int& xDrag, int& yDrag, const int& x0, const int& y0, const int& x1, const int& y1) const;
        void moveMesh(const int axis, const int& xDrag, const int& yDrag, const int& xDrop, const int& yDrop, const int& x0, const int& y0, const int& x1, const int& y1);
};

#endif // SPACE3D_H
