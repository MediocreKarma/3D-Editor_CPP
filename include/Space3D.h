#ifndef SPACE3D_H
#define SPACE3D_H

#include "MyVector.h"
#include "ShapeData.h"
#include <graphics.h>
#include "MyArray.h"
#include <math.h>
#include "Camera.h"
#include "AppTools.h"
#include "Quaternion.h"

class Menu;

class ObjectCreator;

class Space3D
{
    public:
        Space3D();
        Space3D(const double& maxRadius, const int& theme, Menu* menuHolder);
        Space3D(const double& maxRadius, const int& theme, ObjectCreator* objCreatorHolder);
        Space3D(const Space3D& other);
        Space3D& operator = (const Space3D& other);
        void addMesh(const Mesh& mesh);
        void run();
        void draw();
        void render();
        bool insideWorkArea(const int& x, const int& y) const;
        bool insideWorkArea(const Point2D& point) const;
        bool getCommand(const int& xClick, const int& yClick);
        bool getKeyCommand();
        bool fscan(FILE* fp);
        void fprint(FILE* fp);
        MyArray<char, 512> getLinkedFileName();
        void setLinkedFileName(const MyArray<char, 512>& name);
        bool isLinkedWithFile();
        void setCorners(const int& x0_, const int& y0_, const int& x1_, const int& y1_);
        Mesh& meshAt(const size_t& index);
        const Mesh& meshAt(const size_t& index) const;
        Section& sectionAt(const size_t& index);
        const Section& sectionAt(const size_t& index) const;
        void update();
        void setTheme(const int& theme);

    private:
        int x0;
        int y0;
        int x1;
        int y1;
        int m_theme;
        int m_selected;
        bool m_spinballSelected;
        bool m_fadedDrag;
        unsigned int m_objRotateDrag;
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
        Menu* m_menuHolder;
        ObjectCreator* m_objCreatorHolder;
        size_t size() const;
        void callHandlerDrawer();
        void setButtons();
        void drawRotationArrows();
        void dragMesh();
        void dragAndDrop(const int& xDrag, const int& yDrag, Mesh& mesh,  const Quaternion& camQuat, const Quaternion& camInverse);
        double findRotation(const int& xDrag, const int& yDrag, const DonutButton& angleDonut, CircularButton& button);
        bool checkAxisRotation(const int& x, const int& y);
        bool checkCamMovement(const char& c);
        bool checkObjectRotation(int x, int y);
        Point2D moveInsideWorkArea(const Point2D& P, const Point2D& Q, const int& xBorder, const int& yBorder);
        void drawSpinball();
        void showAngleOptions();
        void drawAngleButtons();
        Point2D projectPoint(const Point3D& pct, const Quaternion& camQuat, const Quaternion& camInverse) const;
        Point3D rotateByCamera(const Point3D& pct) const;
        Point3D normalisePoint(const Point3D& pct,  const Quaternion& camQuat, const Quaternion& camInverse) const;
        Section projectSection(const Mesh& mesh);
        void rotateOX(Mesh& lines, const double& alpha);
        void rotateOY(Mesh& lines, const double& alpha);
        void rotateOZ(Mesh& lines, const double& alpha);
        void selectMesh(const size_t& index);
        void highlightMesh();
        bool isDragAndDrop(const int& xDrag, const int& yDrag) const;
};

#endif // SPACE3D_H
