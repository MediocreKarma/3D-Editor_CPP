#ifndef OBJECTCREATOR_H
#define OBJECTCREATOR_H

#include "ShapeData.h"
#include "MyVector.h"
#include "MyArray.h"
#include "Camera.h"
#include "Space3D.h"
#include "AppTools.h"

class Layer {
    public:
        enum class Tool {
            MovePoint, NewPoint, ConnectPoint, DeletePoint
        };

        Layer(MyVector<MyVector<size_t>>* m_adjListPointer);
        Layer(const Layer& other);
        Layer& operator = (const Layer& other);
        Point2D& operator [] (const size_t& index);
        int height() const;
        size_t size() const;
        void changeHeight(const int& height);
        void addPoint(const Point2D& point, const size_t& adjListIndex);
        void removePoint(const Point2D& point);
        void removePoint(const size_t& index);
        MyVector<CircularButton> renderButtons(const int& x0, const int& y0, const int& x1, const int& y1);

    private:
        MyVector<Point2D> m_points;
        MyVector<CircularButton> m_pointButtons;
        MyVector<MyVector<size_t>>* m_adjListPointer;
        MyVector<size_t> m_indices;
        MyVector<size_t> m_updated;
        int m_height;

};

class ObjectCreator {
    public:
        ObjectCreator(const int& theme = 0);
        ObjectCreator(Mesh editedMesh, const int& theme = 0);
        Mesh run();
        void draw();

    private:
        int m_theme;
        MyVector<Layer> m_layers;
        MyVector<TextButton> m_layerSelectButtons;
        MyVector<MyVector<CircularButton>> m_layerPointButtons;
        Button m_addLayerButton;
        MyVector<CircularButton> m_pointButtons;
        MyVector<MyVector<size_t>> m_adjList;
        int x0;
        int y0;
        int x1;
        int y1;
        Space3D m_workArea;
        int m_selectedLayer;

        void drawSelectLayers();
        void updateButtons();
        void drawButtons() ;
        MyArray<char, 24> itoa(int x);
        bool getCommand(const int& x, const int& y);
};

#endif // OBJECTCREATOR_H
