#ifndef OBJECTCREATOR_H
#define OBJECTCREATOR_H

#include "ShapeData.h"
#include "MyVector.h"
#include "MyArray.h"
#include "Camera.h"
#include "Space3D.h"
#include "AppTools.h"
#include "TextInput.h"

class Layer {
    public:
        Layer();
        Layer(const Layer& other);
        Layer& operator = (const Layer& other);
        Point2D& operator [] (const size_t& index);
        int height() const;
        bool empty() const;
        size_t size() const;
        Point2D center() const;
        void changeHeight(const int& height);
        void addPoint(const Point2D& point);
        void removePoint(const Point2D& point);
        void removePoint(const size_t& index);
        void update();
        void update(const size_t& index);
        MyVector<CircularButton> renderButtons(const int& x0, const int& y0, const int& x1, const int& y1);
        void renderButtons(const int& x0, const int& y0, const int& x1, const int& y1, MyVector<CircularButton>& existingButtons);
        void moveRel(const int& x, const int& y);
        void moveRel(const Point2D& rel);

    private:
        MyVector<Point2D> m_points;
        MyVector<bool> m_updated;
        int m_height;
        Point2D m_center;
};

class ObjectCreator {
    public:
        enum class Tool {
            NewPoint = 0, MovePoint, ConnectPoint, DeletePoint, CutLine
        };
        ObjectCreator(const int& theme = 0);
        ObjectCreator(Mesh editedMesh, const int& theme = 0);
        ObjectCreator(const ObjectCreator& other);
        ObjectCreator& operator = (const ObjectCreator& other);
        Mesh run();
        void draw(bool update = true);
        const int& getCloseFlag() const;

    private:
        int m_theme;
        MyVector<Layer> m_layers;
        MyVector<TextButton> m_layerSelectButtons;
        MyVector<MyVector<CircularButton>> m_layerPointButtons;
        MyVector<MyVector<size_t>> m_layerAdjListIndex;
        Button m_addLayerButton;
        MyVector<CircularButton> m_pointButtons;
        Button m_minimizedSpaceButton;
        MyArray<ImageButton, 5> m_toolButtons;
        Tool m_tool;
        int x0;
        int y0;
        int x1;
        int y1;
        int workX0;
        int workY0;
        int workX1;
        int workY1;
        Space3D m_workArea;
        Mesh* m_workMesh;
        int m_selectedLayer;
        int m_hovered;
        Line2D m_assistLine;
        bool m_assistLineDotted;
        int m_layerSelectsBegin, m_layerSelectsEnd;
        MyArray<Button, 2> m_layerScrollArrows;
        MyArray<ImageButton, 1> m_layerTools; //yet to be used
        int m_closeFlag; //close w discard vs close w save
        TextButton m_discardButton, m_saveButton;

        void init();
        void resetLine();
        int getLayerByHeight(const int& height);
        void layerViewMover(const int& x_, const int& y_);
        void pointMover(const size_t& index);
        void pointConnector2D(const size_t& index);
        void pointConnector3D(const size_t& index);
        void pointDeleter(const size_t& index);
        void pointDeleter3D(const size_t& index);
        void pointAdder(const int& x, const int& y);
        void lineCutter2D(const int& x, const int& y);
        void lineCutter3D(const int& x, const int& y);
        void drawSelectLayers();
        void updateButtons();
        void drawButtons();
        void drawLayerView();
        void toolButtonsInit();
        void drawToolButtons();
        void drawPointData();
        MyArray<char, 32> itoa(int x, const char* prefix = nullptr);
        int atoi(MyArray<char, 256>& arr);
        bool getClickCommand();
        bool getHoverCommand();
        bool getHoverCommand(const int& x, const int& y);
        bool getDoubleClickCommand();
        void toolOperationOnPoint(const size_t& index);
        void renderLayerSelectButtons();
        void updateLayerSelectsInterval();
        bool moveLayerSelectsInterval(const int& delta);
        void editLayer(const int& index);
        Mesh generateCube(const unsigned int& length_);
        Mesh generateCylinder(const unsigned int& height, const unsigned int& radius, const unsigned int& sides);
        Mesh generateCone(const unsigned int& height, const unsigned int& radius, const unsigned int& sides);
        Mesh generateSphere(const unsigned int& radius, const unsigned int& segments, const unsigned int& rings);
};

#endif // OBJECTCREATOR_H
