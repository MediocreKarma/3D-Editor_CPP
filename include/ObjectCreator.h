#ifndef OBJECTCREATOR_H
#define OBJECTCREATOR_H

#include "ShapeData.h"
#include "MyVector.h"
#include "MyArray.h"
#include "Camera.h"
#include "Space3D.h"
#include "AppTools.h"
#include "TextInput.h"
#include "MyMap.h"
#include "MyHashMap.h"

class ObjectCreator {
    public:
        enum class Tool {
            NewPoint = 0, MovePoint, EditPoint, DeletePoint, ConnectPoint, CutLine
        };
        ObjectCreator(const int& theme = 0, int language = 0);
        ObjectCreator(const Mesh& editedMesh, const int& theme = 0,
                      int language = 0);
        ObjectCreator(const ObjectCreator& other);
        ObjectCreator& operator = (const ObjectCreator& other);
        Mesh run();
        void draw();
        const int& getCloseFlag() const;

    private:
        int m_width;
        int m_height;
        int m_theme;
        int m_language;

        struct LayerInfo {
            MyHashMap<FixedMesh::iterator_type, CircularButton> data;
            Point2D centerPoint;

            LayerInfo() :
                data(), centerPoint() {}

            void updateButtons(const int xCenter, const int yCenter);
        };

        MyMap<int, LayerInfo> m_layers;

        struct LayerSelectButtonData {
            TextButton txtButton;
            MyMap<int, LayerInfo>::iterator it;

            LayerSelectButtonData() :
                txtButton(), it() {}
        };

        MyVector<LayerSelectButtonData> m_layerSelectButtons;
        MyMap<int, LayerInfo>::iterator m_selectedLayer;
        Button m_addLayerButton;
        Button m_deleteLayerButton;
        Button m_dupeLayerButton;
        Button m_minimizedSpaceButton;
        MyArray<Button, 6> m_toolButtons;
        Tool m_tool;
        int workX0;
        int workY0;
        int workX1;
        int workY1;
        FixedSpace3D m_workArea;
        FixedMesh::iterator_type m_hovered;
        Line2D m_assistLine;
        bool m_assistLineDotted;
        MyArray<Button, 2> m_layerScrollArrows;
        MyArray<ImageButton, 1> m_layerTools; //yet to be used
        int m_closeFlag; //close w discard vs close w save
        TextButton m_discardButton, m_saveButton;
        DropdownButton<4> m_generateButton;
        Button m_pointDataButton;

        void init();
        void resetLine();
        int getLayerByHeight(const int& height);
        void drawToolButtonSymbol(const int xCenter, const int yCenter, const int btnWidth, const int toolIndex);
        void layerViewMover(int x, int y);
        void pointMover();
        void pointConnector2D();
        void pointConnector3D();
        void pointDeleter();
        void pointDeleter3D();
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
        bool changePointData(const int x, const int y);
        bool getClickCommand(const int x, const int y);
        bool getClickCommand();
        bool getHoverCommand();
        bool getHoverCommand(const int& x, const int& y);
        bool getDoubleClickCommand();
        void toolOperationOnPoint();
        void renderLayerSelectButtons();
        void updateLayerSelectsInterval();
        bool moveLayerSelectsInterval(const int dir);
        void centerLayerButton();
        void addLayer();
        void deleteLayer();
        void dupeLayer();
        void mergeLayers(const int movingIndex, MyMap<int, LayerInfo>::iterator destination);
        void editLayer(const int layerIndex);
        Mesh generateCube(const unsigned int& length_);
        Mesh generateCylinder(const unsigned int& height, const unsigned int& radius, const unsigned int& sides);
        Mesh generateCone(const unsigned int& height, const unsigned int& radius, const unsigned int& sides);
        Mesh generateSphere(const unsigned int& radius, const unsigned int& segments, const unsigned int& rings);
        bool generate(int index);
};

#endif // OBJECTCREATOR_H
