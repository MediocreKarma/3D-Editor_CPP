#include "ObjectCreator.h"
#include <iostream>

Layer::Layer(MyVector<MyVector<size_t>>* adjListPointer = nullptr) :
    m_points(), m_pointButtons(), m_adjListPointer(adjListPointer), m_indices(), m_updated(), m_height() {}

Layer::Layer(const Layer& other) :
    m_points(other.m_points), m_pointButtons(other.m_pointButtons), m_adjListPointer(other.m_adjListPointer), m_indices(other.m_indices), m_updated(other.m_updated), m_height(other.m_height) {}

Layer& Layer::operator = (const Layer& other) {
    m_points = other.m_points;
    m_pointButtons = other.m_pointButtons;
    m_height = other.m_height;
    m_adjListPointer = other.m_adjListPointer;
    m_updated = other.m_updated;
    m_indices = other.m_indices;
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

void Layer::addPoint(const Point2D& point, const size_t& adjListIndex) {
    m_points.push_back(point);
    m_indices.push_back(adjListIndex);
    m_updated.push_back(true);
}

void Layer::removePoint(const size_t& adjListIndex) {
    size_t index = size();
    for (size_t i = 0; i < size(); ++i) {
        if (m_indices[i] == adjListIndex) {
            index = i;
            break;
        }
    }
    if (index == size()) {
        return;
    }
    for (size_t i = index + 1; i < size(); ++i) {
        m_points[i - 1] = m_points[i];
        m_indices[i - 1] = m_indices[i];
        m_pointButtons[i - 1] = m_pointButtons[i];
    }
    m_points.pop_back();
    m_indices.pop_back();
    m_pointButtons.pop_back();
}

MyVector<CircularButton> Layer::renderButtons(const int& x0, const int& y0, const int& x1, const int& y1) {
    const int xCenter = (x0 + x1) / 2;
    const int yCenter = (y0 + y1) / 2;
    MyVector<CircularButton> pointButtons(m_points.size());
    for (size_t i = 0; i < m_points.size(); ++i) {
        if (m_updated[i]) {
            m_pointButtons[i] = CircularButton(xCenter + m_points[i].getX(), yCenter - m_points[i].getY(), 5);
            m_updated[i] = false;
        }
    }
    return pointButtons;
}

ObjectCreator::ObjectCreator(const int& theme) :
    m_theme(theme), m_layers(), m_layerSelectButtons(), m_addLayerButton(), m_pointButtons(), m_adjList(), x0(0), y0(0), x1(1000), y1(825), m_workArea(-2500, theme, this), m_selectedLayer(-1) {
    m_workArea.setCorners(0, 25, 800, 825);
    m_workArea.addMesh(Mesh());
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

ObjectCreator::ObjectCreator(Mesh mesh, const int& theme) :
    m_theme(theme), m_layers(), m_layerSelectButtons(), m_addLayerButton(), m_pointButtons(), m_adjList(mesh.adjacencyList()),
    x0(0), y0(0), x1(1000), y1(825), m_workArea(-2500, theme, this), m_selectedLayer(-1) {
    m_workArea.setCorners(25, 25, 800, 825);
    mesh.translate(-mesh.centerPoint().getX(), -mesh.centerPoint().getY(), -mesh.centerPoint().getZ());
    m_workArea.addMesh(mesh);
    m_workArea.render();
    const Section& sect = m_workArea.sectionAt(0);
    m_pointButtons.resize(sect.size());
    updateButtons();
    for (size_t i = 0; i < sect.size(); ++i) {
        bool levelExists = false;
        for (size_t j = 0; j < m_layers.size(); ++j) {
            if (m_layers[j].height() == mesh[i].getZ()) {
                levelExists = true;
                m_layers[j].addPoint(Point2D(mesh[i].getX(), mesh[i].getY()), i);
            }
        }
        if (!levelExists) {
            m_layers.push_back(Layer(&m_adjList));
            m_layers.back().changeHeight(mesh[i].getZ());
            m_layers.back().addPoint(Point2D(mesh[i].getX(), mesh[i].getY()), i);
        }
    }
    m_layerSelectButtons.reserve(m_layers.size());
    for (size_t i = 0; i < m_layers.size(); ++i) {
        MyArray<char, 32> layerLevel = "Layer Z: ";
        MyArray<char, 24> zText = itoa(m_layers[i].height());
        for (size_t j = 9; zText[j - 9]; ++j) {
            layerLevel[j] = zText[j - 9];
        }
        m_layerSelectButtons.push_back(TextButton(900, 200 + i * 40, 200, 40, layerLevel.data()));
    }
    m_addLayerButton = Button(900, 200 + m_layers.size() * 40, 40, 40);
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
        m_pointButtons[i].drawLabel(LIGHTCYAN, LIGHTCYAN);
    }
}

void ObjectCreator::draw() {
    setfillstyle(SOLID_FILL, ColorSchemes::themeColors[m_theme][ColorSchemes::PRIMARYCOLOR]);
    bar(25, 25, 825, 825);
    if (m_selectedLayer == -1) {
        m_workArea.run();
        drawButtons();
    }
    else {
        m_layers[m_selectedLayer].renderButtons(25, 25, 825, 825);
    }
    drawSelectLayers();
    swapbuffers();
}

bool ObjectCreator::getCommand(const int& x, const int& y) {
    for (size_t i = 0; i < m_layerSelectButtons.size(); ++i) {
        if (m_layerSelectButtons[i].hitCollision(x, y)) {
            m_selectedLayer = i;
            return true;
        }
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
