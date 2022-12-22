#include "ObjectCreator.h"
#include <iostream>

//TODO:
//image icons (DONE, but I think I'll change them later anyway so who knows)
//change height of layer (DONE, sort of?)
//organize 3d-area fxns, bc right now they're called randomly in ifs
//pointConnector3D() (DONE)
//pointDeleter3D() (DONE)
//centering for selected layer in layer buttons view
//deleteLine()(DONE for 3D view, but not associated with a tool yet:
               //drag a line on screen at any time & it'll delete intersecting lines)

//TODO: (dupa ce rezolvam cu modularizarea txtInput)
//fxns for generating Sphere, Cone, Cylinder, care au toate parametri (default: 12, probabil)

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
    m_tool(Tool::ConnectPoint), x0(0), y0(0), x1(1000), y1(800), workX0(26), workY0(26), workX1(800), workY1(800), m_workArea(-1000, theme, this), m_workMesh(nullptr), m_selectedLayer(-1),
    m_hovered(-1), m_assistLine(-1, -1, -1, -1), m_assistLineDotted(false){
    toolButtonsInit();
    m_workArea.setCorners(26, 26, 800, 800);
    m_workArea.addMesh(Mesh());
    m_workMesh = &m_workArea.meshAt(0);
    m_minimizedSpaceButton = Button(900, 100, 200, 200);
    m_addLayerButton = Button(900, 200 + m_layers.size() * 40, 40, 40);
}

ObjectCreator::ObjectCreator(Mesh mesh, const int& theme) :
    m_theme(theme), m_layers(), m_layerSelectButtons(), m_layerPointButtons(), m_layerAdjListIndex(), m_addLayerButton(), m_pointButtons(), m_minimizedSpaceButton(), m_toolButtons(), m_tool(Tool::MovePoint),
    x0(0), y0(0), x1(1000), y1(800), workX0(26), workY0(26), workX1(800), workY1(800), m_workArea(-1000, theme, this), m_workMesh(nullptr), m_selectedLayer(-1), m_hovered(-1), m_assistLine(-1, -1, -1, -1), m_assistLineDotted(false) {
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
    m_selectedLayer(oc.m_selectedLayer), m_hovered(oc.m_hovered), m_assistLine(oc.m_assistLine), m_assistLineDotted(oc.m_assistLineDotted) {}

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
    MyArray<MyArray<char, 128>, 4> filenames;
    filenames[(size_t)Tool::NewPoint] = "media\\buttonAddPoint.jpg";
    filenames[(size_t)Tool::MovePoint] = "media\\buttonMove.jpg";
    filenames[(size_t)Tool::ConnectPoint] = "media\\buttonAddLine.jpg";
    filenames[(size_t)Tool::DeletePoint] = "media\\buttonDelete.jpg";
    for (size_t i = 0; i < 4; ++i) {
        m_toolButtons[i] = ImageButton(12, 36 + 24 * i, 24, 24, filenames[i].data());
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
                m_layers[j].addPoint(Point2D(round(pnt.getX()), round(pnt.getY())));
                m_layerAdjListIndex[j].push_back(i);
                break;
            }
        }
        if (!levelExists) {
            m_layers.push_back(Layer());
            const Point3D& pnt = (*m_workMesh)[i];
            m_layers.back().changeHeight(round(pnt.getZ()));
            m_layers.back().addPoint(Point2D(round(pnt.getX()), round(pnt.getY())));
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
        m_layerPointButtons[i] = m_layers[i].renderButtons(x0 + 26, y0 + 26, x1 - 200, y1);
    }
    renderLayerSelectButtons();
    m_minimizedSpaceButton = Button(x1 - 100, y0 + 100, 200, 200);
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

int ObjectCreator::atoi(MyArray<char, 256>& arr) {
    size_t len = strlen(arr.data());
    if (len > 8) {
        return INT_MIN;
    }
    int x = 0;
    size_t i = 0;
    if (arr[0] == '-') {
        ++i;
    }
    for (; i < len; ++i) {
        if (arr[i] < '0' || '9' < arr[i]) {
            return INT_MIN;
        }
        x = x * 10 + (arr[i] - '0');
    }
    if (arr[0] == '-') {
        x *= -1;
    }
    return x;
}

void ObjectCreator::resetLine() {
    m_assistLine = Line2D(0, 0, 0, 0);
}

void ObjectCreator::drawToolButtons() {
    for (size_t i = 0; i < m_toolButtons.size(); ++i) {
        m_toolButtons[i].drawImageButton();
    }
}

void ObjectCreator::drawSelectLayers() {
    for (size_t i = 0; i < m_layerSelectButtons.size(); ++i) {
        m_layerSelectButtons[i].drawTextButton(1, 3, RGB(160, 160, 160), false);
    }
    //butonu PLUS
    int x = m_addLayerButton.getXCenter();
    int y = m_addLayerButton.getYCenter();
    setcolor(ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR]);
    line(x - 15, y, x + 15, y);
    line(x, y - 15, x, y + 15);
}

void ObjectCreator::renderLayerSelectButtons() {
    //TODO: choose which layerBtns to draw such that new selected layer goes roughly in center of onscreen list of layerBtns
    //(dependent on current resolution, how many fit on screen... etc)
    for (size_t i = 0; i < m_layers.size(); ++i) {
        MyArray<char, 32> layerLevel = itoa(m_layers[i].height(), "Layer Z: ");
        m_layerSelectButtons[i] = TextButton(x1 - 100, y0 + 200 + 20 + i * 40, 200, 40, layerLevel.data());
    }
    m_addLayerButton = Button(x1 - 220, y0 + 220, 40, 40);
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
    setcolor(RED);
    line(workXCenter, workY0, workXCenter, workY1);
    line(workX0, workYCenter, workX1, workYCenter);
    setcolor(ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR]);
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
        setcolor(ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR]);
        if (m_assistLineDotted) {
            struct linesettingstype oldSettings;
            getlinesettings(&oldSettings);
            setlinestyle(3, 0, 2);
            m_assistLine.draw();
            setlinestyle(oldSettings.linestyle, oldSettings.upattern, oldSettings.thickness);
        }
        else {
            m_assistLine.draw();
        }
        if (update) {
            updateButtons();
        }
        drawButtons();

    }
    else {
        drawLayerView();
        setcolor(ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR]);
        if (m_assistLineDotted) {
            struct linesettingstype oldSettings;
            getlinesettings(&oldSettings);
            setlinestyle(3, 0, 2);
            m_assistLine.draw();
            setlinestyle(oldSettings.linestyle, oldSettings.upattern, oldSettings.thickness);
        }
        else {
            m_assistLine.draw();
        }
    }
    if (m_hovered != -1) {
        drawPointData();
    }
    drawSelectLayers();
    m_minimizedSpaceButton.drawLabel(LIGHTGRAY, RED);
    swapbuffers();
}

int ObjectCreator::getLayerByHeight(const int& height) {
    for (size_t i = 0; i < m_layers.size(); ++i) {
        if (m_layers[i].height() == height) {
            return i;
        }
    }
    return -1;
}

void ObjectCreator::editLayer(const int& index) {
    TextButton& editedLayer = m_layerSelectButtons[index];
    editedLayer.drawLabel(LIGHTGRAY, BLACK);
    TextInputBox txtBox(editedLayer.getXCenter() - 35, editedLayer.getXCenter() + 100, editedLayer.getYCenter(), BLACK, RGB(150, 150, 150), 4, "-0123456789");
    setactivepage(0);
    setvisualpage(0);
    MyArray<char, 256> readInput = txtBox.getText();
    int result = atoi(readInput);
    if (result == INT_MIN && result != m_layers[index].height()) {
        return;
    }
    //daca Z este diferit de original, modifica tot ce trebuie modificat
    //Daca mutam astfel incat 2 layere sa aibe acelasi height,
    //le lasam pe amandoua pe acelasi height, separate.
    //bubble sort si insertion sort sunt stabile, deci
    //nu o sa se schimbe ordinea consecutiva la sortari
    //TODO: bug testing and fixing

    if (result != INT_MIN && result != m_layers[index].height()) {
        for (size_t i = 0; i < m_layers[index].size(); ++i) {
            (*m_workMesh)[m_layerAdjListIndex[index][i]].setZ((double)result);
        }
        m_layers[index].changeHeight(result);
        MyArray<char, 32> resultText = itoa(result, "Layer Z: ");
        m_layerSelectButtons[index].modifyText(resultText);
        bool sorted = true;
        do {
            sorted = true;
            for (size_t i = 1; i < m_layers.size(); ++i) {
                if (m_layers[i - 1].height() > m_layers[i].height()) {
                    std::swap(m_layers[i - 1], m_layers[i]);
                    std::swap(m_layerAdjListIndex[i - 1], m_layerAdjListIndex[i]);
                    std::swap(m_layerPointButtons[i - 1], m_layerPointButtons[i]);
                    //swap layer select buttons
                    MyArray<char, 32> textPred = itoa(m_layers[i - 1].height(), "Layer Z: "),
                                      textSucc = itoa(m_layers[i].height(), "Layer Z: ");
                    m_layerSelectButtons[i - 1].modifyText(MyArray<char, 32>(textPred));
                    m_layerSelectButtons[i].modifyText(MyArray<char, 32>(textSucc));
                    sorted = false;
                }
            }
        } while (!sorted);
        for (size_t i = 0; i < m_layers.size(); ++i) {
            updateButtons();
            if (m_layers[i].height() == result) {
                m_selectedLayer = i;
                break;
            }
        }
        //draw(true);
    }
    m_workArea.update();
}

void ObjectCreator::pointAdder(const int& x, const int& y) {
    if (!m_workArea.insideWorkArea(x, y)) {
        return;
    }
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
        renderLayerSelectButtons();
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

void ObjectCreator::pointDeleter3D(const size_t& index) {
    Point3D& pnt = (*m_workMesh)[index];
    int pntHeight = round(pnt.getZ());
    std::cout<<"height: "<<pntHeight<<"\n";
    int layerIndex = getLayerByHeight(pntHeight);
    std::cout<<"layerIndex: "<<layerIndex<<"\n";
    //se gaseste indexul local din layer, ca sa copiez practic codul de la fct 2d (cu mici modificari)
    //si oricum trebuie modificate toate chestiile astea de pe layerul unde se afla
    //daca poti gasi mai repede indexul care ne trebuie (instead of a for), go ahead -
    //deocamdata vreau sa fie functional, eficienta o rezolvam noi mai incolo
    for (size_t i = 0; i < m_layers[layerIndex].size(); ++i) {
        if (m_layerAdjListIndex[layerIndex][i] == index) {
            std::cout<<"global index: "<<index<<"\n";
            std::cout<<"local layer index"<<i<<"\n";
            m_pointButtons.erase(m_pointButtons.begin() + index);
            m_workMesh->erase(index);
            m_layers[layerIndex].removePoint(i);
            if (m_layers[layerIndex].empty()) {
                m_layers.erase(m_layers.begin() + layerIndex);
                m_layerSelectButtons.erase(m_layerSelectButtons.begin() + layerIndex);
                m_layerPointButtons.erase(m_layerPointButtons.begin() + layerIndex);
                m_layerAdjListIndex.erase(m_layerAdjListIndex.begin() + layerIndex);
            }
            else {
                m_layerPointButtons[layerIndex].erase(m_layerPointButtons[layerIndex].begin() + i);
                m_layerAdjListIndex[layerIndex].erase(m_layerAdjListIndex[layerIndex].begin() + i);
            }
            for (MyVector<size_t>& mV : m_layerAdjListIndex) {
                for (size_t& k : mV) {
                    if (k > index) {
                        --k;
                    }
                }
            }
            m_hovered = -1;
            break;
        }
    }
    m_workArea.update();
    //draw(true);
}

void ObjectCreator::pointMover(const size_t& index) {
    const int xCenter = (workX0 + workX1) / 2;
    const int yCenter = (workY0 + workY1) / 2;
    size_t adjListIndex = m_layerAdjListIndex[m_selectedLayer][index];
    int xDrag, yDrag;
    clearmouseclick(WM_LBUTTONUP);
    while (!ismouseclick(WM_LBUTTONUP)) {
        getmouseclick(WM_MOUSEMOVE, xDrag, yDrag);
        if (xDrag != -1 && m_workArea.insideWorkArea(xDrag, yDrag)) {
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

void ObjectCreator::pointConnector2D(const size_t& index) {
    const int xCenter = (workX0 + workX1) / 2;
    const int yCenter = (workY0 + workY1) / 2;
    setcolor(ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR]);
    const int& xFirstPoint = m_layers[m_selectedLayer][index].getX();
    const int& yFirstPoint = m_layers[m_selectedLayer][index].getY();
    clearmouseclick(WM_LBUTTONUP);
    int xDrag, yDrag;
    while (!ismouseclick(WM_LBUTTONUP)) {
        getmouseclick(WM_MOUSEMOVE, xDrag, yDrag);
        if (xDrag == -1 || !m_workArea.insideWorkArea(xDrag, yDrag)) {
            continue;
        }
        m_assistLine = Line2D(xCenter + xFirstPoint, yCenter - yFirstPoint, xDrag, yDrag);
        getHoverCommand(xDrag, yDrag);
        draw();
    }
    resetLine();
    getmouseclick(WM_LBUTTONUP, xDrag, yDrag);
    if (!m_workArea.insideWorkArea(xDrag, yDrag)) {
        return;
    }
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

void ObjectCreator::pointConnector3D(const size_t& index) {
    setcolor(ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR]);
    const Section& sect = m_workArea.sectionAt(0);
    const int& xFirstPoint = sect[index].getX();
    const int& yFirstPoint = sect[index].getY();
    clearmouseclick(WM_LBUTTONUP);
    int xDrag, yDrag;
    while (!ismouseclick(WM_LBUTTONUP)) {
        getmouseclick(WM_MOUSEMOVE, xDrag, yDrag);
        if (xDrag == -1 || !m_workArea.insideWorkArea(xDrag, yDrag)) {
            continue;
        }
        m_assistLine = Line2D(xFirstPoint, yFirstPoint, xDrag, yDrag);
        getHoverCommand(xDrag, yDrag);
        draw();
    }
    resetLine();
    getmouseclick(WM_LBUTTONUP, xDrag, yDrag);
    if (!m_workArea.insideWorkArea(xDrag, yDrag)) {
        return;
    }
    size_t i;
    for (i = 0; i < m_pointButtons.size(); ++i) {
        if (m_pointButtons[i].hitCollision(xDrag, yDrag)) {
            break;
        }
    }
    if (i == m_pointButtons.size()) {
        resetLine();
        draw();
        return;
    }
    m_workMesh->addEdge(index, i);
    m_workArea.update();
}

void ObjectCreator::deleteLines3D(const int& x, const int& y){
    const Section& sect = m_workArea.sectionAt(0);
    clearmouseclick(WM_LBUTTONUP);
    int xDrag, yDrag;
    while (!ismouseclick(WM_LBUTTONUP)) {
        getmouseclick(WM_MOUSEMOVE, xDrag, yDrag);
        if (xDrag == -1 || !m_workArea.insideWorkArea(xDrag, yDrag)) {
            continue;
        }
        m_assistLine = Line2D(x, y, xDrag, yDrag);
        getHoverCommand(xDrag, yDrag);
        draw();
    }
    resetLine();
    getmouseclick(WM_LBUTTONUP, xDrag, yDrag);
    if (!m_workArea.insideWorkArea(xDrag, yDrag)) {
        return;
    }
    bool hasDeletedLine = false;
    Point2D p1(x, y), p2(xDrag, yDrag);
    for (size_t i = 0; i < sect.size(); ++i) {
        for (size_t j = 0; j < m_workMesh->adjacencyListReference()[i].size(); ++j) {
            if (linesIntersect(p1, p2, sect[i], sect[m_workMesh->adjacencyListReference()[i][j]])) {
                m_workMesh->deleteIndexConnection(i, m_workMesh->adjacencyListReference()[i][j]);
                hasDeletedLine = true;
                j--;
            }
        }
    }
    resetLine();
    if (hasDeletedLine) {
        m_workArea.update();
    }
    draw(hasDeletedLine);
}

void ObjectCreator::deleteLines2D(const int& x, const int& y) {
clearmouseclick(WM_LBUTTONUP);
    int xDrag, yDrag;
    while (!ismouseclick(WM_LBUTTONUP)) {
        getmouseclick(WM_MOUSEMOVE, xDrag, yDrag);
        if (xDrag == -1 || !m_workArea.insideWorkArea(xDrag, yDrag)) {
            continue;
        }
        m_assistLine = Line2D(x, y, xDrag, yDrag);
        getHoverCommand(xDrag, yDrag);
        draw();
    }
    resetLine();
    getmouseclick(WM_LBUTTONUP, xDrag, yDrag);
    if (!m_workArea.insideWorkArea(xDrag, yDrag)) {
        return;
    }
    bool hasDeletedLine = false;
    Point2D p1(x, y), p2(xDrag, yDrag);
    const int xCenter = (workX0 + workX1) / 2;
    const int yCenter = (workY0 + workY1) / 2;
    Point2D center(xCenter, yCenter);
    for (size_t i = 0; i < m_layers[m_selectedLayer].size(); ++i) {
        for (size_t j = i+1 ; j < m_layers[m_selectedLayer].size(); ++j) {
            size_t index1 =  m_layerAdjListIndex[m_selectedLayer][i], index2 = m_layerAdjListIndex[m_selectedLayer][j];
            Point2D point1 = m_layers[m_selectedLayer][i], point2 = m_layers[m_selectedLayer][j];
            //in 3d, y neg-poz = din fata in spate
            //in 2d, y neg-poz = de sus in jos (cumva din "spate" in "fata")
            //deci trb inversate
            point1.setY(-point1.getY());
            point2.setY(-point2.getY());
            if (linesIntersect(p1, p2, center + point1, center + point2 )){
                m_workMesh->deleteIndexConnection(index1, index2);
                hasDeletedLine = true;
            }
        }
    }
    resetLine();
    if (hasDeletedLine) {
        m_workArea.update();
    }
    draw(hasDeletedLine);
}

void ObjectCreator::toolOperationOnPoint(const size_t& index) {
    //TODO:
    //if m_selected do on layer m_selected. else do in 3d.
    //not sure if we'll eventually merge fxns like
    //pointConnector2D and pointConnector3D together...
    //who knows
    switch (m_tool) {
        case Tool::MovePoint:
            pointMover(index);
            break;
        case Tool::ConnectPoint:
            m_assistLineDotted = false;
            pointConnector2D(index);
            break;
        case Tool::DeletePoint:
            pointDeleter(index);
        case Tool::NewPoint:
        default:;
    }
}

bool ObjectCreator::getClickCommand() {
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
            //TODO: draw minimizedSpace:
            //either copy to separate space and render that
            //or take current section and center it
            //and translate all pnts relative to center
            //such that it fits the small area
            //idk tho. will think abt that later
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
        if (m_workArea.insideWorkArea(x, y)) {
            m_assistLineDotted = true;
            deleteLines2D(x, y);
            return true;
        }
    }
    else {
        for (size_t i = 0; i < m_pointButtons.size(); ++i) {
            if (m_pointButtons[i].hitCollision(x, y)) {
                if (m_tool == Tool::ConnectPoint) {
                    m_assistLineDotted = false;
                    pointConnector3D(i);
                    return true;
                }
                else if(m_tool == Tool::DeletePoint) {
                    pointDeleter3D(i);
                    return true;
                }
            }
        }
        if (m_workArea.getCommand(x, y)) {
            return true;
        }
        if (m_workArea.insideWorkArea(x, y)) {
            m_assistLineDotted = true;
            deleteLines3D(x, y);
            return true;
        }
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

bool ObjectCreator::getDoubleClickCommand() {
    int x, y;
    getmouseclick(WM_LBUTTONDBLCLK, x, y);
    if (x == -1) {
        return false;
    }
    for (size_t i = 0; i < m_layerSelectButtons.size(); ++i) {
        if (m_layerSelectButtons[i].hitCollision(x, y)) {
            editLayer(i);
            return true;
        }
    }
    return false;
}

Mesh ObjectCreator::run() {
    initwindow(x1, y1, "Object creator", 300, 25);
    setvisualpage(0);
    setactivepage(1);
    draw();
    while (true) {
        if (getDoubleClickCommand() || getClickCommand() || getHoverCommand()) {
            draw();
        }
    }
    return Mesh();
}
