#include "ObjectCreator.h"
#include <iostream>

Layer::Layer() :
    m_points(), m_updated(), m_height() {}

Layer::Layer(const Layer& other) :
    m_points(other.m_points), m_updated(other.m_updated), m_height(other.m_height) {}

Layer& Layer::operator = (const Layer& other) {
    m_points = other.m_points;
    m_height = other.m_height;
    m_updated = other.m_updated;
    return *this;
}

Point2D& Layer::operator [] (const size_t& index) {
    return m_points[index];
}

int Layer::height() const {
    return m_height;
}

size_t Layer::size() const {
    return m_points.size();
}

void Layer::changeHeight(const int& height_) {
    m_height = height_;
}

void Layer::addPoint(const Point2D& point) {
    m_points.push_back(point);
    m_updated.push_back(true);
}

void Layer::removePoint(const size_t& index) {
    for (size_t i = index + 1; i < size(); ++i) {
        m_points[i - 1] = m_points[i];
        m_updated[i - 1] = m_updated[i];
    }
    m_points.pop_back();
    m_updated.pop_back();
}

MyVector<CircularButton> Layer::renderButtons(const int& x0, const int& y0, const int& x1, const int& y1) {
    const int xCenter = (x0 + x1) / 2;
    const int yCenter = (y0 + y1) / 2;
    MyVector<CircularButton> pointButtons(m_points.size());
    for (size_t i = 0; i < m_points.size(); ++i) {
        if (m_updated[i]) {
            pointButtons[i] = CircularButton(xCenter + m_points[i].getX(), yCenter - m_points[i].getY(), 5);
            m_updated[i] = false;
        }
    }
    return pointButtons;
}

void Layer::update(const size_t& index) {
    m_updated[index] = true;
}

ObjectCreator::ObjectCreator(const int& theme) :
    m_theme(theme), m_layers(), m_layerSelectButtons(), m_layerPointButtons(), m_layerAdjListIndex(), m_addLayerButton(), m_pointButtons(), m_minimizedSpaceButton(), m_toolButtons(),
    m_tool(Tool::MovePoint), x0(0), y0(0), x1(1000), y1(800), m_workArea(-2500, theme, this), m_workMesh(nullptr), m_selectedLayer(-1) {
    toolButtonsInit();
    m_workArea.setCorners(26, 26, 800, 800);
    m_workArea.addMesh(Mesh());
    m_workMesh = &m_workArea.meshAt(0);
    m_minimizedSpaceButton = Button(900, 100, 200, 200);
    m_addLayerButton = Button(900, 200 + m_layers.size() * 40, 40, 40);
}

ObjectCreator::ObjectCreator(Mesh mesh, const int& theme) :
    m_theme(theme), m_layers(), m_layerSelectButtons(), m_layerPointButtons(), m_layerAdjListIndex(), m_addLayerButton(), m_pointButtons(), m_minimizedSpaceButton(), m_toolButtons(), m_tool(Tool::MovePoint),
    x0(0), y0(0), x1(1000), y1(800), m_workArea(-2500, theme, this), m_workMesh(nullptr), m_selectedLayer(-1) {
    toolButtonsInit();
    m_workArea.setCorners(26, 26, 800, 800);
    mesh.translate(-mesh.centerPoint().getX(), -mesh.centerPoint().getY(), -mesh.centerPoint().getZ());
    m_workArea.addMesh(mesh);
    m_workArea.render();
    m_workMesh = &m_workArea.meshAt(0);
    const Section& sect = m_workArea.sectionAt(0);
    m_pointButtons.resize(sect.size());
    updateButtons();
    for (size_t i = 0; i < sect.size(); ++i) {
        bool levelExists = false;
        for (size_t j = 0; j < m_layers.size(); ++j) {
            if (m_layers[j].height() == mesh[i].getZ()) {
                levelExists = true;
                m_layers[j].addPoint(Point2D(mesh[i].getX(), mesh[i].getY()));
                m_layerAdjListIndex[j].push_back(i);
                break;
            }
        }
        if (!levelExists) {
            m_layers.push_back(Layer());
            m_layers.back().changeHeight(mesh[i].getZ());
            m_layers.back().addPoint(Point2D(mesh[i].getX(), mesh[i].getY()));
            m_layerAdjListIndex.push_back(MyVector<size_t>());
            m_layerAdjListIndex.back().push_back(i);
        }
    }
    m_layerSelectButtons.resize(m_layers.size());
    m_layerSelectButtons.resize(m_layers.size());
    for (size_t i = 0; i < m_layers.size(); ++i) {
        MyArray<char, 32> layerLevel = "Layer Z: ";
        MyArray<char, 24> zText = itoa(m_layers[i].height());
        for (size_t j = 9; zText[j - 9]; ++j) {
            layerLevel[j] = zText[j - 9];
        }
        m_layerSelectButtons[i] = TextButton(900, 200 + 20 + i * 40, 200, 40, layerLevel.data());
        m_layerPointButtons[i] = m_layers[i].renderButtons(26, 26, 800, 800);
    }
    m_minimizedSpaceButton = Button(900, 100, 200, 200);
    m_addLayerButton = Button(900, 200 + 20 + m_layers.size() * 40, 40, 40);
}

void ObjectCreator::toolButtonsInit() {
    for (size_t i = 0; i < 4; ++i) {
        m_toolButtons[i] = Button(12, 36 + i * 24, 24, 24);
    }
}

MyArray<char, 24> ObjectCreator::itoa(int x) {
    MyArray<char, 24> text = {0};
    bool isNegative = false;
    if (x < 0) {
        isNegative = true;
        x = -x;
    }
    size_t i = 0;
    do {
        text[i++] = x % 10 + '0';
    } while (x /= 10);
    if (isNegative) {
        text[i++] = '-';
    }
    for (size_t j = 0; j < i / 2; ++j) {
        char aux = text[j];
        text[j] = text[i - j - 1];
        text[i - j - 1] = aux;
    }
    return text;
}

void ObjectCreator::drawToolButtons() {
    for (size_t i = 0; i < 4; ++i) {
        m_toolButtons[i].drawLabel(RED, ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR]);
    }
}

void ObjectCreator::drawSelectLayers() {
    for (size_t i = 0; i < m_layerSelectButtons.size(); ++i) {
        m_layerSelectButtons[i].drawTextButton(1, 3, RGB(160, 160, 160));
    }
    int x = m_addLayerButton.getXCenter();
    int y = m_addLayerButton.getYCenter();
    setcolor(ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR]);
    line(x - 15, y, x + 15, y);
    line(x, y - 15, x, y + 15);
}

void ObjectCreator::updateButtons() {
    const Section& sect = m_workArea.sectionAt(0);
    for (size_t i = 0; i < m_pointButtons.size(); ++i) {
        m_pointButtons[i] = CircularButton(sect[i].getX(), sect[i].getY(), 5);
    }
}

void ObjectCreator::drawButtons() {
    for (size_t i = 0; i < m_pointButtons.size(); ++i) {
        m_pointButtons[i].drawLabel(RED, RED);
    }
}

void ObjectCreator::drawLayerView() {
    for (size_t i = 0; i < m_layerPointButtons[m_selectedLayer].size(); ++i) {
        m_layerPointButtons[m_selectedLayer][i].drawLabel(RED, RED);
    }
    setcolor(ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR]);
    line(412, 26, 412, 800);
    line(26, 412, 800, 412);
}

void ObjectCreator::draw(bool update = false) {
    setfillstyle(SOLID_FILL, ColorSchemes::themeColors[m_theme][ColorSchemes::PRIMARYCOLOR]);
    bar(0, 0, x1, y1);
    drawToolButtons();
    m_workArea.render();
    if (m_selectedLayer == -1) {
        m_workArea.run();
        if (update) {
            updateButtons();
        }
        drawButtons();
    }
    else {
        drawLayerView();
    }
    drawSelectLayers();
    m_minimizedSpaceButton.drawLabel(LIGHTGRAY, RED);
    swapbuffers();
}

void ObjectCreator::pointMover(const int& index) {
    const int xCenter = (26 + 800) / 2;
    const int yCenter = (26 + 800) / 2;
    int adjListIndex = m_layerAdjListIndex[m_selectedLayer][index];
    int xDrag, yDrag;
    clearmouseclick(WM_LBUTTONUP);
    while (!ismouseclick(WM_LBUTTONUP)) {
        getmouseclick(WM_MOUSEMOVE, xDrag, yDrag);
        if (xDrag != -1) {
            m_layers[m_selectedLayer][index] = Point2D(xDrag - xCenter, yCenter - yDrag);
            m_layers[m_selectedLayer].update(index);
            m_layerPointButtons[m_selectedLayer][index] = CircularButton(xDrag, yDrag, 5);
            (*m_workMesh)[adjListIndex] = Point3D(xDrag - xCenter, yCenter - yDrag, m_layers[m_selectedLayer].height());
            m_workArea.update();
            draw(true);
        }
    }
}

void ObjectCreator::toolOperation(const int& index) {
    switch (m_tool) {
        case Tool::MovePoint:
            pointMover(index);
        case Tool::ConnectPoint:
        case Tool::DeletePoint:
        case Tool::NewPoint:
        default:;
    }
}

bool ObjectCreator::getCommand(const int& x, const int& y) {
    if (m_workArea.getKeyCommand()) {
        return true;
    }
    for (size_t i = 0; i < m_toolButtons.size(); ++i) {
        if (m_toolButtons[i].hitCollision(x, y)) {
            m_tool = (Tool)i;
        }
    }
    for (size_t i = 0; i < m_layerSelectButtons.size(); ++i) {
        if (m_layerSelectButtons[i].hitCollision(x, y)) {
            m_selectedLayer = i;
            return true;
        }
    }
    if (m_selectedLayer != -1) {
        if (m_minimizedSpaceButton.hitCollision(x, y)) {
            m_selectedLayer = -1;
            return true;
        }
        for (size_t i = 0; i < m_layerPointButtons[m_selectedLayer].size(); ++i) {
            if (m_layerPointButtons[m_selectedLayer][i].hitCollision(x, y)) {
                toolOperation(i);
                return true;
            }
        }
    }
    else if (m_workArea.getCommand(x, y)) {
        return true;
    }
    return false;
}

Mesh ObjectCreator::run() {
    initwindow(x1, y1, "Object creator", 300, 0);
    setvisualpage(0);
    setactivepage(1);
    draw();
    while (true) {
        int x, y;
        getmouseclick(WM_LBUTTONDOWN, x, y);
        if (getCommand(x, y)) {
            draw();
        }
    }
    return Mesh();
}
