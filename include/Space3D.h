#ifndef SPACE3D_H
#define SPACE3D_H

#include "MyVector.h"
#include "ShapeData.h"
#include <graphics.h>
#include "MyArray.h"
#include <math.h>
#include "Camera.h"
#include "AppTools.h"

class Menu;

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
        bool getKeyCommand(const char& c);
        bool fscan(FILE* fp);
        void fprint(FILE* fp);
        MyArray<char, 512> getLinkedFileName();
        void setLinkedFileName(const MyArray<char, 512>& name);
        bool isLinkedWithFile();
        void setCorners(const int& x0_, const int& y0_, const int& x1_, const int& y1_);
        Menu* menuHolder;

    private:
        int x0;
        int y0;
        int x1;
        int y1;
        int m_theme;
        int m_selected;
        bool m_spinballSelected;
        bool m_fadedDrag;
        MyVector<Mesh> m_meshes;
        Mesh m_draggedMesh;
        MyVector<Section> m_sections;
        Section m_draggedSection;
        MyVector<bool> m_updated;
        Camera m_cam;
        CircularButton m_buttonOX;
        CircularButton m_buttonOY;
        CircularButton m_buttonOZ;
        DonutButton m_donutOX;
        DonutButton m_donutOY;
        DonutButton m_donutOZ;
        Button m_spinballButton;
        Button m_arrowLeft;
        Button m_arrowRight;
        Button m_arrowUp;
        Button m_arrowDown;
        Button m_arrowSpinLeft;
        Button m_arrowSpinRight;
        MyArray<char, 512> m_linkedFile;

        size_t size() const;
        void swapPages();
        void setButtons();
        void drawRotationArrows();
        void dragMesh();
        void dragAndDrop(const int& xDrag, const int& yDrag, Mesh& mesh);
        double findRotation(const int& xDrag, const int& yDrag, const DonutButton& angleDonut, CircularButton& button);
        bool checkAxisRotation(const int& x, const int& y);
        bool checkCamMovement(const char& c);
        Point2D moveInsideWorkArea(const Point2D& P, const Point2D& Q, const int& xBorder, const int& yBorder);
        void drawSpinball();
        void showAngleOptions();
        Point2D projectPoint(const Point3D& pct) const;
        Point3D normalisePoint(const Point3D& pct) const;
        Section projectSection(const Mesh& mesh);
        void render();
        void rotateOX(Mesh& lines, const double& alpha);
        void rotateOY(Mesh& lines, const double& alpha);
        void rotateOZ(Mesh& lines, const double& alpha);
        void selectMesh(const size_t& index);
        void highlightMesh();
        bool isDragAndDrop(const int& xDrag, const int& yDrag) const;
};

#endif // SPACE3D_H
