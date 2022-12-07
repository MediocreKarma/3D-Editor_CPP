#ifndef SPACE3D_H
#define SPACE3D_H

#include "MyVector.h"
#include "ShapeData.h"
#include <graphics.h>
#include "MyArray.h"
#include <math.h>
#include "Camera.h"

class Space3D
{
    public:
        Space3D(const double& maxRadius = 2500, const int& theme = 0);
        Space3D(MyVector<Mesh>& meshes, const double& maxRadius = 2500);
        void run();
        void addMesh(const Mesh& mesh);
        void draw();
        bool insideWorkArea(const int& x, const int& y) const;
        bool insideWorkArea(const Point2D& point) const;
        bool getCommand(const int& xClick, const int& yClick);
        bool fscan(FILE* fp);
        void fprint(FILE* fp);
        MyArray<char, 512> getLinkedFileName();
        void setLinkedFileName(const MyArray<char, 512>& name);
        bool isLinkedWithFile();
        void setCorners(const int& x0_, const int& y0_, const int& x1_, const int& y1_);

    private:
        int x0;
        int y0;
        int x1;
        int y1;
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
        MyArray<char, 512> m_linkedFile;

        size_t size() const;
        void getDrag(int& xDrag, int& yDrag) const;
        void dragAndDrop(const int& xDrag, const int& yDrag);
        double findRotation(const int& xDrag, const int& yDrag, const DonutButton& angleDonut, CircularButton& button);
        bool checkAxisRotation(const int& x, const int& y);
        Point2D moveInsideWorkArea(const Point2D& P, const Point2D& Q, const int& xBorder, const int& yBorder);
        void drawSpinball(const int& x, const int& y);
        void showAngleOptions(const int& x, const int& y);
        Point2D projectPoint(const Point3D& pct) const;
        Point3D normalisePoint(const Point3D& pct) const;
        Section projectSection(const Mesh& mesh);
        void render();
        void rotateOX(Mesh& lines, const double& alpha);
        void rotateOY(Mesh& lines, const double& alpha);
        void rotateOZ(Mesh& lines, const double& alpha);
        void selectMesh(const size_t& index);
        void highlightMesh();
        bool isDragAndDrop(int& xDrag, int& yDrag) const;
};

#endif // SPACE3D_H
