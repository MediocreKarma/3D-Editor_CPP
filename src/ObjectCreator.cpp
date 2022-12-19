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

bool Layer::empty() const {
    return !m_points.size();
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
    m_points.erase(m_points.begin() + index);
    m_updated.erase(m_updated.begin() + index);
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
    m_tool(Tool::ConnectPoint), x0(0), y0(0), x1(1000), y1(800), workX0(26), workY0(26), workX1(800), workY1(800), m_workArea(-2500, theme, this), m_workMesh(nullptr), m_selectedLayer(-1),
    m_hovered(-1), m_assistLine(-1, -1, -1, -1) {
    toolButtonsInit();
    m_workArea.setCorners(26, 26, 800, 800);
    m_workArea.addMesh(Mesh());
    m_workMesh = &m_workArea.meshAt(0);
    m_minimizedSpaceButton = Button(900, 100, 200, 200);
    m_addLayerButton = Button(900, 200 + m_layers.size() * 40, 40, 40);
}

ObjectCreator::ObjectCreator(Mesh mesh, const int& theme) :
    m_theme(theme), m_layers(), m_layerSelectButtons(), m_layerPointButtons(), m_layerAdjListIndex(), m_addLayerButton(), m_pointButtons(), m_minimizedSpaceButton(), m_toolButtons(), m_tool(Tool::MovePoint),
    x0(0), y0(0), x1(1000), y1(800), workX0(26), workY0(26), workX1(800), workY1(800), m_workArea(-2500, theme, this), m_workMesh(nullptr), m_selectedLayer(-1), m_hovered(-1), m_assistLine(-1, -1, -1, -1) {
    toolButtonsInit();
    const Point3D& centerPoint = mesh.centerPoint();
    mesh.translate(-centerPoint.getX(), -centerPoint.getY(), -centerPoint.getZ());
    m_workArea.setCorners(x0 + 26, y0 + 26, x1 - 200, y1);
    m_workArea.addMesh(mesh);
    m_workArea.render();
    init();
}

ObjectCreator::ObjectCreator(const ObjectCreator& oc) :
    m_theme(oc.m_theme), m_layers(oc.m_layers), m_layerSelectButtons(oc.m_layerSelectButtons), m_layerPointButtons(oc.m_layerPointButtons), m_layerAdjListIndex(oc.m_layerAdjListIndex),
    m_addLayerButton(oc.m_addLayerButton), m_pointButtons(oc.m_pointButtons), m_minimizedSpaceButton(oc.m_minimizedSpaceButton), m_toolButtons(oc.m_toolButtons), m_tool(oc.m_tool),
    x0(oc.x0), y0(oc.y0), x1(oc.x1), y1(oc.y1), workX0(oc.workX0), workY0(oc.workY0), workX1(oc.workX1), workY1(oc.workY1), m_workArea(oc.m_workArea), m_workMesh(oc.m_workMesh),
    m_selectedLayer(oc.m_selectedLayer), m_hovered(oc.m_hovered), m_assistLine(oc.m_assistLine) {}

ObjectCreator& ObjectCreator::operator = (const ObjectCreator& oc) {
    m_theme = oc.m_theme;
    x0 = oc.x0, y0 = oc.y0, x1 = oc.x1, y1 = oc.y1;
    workX0 = oc.workX0, workY0 = oc.workY0, workX1 = oc.workX1, workY1 = oc.workY1;
    toolButtonsInit();
    m_workArea.setCorners(workX0, workY0, workX1, workY1);
    Mesh mesh = oc.m_workArea.meshAt(0);
    m_workArea.addMesh(mesh);
    m_assistLine = oc.m_assistLine;
    m_hovered = oc.m_hovered;
    m_workArea.render();
    init();
    return *this;
}

void ObjectCreator::toolButtonsInit() {
    for (size_t i = 0; i < 4; ++i) {
        m_toolButtons[i] = Button(12, 36 + i * 24, 24, 24);
    }
}

void ObjectCreator::init() {
    m_workMesh = &m_workArea.meshAt(0);
    const Section& sect = m_workArea.sectionAt(0);
    m_pointButtons.resize(sect.size());
    updateButtons();
    for (size_t i = 0; i < sect.size(); ++i) {
        bool levelExists = false;
        for (size_t j = 0; j < m_layers.size(); ++j) {
            const Point3D& pnt = (*m_workMesh)[i];
            if (m_layers[j].height() == (int)round(pnt.getZ())) {
                levelExists = true;
                m_layers[j].addPoint(Point2D(pnt.getX(), pnt.getY()));
                m_layerAdjListIndex[j].push_back(i);
                break;
            }
        }
        if (!levelExists) {
            m_layers.push_back(Layer());
            const Point3D& pnt = (*m_workMesh)[i];
            m_layers.back().changeHeight(round(pnt.getZ()));
            m_layers.back().addPoint(Point2D(pnt.getX(), pnt.getY()));
            m_layerAdjListIndex.push_back(MyVector<size_t>());
            m_layerAdjListIndex.back().push_back(i);
        }
    }
    bool sorted = true;
    do {
        sorted = true;
        for (size_t i = 1; i < m_layers.size(); ++i) {
            if (m_layers[i - 1].height() > m_layers[i].height()) {
                std::swap(m_layers[i - 1], m_layers[i]);
                std::swap(m_layerAdjListIndex[i - 1], m_layerAdjListIndex[i]);
                sorted = false;
            }
        }
    } while (!sorted);
    m_layerSelectButtons.resize(m_layers.size());
    m_layerPointButtons.resize(m_layers.size());
    for (size_t i = 0; i < m_layers.size(); ++i) {
        MyArray<char, 32> layerLevel = itoa(m_layers[i].height(), "Layer Z: ");
        m_layerSelectButtons[i] = TextButton(x1 - 100, y0 + 200 + 20 + i * 40, 200, 40, layerLevel.data());
        m_layerPointButtons[i] = m_layers[i].renderButtons(x0 + 26, y0 + 26, x1 - 200, y1);
    }
    m_minimizedSpaceButton = Button(x1 - 100, y0 + 100, 200, 200);
    m_addLayerButton = Button(x1 - 100, y0 + 200 + 20 + m_layers.size() * 40, 40, 40);
}

MyArray<char, 32> ObjectCreator::itoa(int x, const char* prefix) {
    MyArray<char, 32> text = prefix;
    const size_t pLen = strlen(prefix);
    bool isNegative = false;
    if (x < 0) {
        isNegative = true;
        x = -x;
    }
    size_t i = 0;
    do {
        text[pLen + i++] = x % 10 + '0';
    } while (x /= 10);
    if (isNegative) {
        text[pLen + i++] = '-';
    }
    for (size_t j = pLen; j < pLen + i / 2; ++j) {
        char aux = text[j];
        text[j] = text[2 * pLen + i - j - 1];
        text[2 * pLen + i - j - 1] = aux;
    }
    return text;
}

void ObjectCreator::resetLine() {
    m_assistLine = Line2D(0, 0, 0, 0);
}

void ObjectCreator::drawToolButtons() {
    for (size_t i = 0; i < m_toolButtons.size(); ++i) {
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

void ObjectCreator::drawLayerView() { // O(layer.size()^2 * adjList.size())   D:<
    const MyVector<MyVector<size_t>>& adjList = m_workMesh->adjacencyList();
    const int workXCenter = (workX0 + workX1) / 2;
    const int workYCenter = (workY0 + workY1) / 2;
    //draw lines
    for (size_t i = 0; i < m_layers[m_selectedLayer].size(); ++i) {
        size_t iReal = m_layerAdjListIndex[m_selectedLayer][i];
        for (size_t j = i + 1; j < m_layers[m_selectedLayer].size(); ++j) {
            size_t jReal = m_layerAdjListIndex[m_selectedLayer][j];
            for (const size_t& nodIndex : adjList[iReal]) {
                if (nodIndex == jReal) {
                    const Point2D& P = m_layers[m_selectedLayer][i];
                    const Point2D& Q = m_layers[m_selectedLayer][j];
                    line(workXCenter + P.getX(), workYCenter - P.getY(), workXCenter + Q.getX(), workYCenter - Q.getY());
                }
            }
        }
    }
    for (size_t i = 0; i < m_layerPointButtons[m_selectedLayer].size(); ++i) {
        m_layerPointButtons[m_selectedLayer][i].drawLabel(RED, RED);
    }
    setcolor(ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR]);
    line(workXCenter, workY0, workXCenter, workY1);
    line(workX0, workYCenter, workX1, workYCenter);
}

void ObjectCreator::drawPointData() {
    int i = m_hovered;
    if (m_selectedLayer != -1) {
        i = m_layerAdjListIndex[m_selectedLayer][m_hovered];
    }
    const Point3D& pnt = (*m_workMesh)[i];
    setcolor(ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR]);
    rectangle(730, 30, 790, 120);
    setbkcolor(ColorSchemes::themeColors[m_theme][ColorSchemes::PRIMARYCOLOR]);
    MyArray<char, 32> headerText = itoa(i, "Point: "),
                        xPosText = itoa(pnt.getX(), "x: "),
                        yPosText = itoa(pnt.getY(), "y: "),
                        zPosText = itoa(pnt.getZ(), "z: ");
    outtextxy(735, 35, headerText.data());
    outtextxy(735, 55, xPosText.data());
    outtextxy(735, 75, yPosText.data());
    outtextxy(735, 95, zPosText.data());
}

void ObjectCreator::draw(bool update) {
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
        m_assistLine.draw();
        drawLayerView();
    }
    if (m_hovered != -1) {
        drawPointData();
    }
    drawSelectLayers();
    m_minimizedSpaceButton.drawLabel(LIGHTGRAY, RED);
    swapbuffers();
}

void ObjectCreator::pointAdder(const int& x, const int& y) {
    const int xCenter = (x0 + 26 + x1 - 200) / 2;
    const int yCenter = (y0 + 26 + y1) / 2;
    m_layers[m_selectedLayer].addPoint(Point2D(x - xCenter, yCenter - y));
    m_workMesh->addPoint(x - xCenter, yCenter - y, m_layers[m_selectedLayer].height());
    m_layerAdjListIndex[m_selectedLayer].push_back(m_workMesh->size() - 1);
    m_layerPointButtons[m_selectedLayer].push_back(CircularButton(x, y, 5));
    m_workArea.update();
    m_workArea.render();
    const Section& sect = m_workArea.sectionAt(0);
    m_pointButtons.push_back(CircularButton(sect[sect.size() - 1].getX(), sect[sect.size() - 1].getY(), 5));
    m_hovered = m_layers[m_selectedLayer].size() - 1;
}

void ObjectCreator::pointDeleter(const size_t& index) {
    const size_t i = m_layerAdjListIndex[m_selectedLayer][index];
    m_pointButtons.erase(m_pointButtons.begin() + i);
    m_workMesh->erase(i);
    m_layers[m_selectedLayer].removePoint(index);
    if (m_layers[m_selectedLayer].empty()) {
        m_layers.erase(m_layers.begin() + m_selectedLayer);
        m_layerSelectButtons.erase(m_layerSelectButtons.begin() + m_selectedLayer);
        m_layerPointButtons.erase(m_layerPointButtons.begin() + m_selectedLayer);
        m_layerAdjListIndex.erase(m_layerAdjListIndex.begin() + m_selectedLayer);
        m_selectedLayer = -1;
    }
    else {
        m_layerPointButtons[m_selectedLayer].erase(m_layerPointButtons[m_selectedLayer].begin() + index);
        m_layerAdjListIndex[m_selectedLayer].erase(m_layerAdjListIndex[m_selectedLayer].begin() + index);
    }
    for (MyVector<size_t>& mV : m_layerAdjListIndex) {
        for (size_t& k : mV) {
            if (k > i) {
                --k;
            }
        }
    }
    m_hovered = -1;
    m_workArea.update();
    draw(true);
}

void ObjectCreator::pointMover(const size_t& index) {
    const int xCenter = (workX0 + workX1) / 2;
    const int yCenter = (workY0 + workY1) / 2;
    size_t adjListIndex = m_layerAdjListIndex[m_selectedLayer][index];
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
            m_workArea.render();
            draw(true);
        }
    }
}

void ObjectCreator::pointConnector(const size_t& index) {
    const int xCenter = (workX0 + workX1) / 2;
    const int yCenter = (workY0 + workY1) / 2;
    setcolor(ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR]);
    const int& xFirstPoint = m_layers[m_selectedLayer][index].getX();
    const int& yFirstPoint = m_layers[m_selectedLayer][index].getY();
    clearmouseclick(WM_LBUTTONUP);
    int xDrag, yDrag;
    while (!ismouseclick(WM_LBUTTONUP)) {
        getmouseclick(WM_MOUSEMOVE, xDrag, yDrag);
        if (xDrag == -1) {
            continue;
        }
        m_assistLine = Line2D(xCenter + xFirstPoint, yCenter - yFirstPoint, xDrag, yDrag);
        getHoverCommand(xDrag, yDrag);
        draw();
    }
    resetLine();
    getmouseclick(WM_LBUTTONUP, xDrag, yDrag);
    size_t i;
    for (i = 0; i < m_layerPointButtons[m_selectedLayer].size(); ++i) {
        if (m_layerPointButtons[m_selectedLayer][i].hitCollision(xDrag, yDrag)) {
            break;
        }
    }
    if (i == m_layerPointButtons[m_selectedLayer].size()) {
        draw();
        return;
    }
    m_workMesh->addEdge(m_layerAdjListIndex[m_selectedLayer][index], m_layerAdjListIndex[m_selectedLayer][i]);
    m_workArea.update();
}

void ObjectCreator::toolOperationOnPoint(const size_t& index) {
    switch (m_tool) {
        case Tool::MovePoint:
            pointMover(index);
            break;
        case Tool::ConnectPoint:
            pointConnector(index);
            break;
        case Tool::DeletePoint:
            pointDeleter(index);
        case Tool::NewPoint:
        default:;
    }
}

bool ObjectCreator::getCommand() {
    int x, y;
    getmouseclick(WM_LBUTTONDOWN, x, y);
    if (x == -1) {
        return m_workArea.getKeyCommand();
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
        if (m_tool == Tool::NewPoint && m_workArea.insideWorkArea(x, y)) {
            pointAdder(x, y);
            return true;
        }
        for (size_t i = 0; i < m_layerPointButtons[m_selectedLayer].size(); ++i) {
            if (m_layerPointButtons[m_selectedLayer][i].hitCollision(x, y)) {
                toolOperationOnPoint(i);
                return true;
            }
        }
    }
    else if (m_workArea.getCommand(x, y)) {
        return true;
    }
    return false;
}

bool ObjectCreator::getHoverCommand(const int& xHov, const int& yHov) {
    if (m_selectedLayer == -1) {
        for (size_t i = 0; i < m_pointButtons.size(); ++i) {
            if (m_pointButtons[i].hitCollision(xHov, yHov)) {
                m_hovered = i;
                return true;
            }
        }
    }
    else {
        for (size_t i = 0; i < m_layerPointButtons[m_selectedLayer].size(); ++i) {
            if (m_layerPointButtons[m_selectedLayer][i].hitCollision(xHov, yHov)) {
                m_hovered = i;
                return true;
            }
        }
    }
    if (m_hovered != -1) {
        m_hovered = -1;
        return true;
    }
    return false;
}

bool ObjectCreator::getHoverCommand() {
    int xHov, yHov;
    getmouseclick(WM_MOUSEMOVE, xHov, yHov);
    if (xHov == -1) {
        return false;
    }
    return getHoverCommand(xHov, yHov);
}

Mesh ObjectCreator::run() {
    initwindow(x1, y1, "Object creator", 300, 25);
    setvisualpage(0);
    setactivepage(1);
    draw();
    while (true) {
        if (getCommand() || getHoverCommand()) {
            draw();
        }
    }
    return Mesh();
}
