#include "ObjectCreator.h"
#include <iostream>

const double pi = 3.1415926535897;
/*

DONE:
move layer view (DONE, using same tool as PointMover for now - with tool selected, drag on screen)
generate fxns for Cone, Cylinder, Cube, Sphere

TODO:
-Duplicate, Duplicate Linked (adds vertical connections)
-organize 3d-area fxns, bc right now they're called randomly in ifs
-separate layerView workArea (add separate bool fxn or something, why bother with more global stuff)
-draw minimizedSpace?
-i initially wanted to be able to zoom in on layers but zooming would lead to rounding errors (Point2D(int, int))
 (might be fixed when adding maps as we'd be able to pull points straight from 3d mesh easier? idk)


*/
Layer::Layer() :
    m_points(), m_updated(), m_height(), m_center(Point2D(0, 0)) {}

Layer::Layer(const Layer& other) :
    m_points(other.m_points), m_updated(other.m_updated), m_height(other.m_height), m_center(other.m_center) {}

Layer& Layer::operator = (const Layer& other) {
    m_points = other.m_points;
    m_height = other.m_height;
    m_updated = other.m_updated;
    m_center = other.m_center;
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

Point2D Layer::center() const {
    return m_center;
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

//PENTRU randare completa, fara sa i pese de m_updated
MyVector<CircularButton> Layer::renderButtons(const int& x0, const int& y0, const int& x1, const int& y1) {
    const int xCenter = (x0 + x1) / 2;
    const int yCenter = (y0 + y1) / 2;
    MyVector<CircularButton> pointButtons(m_points.size());
    for (size_t i = 0; i < m_points.size(); ++i) {
        pointButtons[i] = CircularButton(xCenter + m_points[i].getX(), yCenter - m_points[i].getY(), 5);
    }
    return pointButtons;
}

//PENTRU randare partiala cu un vector-destinatie, daca ne pasa de m_updated.
//nu garantez ca vom avea nevoie de ea. insa felul in care foloseai m_updated in originala
//era straight up pointless; pt ca in vectorul original tot se mutau toate elementele,
//si se faceau niste initializari degeaba, si se stergeau date, etc.
//asa, daca ai nevoie, ai functia asta separata care se foloseste de m_updated.
void Layer::renderButtons(const int& x0, const int& y0, const int& x1, const int& y1, MyVector<CircularButton>& existingButtons) {
    const int xCenter = (x0 + x1) / 2;
    const int yCenter = (y0 + y1) / 2;
    if (existingButtons.size() < m_points.size()) {
        //nu stiu exact daca o sa apara vreodata eroarea asta tbh ca in init o sa folosesti oricum cealalta functie
        std::cout<<"ti o aparut eroarea aia la renderButtons\n";
        return;
    }
    for (size_t i = 0; i < m_points.size(); ++i) {
        if (m_updated[i]) {
            existingButtons[i] = CircularButton(xCenter + m_points[i].getX(), yCenter - m_points[i].getY(), 5);
            m_updated[i] = false;
        }
    }
}

void Layer::update() {
    m_updated.fill(true);
}

void Layer::update(const size_t& index) {
    m_updated[index] = true;
}


void Layer::moveRel(const int& x, const int& y) {
    Point2D rel(x, y);
    for (auto& pnt : m_points) {
        pnt += rel;
    }
    m_center += rel;
}

void Layer::moveRel(const Point2D& rel) {
    for (auto& pnt : m_points) {
        pnt += rel;
    }
    m_center += rel;
}

ObjectCreator::ObjectCreator(const int& theme) :
    m_theme(theme), m_layers(), m_layerSelectButtons(), m_layerPointButtons(), m_layerAdjListIndex(), m_addLayerButton(), m_pointButtons(), m_minimizedSpaceButton(), m_toolButtons(),
    m_tool(Tool::MovePoint), x0(0), y0(0), x1(1000), y1(800), workX0(26), workY0(26), workX1(800), workY1(800), m_workArea(-1000, theme, this), m_workMesh(nullptr), m_selectedLayer(-1),
    m_hovered(-1), m_assistLine(-1, -1, -1, -1), m_assistLineDotted(false), m_layerSelectsBegin(-1), m_layerSelectsEnd(-1), m_layerScrollArrows(), m_layerTools() {
    toolButtonsInit();
    m_workArea.setCorners(26, 26, 800, 800);
    m_workArea.addMesh(Mesh());
    m_workMesh = &m_workArea.meshAt(0);
    m_minimizedSpaceButton = Button(900 - 1, 100 - 1, 200 - 1, 200 - 4);
    m_addLayerButton = Button(900, 200 + m_layers.size() * 40, 40, 40);
}

ObjectCreator::ObjectCreator(Mesh mesh, const int& theme) :
    m_theme(theme), m_layers(), m_layerSelectButtons(), m_layerPointButtons(), m_layerAdjListIndex(), m_addLayerButton(), m_pointButtons(), m_minimizedSpaceButton(),
    m_toolButtons(), m_tool(Tool::MovePoint), x0(0), y0(0), x1(1000), y1(800), workX0(26), workY0(26), workX1(800), workY1(800), m_workArea(-1000, theme, this), m_workMesh(nullptr),
    m_selectedLayer(-1), m_hovered(-1), m_assistLine(-1, -1, -1, -1), m_assistLineDotted(false), m_layerSelectsBegin(-1), m_layerSelectsEnd(-1), m_layerScrollArrows(), m_layerTools() {
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
    m_selectedLayer(oc.m_selectedLayer), m_hovered(oc.m_hovered), m_assistLine(oc.m_assistLine), m_assistLineDotted(oc.m_assistLineDotted),
    m_layerSelectsBegin(oc.m_layerSelectsBegin), m_layerSelectsEnd(oc.m_layerSelectsEnd), m_layerScrollArrows(oc.m_layerScrollArrows), m_layerTools(oc.m_layerTools){}

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
    m_layerSelectsBegin = oc.m_layerSelectsBegin;
    m_layerSelectsEnd = oc.m_layerSelectsEnd;
    m_workArea.render();
    init();
    return *this;
}

void ObjectCreator::toolButtonsInit() {
    MyArray<MyArray<char, 128>, 5> filenames;
    filenames[(size_t)Tool::NewPoint] = "media\\buttonAddPoint.gif";
    filenames[(size_t)Tool::MovePoint] = "media\\buttonMove.gif";
    filenames[(size_t)Tool::ConnectPoint] = "media\\buttonAddLine.gif";
    filenames[(size_t)Tool::DeletePoint] = "media\\buttonDelete.gif";
    filenames[(size_t)Tool::CutLine] = "media\\buttonLineCutter.gif";
    size_t buttonSize = 32;
    for (size_t i = 0; i < 5; ++i) {
        m_toolButtons[i] = ImageButton(buttonSize / 2, 36 + buttonSize * i, buttonSize, buttonSize, filenames[i].data());
    }
    //MyArray<MyArray<char, 128>, 1> layerFilenames;
    //pt cand o sa avem probabil tools precum Duplicate, Duplicate Linked, Draw Circle, Draw Rectangle
    //deocamdata este initializat un MyArray cu 1 imagebutton care nu i folosit la nimic
}

void ObjectCreator::init() {
    m_workArea.removeMesh(0);
    m_workArea.addMesh(generateCylinder(200, 100, 12));
    m_workMesh = &m_workArea.meshAt(0);
    m_workArea.render();
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
            if (m_layers[i - 1].height() < m_layers[i].height()) {
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
    int availableSlots = (y1 - y0 - 200) / 40 + ((y1 - y0 - 200) % 40 >= 20 ? 0 : -1);
    m_layerSelectsBegin = 0;
    m_layerSelectsEnd = (availableSlots < (int)m_layers.size() ? availableSlots - 1 : m_layers.size() - 1);
    renderLayerSelectButtons();
    m_minimizedSpaceButton = Button(x1 - 100 - 1, y0 + 100 - 1, 200 - 1, 200 - 4);
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
    for (auto& button : m_toolButtons) {
        button.drawLabel(ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR], BLACK);
        button.drawImageButton();
    }
}

void ObjectCreator::drawSelectLayers() {
    //draw layer btns
    int btnColor = RGB(160, 160, 160),
        highlightedColor = ColorSchemes::mixColors(btnColor, ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR], 50);
    for (auto& button : m_layerSelectButtons) {
        if (m_selectedLayer != (&button) - m_layerSelectButtons.begin()) {
            button.drawTextButton(1, 3, btnColor, false);
        }
        else {
            button.drawTextButton(1, 3, highlightedColor, false);
        }
    }
    //butoane de PLUS + arrows
    setcolor(ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR]);
    int x = m_addLayerButton.getXCenter();
    int y = m_addLayerButton.getYCenter();
    int visualHalfBtnSize = 15;
    line(x - visualHalfBtnSize, y, x + visualHalfBtnSize, y);
    line(x, y - visualHalfBtnSize, x, y + visualHalfBtnSize);
    x = m_layerScrollArrows[0].getXCenter();
    line(x - visualHalfBtnSize, y + visualHalfBtnSize / 2, x, y - visualHalfBtnSize / 2);
    line(x + visualHalfBtnSize, y + visualHalfBtnSize / 2, x, y - visualHalfBtnSize / 2);
    x = m_layerScrollArrows[1].getXCenter();
    line(x - visualHalfBtnSize, y - visualHalfBtnSize / 2, x, y + visualHalfBtnSize / 2);
    line(x + visualHalfBtnSize, y - visualHalfBtnSize / 2, x, y + visualHalfBtnSize / 2);
}

void ObjectCreator::renderLayerSelectButtons() {
    updateLayerSelectsInterval();
    int rendered = 0;
    for (int i = 0; i < m_layerSelectsBegin; ++i) {
        m_layerSelectButtons[i] = TextButton(-100, -100, 1, 1, "");
    }
    for (int i = m_layerSelectsBegin; i <= m_layerSelectsEnd; ++i) {
        MyArray<char, 32> layerLevel = itoa(m_layers[i].height(), "Layer Z: ");
        m_layerSelectButtons[i] = TextButton(x1 - 100 - 1, y0 + 200 + 20 + rendered++ * 40, 200 - 1, 40, layerLevel.data());
    }
    for (size_t i = m_layerSelectsEnd + 1; i < m_layers.size(); ++i) {
        m_layerSelectButtons[i] = TextButton(-100, -100, 1, 1, "");
    }
    m_addLayerButton = Button(x1 - 180, y0 + 200 + 20 + (m_layerSelectsEnd - m_layerSelectsBegin + 1) * 40, 40, 40);
    m_layerScrollArrows[0] = Button(x1 - 100, y0 + 200 + 20 + (m_layerSelectsEnd - m_layerSelectsBegin + 1) * 40, 40, 40);
    m_layerScrollArrows[1] = Button(x1 - 20, y0 + 200 + 20 + (m_layerSelectsEnd - m_layerSelectsBegin + 1) * 40, 40, 40);
}

void ObjectCreator::updateLayerSelectsInterval() {
    if ((y1 - y0 - 200) / 40 + ((y1 - y0 - 200) % 40 >= 20 ? 0 : -1) <= (int)m_layers.size()) {
        return;
    }
    else {
        if (m_layerSelectsEnd > (int)m_layers.size() - 1) {
            while (m_layerSelectsBegin > 0) {
                --m_layerSelectsEnd;
                --m_layerSelectsBegin;
            }
            if (m_layerSelectsEnd > (int)m_layers.size() - 1) {
                //singura situatie in care se poate intampla asta
                //chiar si dupa move este momentul in care intervalul
                //a fost mutat in sus la maxim dar tot e prea mare
                //Begin este, deci, deja la 0
                m_layerSelectsEnd = m_layers.size() - 1;
            }
            return;
        }
        else if (m_layerSelectsBegin < 0) {
            //nu cred ca putem ajunge in situatia asta
            //dar daca dau vreodata de ea, o adaug
            std::cout<<"ai dat de situatia aia de care ziceai ca nu dai\n";
            return;
        }
    }
}

//true if layers onscreen have actually changed
bool ObjectCreator::moveLayerSelectsInterval(const int& delta) {
    int oldBegin = m_layerSelectsBegin, oldEnd = m_layerSelectsEnd;
    if ((y1 - y0 - 200) / 40 + ((y1 - y0 - 200) % 40 >= 20 ? 0 : -1) >= (int)m_layers.size() || delta == 0) {
        return false;
    }
    if (delta < 0) {
        int localDelta = (m_layerSelectsBegin + delta >= 0 ? delta : -m_layerSelectsBegin);
        m_layerSelectsBegin += localDelta;
        m_layerSelectsEnd += localDelta;
    }
    if (delta > 0) {
        int localDelta = (m_layerSelectsEnd + delta < (int)m_layers.size() ? delta : (int)m_layers.size() - 1 - m_layerSelectsEnd);
        m_layerSelectsBegin += localDelta;
        m_layerSelectsEnd += localDelta;
    }
    if (oldBegin != m_layerSelectsBegin || oldEnd != m_layerSelectsEnd) {
        renderLayerSelectButtons();
        return true;
    }
    return false;
}

void ObjectCreator::updateButtons() {
    const Section& sect = m_workArea.sectionAt(0);
    for (size_t i = 0; i < m_pointButtons.size(); ++i) {
        m_pointButtons[i] = CircularButton(sect[i].getX(), sect[i].getY(), 5);
    }
}

void ObjectCreator::drawButtons() {
    for (auto& button : m_pointButtons) {
        button.drawLabel(ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR], ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR]);
    }
}

void ObjectCreator::drawLayerView() { // O(layer.size()^2 * adjList.size())   D:<
    const MyVector<MyVector<size_t>>& adjList = m_workMesh->adjacencyList();
    const int workXCenter = (workX0 + workX1) / 2;
    const int workYCenter = (workY0 + workY1) / 2;
    const Point2D& layerCenter = m_layers[m_selectedLayer].center();
    setcolor(ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR]);
    line(workXCenter + layerCenter.getX(), workY0, workXCenter + layerCenter.getX(), workY1);
    line(workX0, workYCenter - layerCenter.getY(), workX1, workYCenter - layerCenter.getY());
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
    for (auto& button : m_layerPointButtons[m_selectedLayer]) {
        button.drawLabel(ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR], ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR]);
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
    m_workArea.render();
    if (m_selectedLayer == -1) {
        m_workArea.setCorners(x0 + 26, y0 + 26, x1 - 200, y1);
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
        m_minimizedSpaceButton.drawLabel(LIGHTGRAY, ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR]);
    }
    else {
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
        drawLayerView();
        setbkcolor(ColorSchemes::themeColors[m_theme][ColorSchemes::PRIMARYCOLOR]);
        setcolor(ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR]);
        outtextxy(0, 3, itoa(m_layers[m_selectedLayer].height(), "Current layer: ").data());
        m_minimizedSpaceButton.drawLabel(LIGHTGRAY, ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR]);
    }
    if (m_hovered != -1) {
        drawPointData();
    }
    drawToolButtons();
    drawSelectLayers();
    swapbuffers();
}

int ObjectCreator::getLayerByHeight(const int& height) {
    for (auto& layer : m_layers) {
        if (layer.height() == height) {
            return &layer - m_layers.begin();
        }
    }
    return -1;
}

void ObjectCreator::editLayer(const int& index) {
    //TODO: change interval
    int btnColor = RGB(160, 160, 160),
        highlightedColor = ColorSchemes::mixColors(btnColor, ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR], 50);
    TextButton& editedLayer = m_layerSelectButtons[index];
    editedLayer.drawLabel(highlightedColor, BLACK);
    TextInputBox txtBox(editedLayer.getXCenter() - 35, editedLayer.getXCenter() + 100, editedLayer.getYCenter(), BLACK, highlightedColor, 4, "-0123456789");
    int oldv = getvisualpage();
    int olda = getactivepage();
    setactivepage(oldv);
    MyArray<char, 256> readInput = txtBox.getText();
    int result = atoi(readInput);
    setvisualpage(oldv);
    setactivepage(olda);
    if (readInput.data() == NULL) {
        return;
    }
    if (result == INT_MIN && result != m_layers[index].height()) {
        return;
    }
    if (result != INT_MIN && result != m_layers[index].height()) {
        if (getLayerByHeight(result) != -1) {
            return;
        }
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
                if (m_layers[i - 1].height() < m_layers[i].height()) {
                    std::swap(m_layers[i - 1], m_layers[i]);
                    std::swap(m_layerAdjListIndex[i - 1], m_layerAdjListIndex[i]);
                    std::swap(m_layerPointButtons[i - 1], m_layerPointButtons[i]);
                    sorted = false;
                }
            }
        } while (!sorted);
        m_selectedLayer = getLayerByHeight(result);
        renderLayerSelectButtons();
        updateButtons();
    }
    m_workArea.update();
    draw();
}

void ObjectCreator::pointAdder(const int& x, const int& y) {
    if (!m_workArea.insideWorkArea(x, y)) {
        return;
    }
    const int xCenter = (x0 + 26 + x1 - 200) / 2;
    const int yCenter = (y0 + 26 + y1) / 2;
    const Point2D layerCenter = m_layers[m_selectedLayer].center();
    m_layers[m_selectedLayer].addPoint(Point2D(x - xCenter, yCenter - y));
    m_workMesh->addPoint(x - xCenter - layerCenter.getX(), yCenter - y - layerCenter.getY(), m_layers[m_selectedLayer].height());
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
    int layerIndex = getLayerByHeight(pntHeight);
    //daca poti gasi mai repede indexul care ne trebuie (instead of a for), go ahead -
    //deocamdata vreau sa fie functional, eficienta o rezolvam noi mai incolo
    for (size_t i = 0; i < m_layers[layerIndex].size(); ++i) {
        if (m_layerAdjListIndex[layerIndex][i] == index) {
            m_pointButtons.erase(m_pointButtons.begin() + index);
            m_workMesh->erase(index);
            m_layers[layerIndex].removePoint(i);
            if (m_layers[layerIndex].empty()) {
                m_layers.erase(m_layers.begin() + layerIndex);
                m_layerSelectButtons.erase(m_layerSelectButtons.begin() + layerIndex);
                m_layerPointButtons.erase(m_layerPointButtons.begin() + layerIndex);
                m_layerAdjListIndex.erase(m_layerAdjListIndex.begin() + layerIndex);
                renderLayerSelectButtons();
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
}

void ObjectCreator::layerViewMover(const int& x_, const int& y_) {
    int x = x_, y = y_;
    int xDrag, yDrag;
    clearmouseclick(WM_LBUTTONUP);
    while (!ismouseclick(WM_LBUTTONUP)) {
        getmouseclick(WM_MOUSEMOVE, xDrag, yDrag);
        if (xDrag != -1 && m_workArea.insideWorkArea(xDrag, yDrag)) {
            m_layers[m_selectedLayer].moveRel(xDrag - x, y - yDrag);
            m_layerPointButtons[m_selectedLayer] = m_layers[m_selectedLayer].renderButtons(x0 + 26, y0 + 26, x1 - 200, y1);
            draw();
            x = xDrag;
            y = yDrag;
        }
    }
}

void ObjectCreator::pointMover(const size_t& index) {
    const int xCenter = (workX0 + workX1) / 2;
    const int yCenter = (workY0 + workY1) / 2;
    size_t adjListIndex = m_layerAdjListIndex[m_selectedLayer][index];
    const Point2D layerCenter = m_layers[m_selectedLayer].center();
    int xDrag, yDrag;
    clearmouseclick(WM_LBUTTONUP);
    while (!ismouseclick(WM_LBUTTONUP)) {
        getmouseclick(WM_MOUSEMOVE, xDrag, yDrag);
        if (xDrag != -1 && m_workArea.insideWorkArea(xDrag, yDrag)) {
            m_layers[m_selectedLayer][index] = Point2D(xDrag - xCenter, yCenter - yDrag);
            m_layers[m_selectedLayer].update(index);
            m_layerPointButtons[m_selectedLayer][index] = CircularButton(xDrag, yDrag, 5);
            (*m_workMesh)[adjListIndex] = Point3D(xDrag - xCenter - layerCenter.getX(), yCenter - yDrag - layerCenter.getY(), m_layers[m_selectedLayer].height());
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

void ObjectCreator::lineCutter3D(const int& x, const int& y){
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
}

void ObjectCreator::lineCutter2D(const int& x, const int& y) {
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
}

void ObjectCreator::toolOperationOnPoint(const size_t& index) {
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
        case Tool::CutLine:
        default:;
    }
}

bool ObjectCreator::getClickCommand() {
    int x, y;
    getmouseclick(WM_LBUTTONDOWN, x, y);
    if (x == -1) {
        //nu prea ne intereseaza outcomeul;
        //oricum se apeleaza callHandlerDrawer
        //in checkCamMovement daca e nevoie
        m_workArea.getKeyCommand();
        return 0;
    }
    //is doar 2 dar e mai cute asa decat 2 ifuri
    for (size_t i = 0; i < m_layerScrollArrows.size(); ++i) {
        if (m_layerScrollArrows[i].hitCollision(x, y)) {
            clearmouseclick(WM_LBUTTONUP);
            while (!ismouseclick(WM_LBUTTONUP)) {
                if (moveLayerSelectsInterval(-1 + 2 * i)) {
                    //am renuntat la ideea de a desena doar layerele;
                    //se glitchuia. rau
                    draw();
                    Sleep(100);
                }
            }
            return true;
        }
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
        if (m_tool == Tool::CutLine && m_workArea.insideWorkArea(x, y)) {
            m_assistLineDotted = true;
            lineCutter2D(x, y);
            return true;
        }
        for (size_t i = 0; i < m_layerPointButtons[m_selectedLayer].size(); ++i) {
            if (m_layerPointButtons[m_selectedLayer][i].hitCollision(x, y)) {
                toolOperationOnPoint(i);
                return true;
            }
        }
        if (m_workArea.insideWorkArea(x, y) && m_tool == Tool::MovePoint) {
            layerViewMover(x, y);
            return false;
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
            //tot ce se poate intampla in workArea este rotirea camerei
            //moment in care se apeleaza callHandlerDrawer
            //unde inclusiv meniul objectcreatorului este apelat
            return false;
        }
        if (m_workArea.insideWorkArea(x, y) && m_tool == Tool::CutLine) {
            m_assistLineDotted = true;
            lineCutter3D(x, y);
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
        if (getClickCommand() || getDoubleClickCommand() || getHoverCommand()) {
            draw();
        }
    }
    return Mesh();
}


Mesh ObjectCreator::generateCube(const unsigned int& length_) {
    int length = length_/2;
    Mesh cube;
    cube.addPoint(-length, -length, -length);
    cube.addPoint(-length, -length, length);
    cube.addPoint(-length, length, -length);
    cube.addPoint(length, -length, -length);
    cube.addPoint(-length, length, length);
    cube.addPoint(length, -length, length);
    cube.addPoint(length, length, -length);
    cube.addPoint(length, length, length);
    cube.addIndexConnections(0, {1, 2, 3});
    cube.addIndexConnections(1, {0, 4, 5});
    cube.addIndexConnections(2, {0, 4, 6});
    cube.addIndexConnections(3, {0, 5, 6});
    cube.addIndexConnections(4, {1, 2, 7});
    cube.addIndexConnections(5, {1, 3, 7});
    cube.addIndexConnections(6, {2, 3, 7});
    return cube;
}

Mesh ObjectCreator::generateCone(const unsigned int& height, const unsigned int& radius, const unsigned int& sides) {
    Mesh cone = Mesh();
    cone.addPoint(0, 0, height / 2);
    Point3D tmp(0, radius, -((int)height / 2));
    for (size_t i = 0; i < sides; ++i) {
        //o sa generam o singura data un corp din asta; eficienta nu e neaparat esentiala
        //decat sa fac cu sin si cos mai bine folosesc ce am deja de la quaternioni
        tmp.rotateByAxisVector(2 * pi / sides, {0, 0, 1});
        cone.addPoint(tmp);
        cone[cone.size() - 1].round();
        cone.addEdge(cone.size() - 1, cone.size() - 2);
        if (i > 0) {
            cone.addEdge(cone.size() - 1, 0);
        }
    }
    cone.addEdge(cone.size() - 1, cone.size() - sides);
    return cone;
}

Mesh ObjectCreator::generateCylinder(const unsigned int& height, const unsigned int& radius, const unsigned int& sides) {
    Mesh cylinder = Mesh();
    Point3D tmp(0, radius, -((int)height / 2));
    for (size_t i = 0; i < sides; ++i) {
        tmp.rotateByAxisVector(2 * pi / sides, {0, 0, 1});
        cylinder.addPoint(tmp);
        cylinder[cylinder.size() - 1].round();
        tmp.setZ(-tmp.getZ());
        cylinder.addPoint(tmp);
        tmp.setZ(-tmp.getZ());
        cylinder[cylinder.size() - 1].round();
        cylinder.addEdge(cylinder.size() - 1, cylinder.size() - 2);
        if (i > 0) {
            cylinder.addEdge(cylinder.size() - 1, cylinder.size() - 3);
            cylinder.addEdge(cylinder.size() - 2, cylinder.size() - 4);
        }
    }
    cylinder.addEdge(cylinder.size() - 2, 0);
    cylinder.addEdge(cylinder.size() - 1, 1);
    return cylinder;
}

Mesh ObjectCreator::generateSphere(const unsigned int& radius, const unsigned int& segments, const unsigned int& rings) {
    //chose UV sphere for clear layer separation -> easier editing in ObjCreator
    //higher density near poles isn't our problem, we aren't even dealing with UVs
    Mesh sphere = Mesh();
    sphere.addPoint(0, 0, radius);
    size_t verticalPredIndex = 0;
    Point3D verticalPred = sphere[verticalPredIndex];
    for (size_t i = 1; i < rings - 1; ++i) {
        Point3D tmp(verticalPred);
        tmp.rotateByAxisVector(pi / (rings - 1), {1, 0, 0});
        verticalPred = tmp;
        tmp.round();
        sphere.addPoint(tmp);
        sphere.addEdge(verticalPredIndex, sphere.size() - 1);
        verticalPredIndex = sphere.size() - 1;
        for (size_t j = 1; j < segments; ++j) {
            Point3D tmp2(tmp);
            tmp2.rotateByAxisVector(2 * pi * j / segments, {0, 0, 1});
            tmp2.round();
            sphere.addPoint(tmp2);
            int sphSize = sphere.size() - 1;
            if (sphSize - (int)segments >= 0) {
                sphere.addEdge(sphSize, sphSize - segments);
            }
            if (j > 0) {
                sphere.addEdge(sphSize, sphSize - 1);
            }
            if (j == segments - 1) {
                sphere.addEdge(sphSize, sphSize - j);
            }
        }
    }
    sphere.addPoint(0, 0, -(int)radius);
    for (size_t i = 1; i <= segments; ++i) {
        sphere.addEdge(0, i);
        sphere.addEdge(sphere.size() - 1, sphere.size() - 1 - i);
    }
    return sphere;
}
