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
        void changeHeight(const int& height);
        void addPoint(const Point2D& point);
        void removePoint(const Point2D& point);
        void removePoint(const size_t& index);
        void update(const size_t& index);
        MyVector<CircularButton> renderButtons(const int& x0, const int& y0, const int& x1, const int& y1);

    private:
        MyVector<Point2D> m_points;
        MyVector<bool> m_updated;
        int m_height;

};

class ObjectCreator {
    public:
        enum class Tool {
            NewPoint = 0, MovePoint, ConnectPoint, DeletePoint
        };
        ObjectCreator(const int& theme = 0);
        ObjectCreator(Mesh editedMesh, const int& theme = 0);
        ObjectCreator(const ObjectCreator& other);
        ObjectCreator& operator = (const ObjectCreator& other);
        Mesh run();
        void draw(bool update = true);

    private:
        int m_theme;
        MyVector<Layer> m_layers;
        MyVector<TextButton> m_layerSelectButtons;
        MyVector<MyVector<CircularButton>> m_layerPointButtons;
        MyVector<MyVector<size_t>> m_layerAdjListIndex;
        Button m_addLayerButton;
        MyVector<CircularButton> m_pointButtons;
        Button m_minimizedSpaceButton;
        MyArray<ImageButton, 4> m_toolButtons;
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

        void init();
        void resetLine();
        int getLayerByHeight(const int& height);
        void pointMover(const size_t& index);
        void pointConnector2D(const size_t& index);
        void pointConnector3D(const size_t& index);
        void pointDeleter(const size_t& index);
        void pointDeleter3D(const size_t& index);
        void pointAdder(const int& x, const int& y);
        void deleteLines2D(const int& x, const int& y);
        void deleteLines3D(const int& x, const int& y);
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
        void editLayer(const int& index);
};

#endif // OBJECTCREATOR_H
