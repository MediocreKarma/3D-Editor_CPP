#include "ObjectCreator.h"
#include <iostream>
#include "MyHashMap.h"

const double pi = 3.1415926535897;

ObjectCreator::ObjectCreator(const Mesh& mesh, const int& theme, int language) :
    m_width(1000), m_height(800), m_theme(theme), m_language(language), m_layers(), m_layerSelectButtons(), m_selectedLayer(), m_addLayerButton(), m_minimizedSpaceButton(), m_toolButtons(),
    m_tool(Tool::MovePoint), workX0(26), workY0(26), workX1(800), workY1(800), m_workArea(theme, mesh, this),
    m_hovered(), m_assistLine(), m_assistLineDotted(false), m_layerScrollArrows(),
    m_layerTools(), m_closeFlag(false), m_discardButton(), m_saveButton(), m_generateButton() {
    toolButtonsInit();
    resetLine();
    m_workArea.setCorners(workX0, workY0, workX1, workY1);
    init();
}

/*ObjectCreator::ObjectCreator(const ObjectCreator& oc) :
    m_theme(oc.m_theme), m_language(oc.m_language), m_layers(oc.m_layers), m_layerSelectButtons(oc.m_layerSelectButtons), m_layerPointButtons(oc.m_layerPointButtons), m_layerAdjListIndex(oc.m_layerAdjListIndex),
    m_addLayerButton(oc.m_addLayerButton), m_pointButtons(oc.m_pointButtons), m_minimizedSpaceButton(oc.m_minimizedSpaceButton), m_toolButtons(oc.m_toolButtons), m_tool(oc.m_tool),
    x0(oc.x0), y0(oc.y0), x1(oc.x1), y1(oc.y1), workX0(oc.workX0), workY0(oc.workY0), workX1(oc.workX1), workY1(oc.workY1), m_workArea(oc.m_workArea), m_workMesh(oc.m_workMesh),
    m_selectedLayer(oc.m_selectedLayer), m_hovered(oc.m_hovered), m_assistLine(oc.m_assistLine), m_assistLineDotted(oc.m_assistLineDotted),
    m_layerSelectsBegin(oc.m_layerSelectsBegin), m_layerSelectsEnd(oc.m_layerSelectsEnd), m_layerScrollArrows(oc.m_layerScrollArrows), m_layerTools(oc.m_layerTools), m_closeFlag(oc.m_closeFlag),
    m_discardButton(oc.m_discardButton), m_saveButton(oc.m_saveButton), m_generateButton(oc.m_generateButton){}

ObjectCreator& ObjectCreator::operator = (const ObjectCreator& oc) {
    m_theme = oc.m_theme;
    m_language = oc.m_language;
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
    m_closeFlag = oc.m_closeFlag;
    init();
    return *this;
}*/

void ObjectCreator::toolButtonsInit() {
    MyArray<MyArray<char, 128>, 5> filenames;
    filenames[(size_t)Tool::NewPoint] = "media\\buttonAddPoint.gif";
    filenames[(size_t)Tool::MovePoint] = "media\\buttonMove.gif";
    filenames[(size_t)Tool::ConnectPoint] = "media\\buttonAddLine.gif";
    filenames[(size_t)Tool::DeletePoint] = "media\\buttonDelete.gif";
    filenames[(size_t)Tool::CutLine] = "media\\buttonLineCutter.gif";
    static const size_t TOOL_BTN_SIZE = 32;
    for (size_t i = 0; i < 5; ++i) {
        m_toolButtons[i] = ImageButton(TOOL_BTN_SIZE / 2, 24 + TOOL_BTN_SIZE / 2 + TOOL_BTN_SIZE * i, TOOL_BTN_SIZE, TOOL_BTN_SIZE, filenames[i].data());
    }
    m_layerSelectButtons.resize((m_height - 200) / 40 + ((m_height - 200) % 40 >= 20 ? 0 : -1));
    for (size_t i = 0; i < m_layerSelectButtons.size(); ++i) {
        m_layerSelectButtons[i].txtButton = TextButton(m_width - 100 - 1, 200 + 20 + i * 40, 200 - 1, 40, "");
    }
    m_discardButton = TextButton(60 / 2, 24 / 2, 60, 24, Language::Text[(int)Lang::Discard][m_language].data());
    m_saveButton = TextButton(60 + 60 / 2, 24 / 2, 60, 24, Language::Text[(int)Lang::Save][m_language].data());
    m_generateButton = DropdownButton<4>(160, 24 / 2, 80, 24, Language::Text[(int)Lang::Generate][m_language].data(), 80, 24 * 4);
    m_generateButton.addOption(Language::Text[(int)Lang::Cube][m_language].data());
    m_generateButton.addOption(Language::Text[(int)Lang::Cone][m_language].data());
    m_generateButton.addOption(Language::Text[(int)Lang::Cylinder][m_language].data());
    m_generateButton.addOption(Language::Text[(int)Lang::Sphere][m_language].data());
    //MyArray<MyArray<char, 128>, 1> layerFilenames;
    //pt cand o sa avem probabil tools precum Duplicate, Duplicate Linked, Draw Circle, Draw Rectangle
    //deocamdata este initializat un MyArray cu 1 imagebutton care nu i folosit la nimic
}

void ObjectCreator::init() {
    const int xCenter = (workX0 + workX1) / 2;
    const int yCenter = (workY0 + workY1) / 2;
    m_workArea.render();
    m_layers.clear();
    for (FixedMesh::iterator_type it = m_workArea.mesh().begin(); it != m_workArea.mesh().end(); ++it) {
        CircularButton layerPointButton = CircularButton(xCenter + it->point.x, yCenter - it->point.y, 5);
        m_layers[it->point.z].data.insert(it, layerPointButton);
    }
    size_t i = 0;
    auto it = m_layers.begin();
    while (i < m_layerSelectButtons.size()) {
        m_layerSelectButtons[i].it = MyMap<int, LayerInfo>::iterator();
        ++i;
    }
    i = 0;
    while (i < m_layers.size() && i < m_layerSelectButtons.size()) {
        m_layerSelectButtons[i].it = it++;
        ++i;
    }
    renderLayerSelectButtons();
    m_minimizedSpaceButton = Button(m_width - 100 - 1, 100 - 1, 200 - 1, 200 - 4);
    m_addLayerButton = Button(m_width - 180, 200 + 20 + m_layerSelectButtons.size() * 40, 40, 40);
    m_layerScrollArrows[0] = Button(m_width - 100, 200 + 20 + m_layerSelectButtons.size() * 40, 40, 40);
    m_layerScrollArrows[1] = Button(m_width - 20, 200 + 20 + m_layerSelectButtons.size() * 40, 40, 40);
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
    m_discardButton.drawTextButton(3, 1, LIGHTGRAY);
    m_saveButton.drawTextButton(3, 1, LIGHTGRAY);
    if (m_generateButton.isListVisible()) {
        m_generateButton.showList(3, 1, LIGHTGRAY, false);
    }
    m_generateButton.drawTextButton(3, 1, LIGHTGRAY);
}

void ObjectCreator::drawSelectLayers() {
    int btnColor = RGB(160, 160, 160),
        highlightedColor = ColorSchemes::mixColors(btnColor, ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR], 50);
    for (size_t i = 0; i < m_layerSelectButtons.size(); ++i) {
        if (m_layerSelectButtons[i].it == m_selectedLayer) {
            m_layerSelectButtons[i].txtButton.drawTextButton(1, 3, highlightedColor, false);
        }
        else {
            m_layerSelectButtons[i].txtButton.drawTextButton(1, 3, btnColor, false);
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
    for (LayerSelectButtonData& data : m_layerSelectButtons) {
        if (!data.it) {
            continue;
        }
        MyArray<char, 32> layerLevel = itoa(data.it->key, Language::Text[(int)Lang::Layer_Z][m_language].data());
        data.txtButton.modifyText(layerLevel.data());
    }
}

//true if layers onscreen have actually changed
bool ObjectCreator::moveLayerSelectsInterval(const int dir) {
    if (m_layers.size() < m_layerSelectButtons.size()) {
        return false;
    }
    if (dir == -1) {
        if (m_layerSelectButtons.front().it == m_layers.begin()) {
            return false;
        }
        for (int i = m_layerSelectButtons.size() - 1; i > 0; --i) {
            m_layerSelectButtons[i].it = m_layerSelectButtons[i - 1].it;
        }
        --m_layerSelectButtons.front().it;
        renderLayerSelectButtons();
        return true;
    }
    else if (dir == 1) {
        auto backIterator = m_layers.end();
        --backIterator;
        if (m_layerSelectButtons.back().it == backIterator) {
            return false;
        }
        for (size_t i = 1; i < m_layerSelectButtons.size(); ++i) {
            m_layerSelectButtons[i - 1].it = m_layerSelectButtons[i].it;
        }
        ++m_layerSelectButtons.back().it;
        renderLayerSelectButtons();
        return true;
    }
    return false;
}

void ObjectCreator::LayerInfo::updateButtons(const int xCenter, const int yCenter) {
    for (auto& info : data) {
        info.value = CircularButton(xCenter + centerPoint.x + info.key->point.x, yCenter - centerPoint.y - info.key->point.y, 5);
    }
}

void ObjectCreator::drawLayerView() {
    const int xCenter = (workX0 + workX1) / 2;
    const int yCenter = (workY0 + workY1) / 2;
    const Point2D layerCenter = m_selectedLayer->value.centerPoint;
    setcolor(ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR]);
    line(xCenter + layerCenter.getX(), workY0, xCenter + layerCenter.getX(), workY1);
    line(workX0, yCenter - layerCenter.getY(), workX1, yCenter - layerCenter.getY());
    setcolor(ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR]);
    for (auto& it : m_selectedLayer->value.data) {
        Point2D P1(it.value.getX(), it.value.getY());
        if (m_workArea.mesh().countConnections(it.key) < m_selectedLayer->value.data.size()) {
            for (FixedMesh::iterator_type it2 : m_workArea.mesh().adjacentPoints(it.key)) {
                if (it2->point.z != m_selectedLayer->key) {
                    continue;
                }
                Point2D P2(m_selectedLayer->value.data[it2].getX(), m_selectedLayer->value.data[it2].getY());
                line(P1.x, P1.y, P2.x, P2.y);
            }
        }
        else {
            for (auto& node : m_selectedLayer->value.data) {
                FixedMesh::iterator_type it2 = node.key;
                if (!m_workArea.mesh().adjacentPoints(it.key).contains(it2)) {
                    continue;
                }
                Point2D P2(m_selectedLayer->value.data[it2].getX(), m_selectedLayer->value.data[it2].getY());
                line(P1.x, P1.y, P2.x, P2.y);
            }
        }
    }
    for (auto& button : m_selectedLayer->value.data) {
        button.value.drawLabel(ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR],
                                ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR]);
    }
}

void ObjectCreator::drawPointData() {
    const IntegerPoint3D& p = m_hovered->point;
    setcolor(ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR]);
    rectangle(730, 30, 790, 120);
    setbkcolor(ColorSchemes::themeColors[m_theme][ColorSchemes::PRIMARYCOLOR]);
    MyArray<char, 32> headerText = Language::Text[(int)Lang::Point][m_language],
                        xPosText = itoa(p.x, "X: "),
                        yPosText = itoa(p.y, "Y: "),
                        zPosText = itoa(p.z, "Z: ");
    outtextxy(735, 35, headerText.data());
    outtextxy(735, 55, xPosText.data());
    outtextxy(735, 75, yPosText.data());
    outtextxy(735, 95, zPosText.data());
}

void ObjectCreator::draw() {
    setfillstyle(SOLID_FILL, ColorSchemes::themeColors[m_theme][ColorSchemes::PRIMARYCOLOR]);
    bar(0, 0, m_width, m_height);
    m_workArea.render();
    if (!m_selectedLayer) {
        m_workArea.draw();
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
        outtextxy(0, 3, itoa(m_selectedLayer->key, Language::Text[(int)Lang::Current_Layer][m_language].data()).data());
        m_minimizedSpaceButton.drawLabel(LIGHTGRAY, ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR]);
    }
    if (m_hovered) {
        drawPointData();
    }
    drawToolButtons();
    drawSelectLayers();
    swapbuffers();
}

void ObjectCreator::centerLayerButton() {
    //of
    //poftim un placeholder
    size_t i = 0;
    auto it = m_layers.begin();
    while (i < m_layers.size() && i < m_layerSelectButtons.size()) {
        m_layerSelectButtons[i].it = it++;
        ++i;
    }
}

void ObjectCreator::mergeLayers(MyMap<int, LayerInfo>::iterator moving, MyMap<int, LayerInfo>::iterator destination) {
    const int xCenter = (workX0 + workX1) / 2;
    const int yCenter = (workY0 + workY1) / 2;
    for (auto& dataNode : moving->value.data) {
        FixedMesh::iterator_type it = dataNode.key;
        IntegerPoint3D newPoint = it->point;
        newPoint.z = destination->key;
        if (!m_workArea.mesh().contains(newPoint)) {
            m_workArea.mesh().updatePointValue(it, newPoint);
            CircularButton pntBtn(xCenter + it->point.x, yCenter - it->point.y, 5);
            m_layers[destination->key].data.insert(it, pntBtn);
        }
        else {
            m_workArea.mesh().erasePoint(it);
        }
    }
    m_layers.erase(moving);
    m_selectedLayer = m_layers.find(destination->key);
    centerLayerButton();
    renderLayerSelectButtons();
}

void ObjectCreator::editLayer(const int layerIndex) {
    int btnColor = RGB(160, 160, 160),
        highlightedColor = ColorSchemes::mixColors(btnColor, ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR], 50);
    TextButton& editedLayer = m_layerSelectButtons[layerIndex].txtButton;
    m_selectedLayer = m_layerSelectButtons[layerIndex].it;
    draw();
    setactivepage(getvisualpage());
    NumericInputBox txtBox(editedLayer.getXCenter() - 35, editedLayer.getXCenter() + 90, editedLayer.getYCenter(),
                            BLACK, highlightedColor);
    int result = txtBox.getIntegerValue();
    setvisualpage(1 - getactivepage());
    if (result == m_selectedLayer->key) {
        return;
    }
    if (m_layers.contains(result)) {
        mergeLayers(m_selectedLayer, m_layers.find(result));
        int x, y;
        txtBox.getClick(x, y);
        if (x != -1) {
            getClickCommand(x, y);
        }
        draw();
        return;
    }
    for (auto& dataNode : m_selectedLayer->value.data) {
        FixedMesh::iterator_type it = dataNode.key;
        IntegerPoint3D newPoint = it->point;
        newPoint.z = result;
        m_workArea.mesh().updatePointValue(it, newPoint);
    }
    m_layers[result] = m_selectedLayer->value;
    m_layers.erase(m_selectedLayer);
    m_selectedLayer = m_layers.find(result);
    centerLayerButton();
    renderLayerSelectButtons();
    int x, y;
    txtBox.getClick(x, y);
    if (x != -1) {
        getClickCommand(x, y);
    }
    draw();
}

void ObjectCreator::pointAdder(const int& x, const int& y) {
    if (!m_workArea.insideWorkArea(x, y)) {
        return;
    }
    const int xCenter = (workX0 + workX1) / 2;
    const int yCenter = (workY0 + workY1) / 2;
    const Point2D layerCenter = m_selectedLayer->value.centerPoint;
    IntegerPoint3D newPoint(x - xCenter - layerCenter.x, yCenter - y - layerCenter.y, m_selectedLayer->key);
    m_workArea.mesh().addPoint(newPoint);
    m_hovered = m_workArea.mesh().begin();
    m_selectedLayer->value.data.insert(m_hovered, CircularButton(x, y, 5));
}

void ObjectCreator::pointDeleter() {
    m_selectedLayer->value.data.erase(m_hovered);
    m_workArea.mesh().erasePoint(m_hovered);
    m_hovered = FixedMesh::iterator_type();
}

void ObjectCreator::pointDeleter3D() {
    m_layers[m_hovered->point.z].data.erase(m_hovered);
    m_workArea.mesh().erasePoint(m_hovered);
    m_hovered = FixedMesh::iterator_type();
}

void ObjectCreator::layerViewMover(int x, int y) {
    const int xCenter = (workX0 + workX1) / 2;
    const int yCenter = (workY0 + workY1) / 2;
    int xDrag, yDrag;
    clearmouseclick(WM_LBUTTONUP);
    while (!ismouseclick(WM_LBUTTONUP)) {
        getmouseclick(WM_MOUSEMOVE, xDrag, yDrag);
        if (xDrag != -1 && m_workArea.insideWorkArea(xDrag, yDrag)) {
            m_selectedLayer->value.centerPoint += Point2D(xDrag - x, y - yDrag);
            m_selectedLayer->value.updateButtons(xCenter, yCenter);
            draw();
            x = xDrag;
            y = yDrag;
        }
    }
}

void ObjectCreator::pointMover() {
    const int xCenter = (workX0 + workX1) / 2;
    const int yCenter = (workY0 + workY1) / 2;
    const Point2D cPoint = m_selectedLayer->value.centerPoint;
    int xDrag, yDrag;
    clearmouseclick(WM_LBUTTONUP);
    while (!ismouseclick(WM_LBUTTONUP)) {
        getmouseclick(WM_MOUSEMOVE, xDrag, yDrag);
        if (m_workArea.insideWorkArea(xDrag, yDrag)) {
            m_workArea.mesh().updatePointValue(m_hovered,
                            IntegerPoint3D(xDrag - xCenter - cPoint.x, yCenter - yDrag - cPoint.y, m_hovered->point.z));
            m_selectedLayer->value.data[m_hovered] = CircularButton(xDrag, yDrag, 5);
            draw();
        }
    }
}

void ObjectCreator::pointConnector2D() {
    const int xCenter = (workX0 + workX1) / 2;
    const int yCenter = (workY0 + workY1) / 2;
    setcolor(ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR]);
    FixedMesh::iterator_type firstPoint = m_hovered;
    int xFirstPoint = firstPoint->point.x;
    int yFirstPoint = firstPoint->point.y;
    clearmouseclick(WM_LBUTTONUP);
    int xDrag, yDrag;
    while (!ismouseclick(WM_LBUTTONUP)) {
        getmouseclick(WM_MOUSEMOVE, xDrag, yDrag);
        if (!m_workArea.insideWorkArea(xDrag, yDrag)) {
            continue;
        }
        m_assistLine = Line2D(xCenter + xFirstPoint, yCenter - yFirstPoint, xDrag, yDrag);
        getHoverCommand(xDrag, yDrag);
        draw();
    }
    resetLine();
    getmouseclick(WM_LBUTTONUP, xDrag, yDrag);
    getHoverCommand(xDrag, yDrag);
    if (m_hovered) {
        m_workArea.mesh().addEdge(firstPoint, m_hovered);
    }
}

void ObjectCreator::pointConnector3D() {
    setcolor(ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR]);
    FixedMesh::iterator_type firstPoint = m_hovered;
    int xFirstPoint = firstPoint->button2d.getX();
    int yFirstPoint = firstPoint->button2d.getY();
    clearmouseclick(WM_LBUTTONUP);
    int xDrag, yDrag;
    while (!ismouseclick(WM_LBUTTONUP)) {
        getmouseclick(WM_MOUSEMOVE, xDrag, yDrag);
        if (!m_workArea.insideWorkArea(xDrag, yDrag)) {
            continue;
        }
        m_assistLine = Line2D(xFirstPoint, yFirstPoint, xDrag, yDrag);
        getHoverCommand(xDrag, yDrag);
        draw();
    }
    resetLine();
    getmouseclick(WM_LBUTTONUP, xDrag, yDrag);
    getHoverCommand(xDrag, yDrag);
    if (m_hovered) {
        m_workArea.mesh().addEdge(firstPoint, m_hovered);
    }
}

void ObjectCreator::lineCutter3D(const int& x, const int& y){
    clearmouseclick(WM_LBUTTONUP);
    int xDrag, yDrag;
    while (!ismouseclick(WM_LBUTTONUP)) {
        getmouseclick(WM_MOUSEMOVE, xDrag, yDrag);
        if (!m_workArea.insideWorkArea(xDrag, yDrag)) {
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
    Point2D p1(x, y), p2(xDrag, yDrag);
    if (m_workArea.mesh().cutLines(Line2D(p1, p2))) {
        m_workArea.render();
    }
}

void ObjectCreator::lineCutter2D(const int& x, const int& y) {
    clearmouseclick(WM_LBUTTONUP);
    int xDrag, yDrag;
    while (!ismouseclick(WM_LBUTTONUP)) {
        getmouseclick(WM_MOUSEMOVE, xDrag, yDrag);
        if (!m_workArea.insideWorkArea(xDrag, yDrag)) {
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
    for (auto& node : m_selectedLayer->value.data) {
        FixedMesh::iterator_type it = node.key;
        MyList<FixedMesh::iterator_type> deleteConnections;
        Point2D P1(m_selectedLayer->value.data[it].getX(), m_selectedLayer->value.data[it].getY());
        if (m_workArea.mesh().countConnections(it) < m_selectedLayer->value.data.size()) {
            for (FixedMesh::iterator_type it2 : m_workArea.mesh().adjacentPoints(it)) {
                if (it2->point.z != m_selectedLayer->key) {
                    continue;
                }
                Point2D P2(m_selectedLayer->value.data[it2].getX(), m_selectedLayer->value.data[it2].getY());
                if (linesIntersect(p1, p2, P1, P2)) {
                    deleteConnections.push_back(it2);
                    hasDeletedLine = true;
                }
            }
        }
        else {
            for (auto& node2 : m_selectedLayer->value.data) {
                FixedMesh::iterator_type it2 = node2.key;
                if (!m_workArea.mesh().adjacentPoints(it).contains(it2) && !m_workArea.mesh().adjacentPoints(it2).contains(it)) {
                    continue;
                }
                Point2D P2(m_selectedLayer->value.data[it2].getX(), m_selectedLayer->value.data[it2].getY());
                if (linesIntersect(p1, p2, P1, P2)) {
                    deleteConnections.push_back(it2);
                    hasDeletedLine = true;
                }
            }
        }
        for (FixedMesh::iterator_type it2 : deleteConnections) {
            m_workArea.mesh().eraseConnection(it, it2);
        }
    }
    if (hasDeletedLine) {
        m_workArea.render();
    }
}

void ObjectCreator::toolOperationOnPoint() {
    switch (m_tool) {
        case Tool::MovePoint:
            pointMover();
            break;
        case Tool::ConnectPoint:
            m_assistLineDotted = false;
            pointConnector2D();
            break;
        case Tool::DeletePoint:
            pointDeleter();
        case Tool::NewPoint:
        case Tool::CutLine:
        default:;
    }
}


bool ObjectCreator::getClickCommand() {
    int x, y;
    getmouseclick(WM_LBUTTONDOWN, x, y);
    return getClickCommand(x, y);
}

bool ObjectCreator::getClickCommand(const int x, const int y) {
    if (x == -1) {
        return m_workArea.getKeyCommand();
    }
    if (m_generateButton.isListVisible()) {
        if (m_generateButton.listHitCollision(x, y) > -1) {
            bool index = m_generateButton.listHitCollision(x, y);
            m_generateButton.setVisibility(false);
            return generate(index);
        }
    }
    if (m_generateButton.hitCollision(x, y)) {
        bool visible = m_generateButton.isListVisible();
        m_generateButton.setVisibility(!visible);
        return true;
    }
    for (size_t i = 0; i < m_layerScrollArrows.size(); ++i) {
        if (m_layerScrollArrows[i].hitCollision(x, y)) {
            clearmouseclick(WM_LBUTTONUP);
            while (!ismouseclick(WM_LBUTTONUP)) {
                if (moveLayerSelectsInterval(-1 + 2 * i)) {
                    draw();
                }
            }
            return true;
        }
    }
    if (m_discardButton.hitCollision(x, y)) {
        m_closeFlag = 1;
        return false;
    }
    if (m_saveButton.hitCollision(x, y)) {
        m_closeFlag = 2;
        return false;
    }
    for (size_t i = 0; i < m_toolButtons.size(); ++i) {
        if (m_toolButtons[i].hitCollision(x, y)) {
            m_tool = (Tool)i;
        }
    }
    for (size_t i = 0; i < m_layerSelectButtons.size(); ++i) {
        if (m_layerSelectButtons[i].txtButton.hitCollision(x, y)) {
            m_selectedLayer = m_layerSelectButtons[i].it;
            return true;
        }
    }
    if (m_selectedLayer) {
        if (m_minimizedSpaceButton.hitCollision(x, y)) {
            m_selectedLayer = MyMap<int, LayerInfo>::iterator();
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
        for (const auto& node : m_selectedLayer->value.data) {
            if (node.value.hitCollision(x, y)) {
                m_hovered = node.key;
                toolOperationOnPoint();
                return true;
            }
        }
        if (m_workArea.insideWorkArea(x, y) && m_tool == Tool::MovePoint) {
            layerViewMover(x, y);
            return false;
        }
    }
    else {
        for (FixedMesh::iterator_type it = m_workArea.mesh().begin(); it != m_workArea.mesh().end(); ++it) {
            if (it->button2d.hitCollision(x, y)) {
                m_hovered = it;
                if (m_tool == Tool::ConnectPoint) {
                    m_assistLineDotted = false;
                    pointConnector3D();
                    return true;
                }
                else if(m_tool == Tool::DeletePoint) {
                    pointDeleter3D();
                    return true;
                }
            }
        }
        if (m_workArea.getCommand(x, y)) {
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
    if (!m_selectedLayer) {
        for (FixedMesh::iterator_type it = m_workArea.mesh().begin(); it != m_workArea.mesh().end(); ++it) {
            if (it->button2d.hitCollision(xHov, yHov)) {
                m_hovered = it;
                return true;
            }
        }
    }
    else {
        for (const auto& node : m_selectedLayer->value.data) {
            if (node.value.hitCollision(xHov, yHov)) {
                m_hovered = node.key;
                return true;
            }
        }
    }
    if (m_hovered) {
        m_hovered = nullptr;
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
        if (m_layerSelectButtons[i].txtButton.hitCollision(x, y)) {
            editLayer(i);
            return true;
        }
    }
    return false;
}

Mesh ObjectCreator::run() {
    initwindow(m_width, m_height, "Object creator", 300, 25, false, false);
    setvisualpage(0);
    setactivepage(1);
    m_closeFlag = false;
    draw();
    while (true) {
        if (getClickCommand() || getDoubleClickCommand() || getHoverCommand()) {
            draw();
        }
        if (m_closeFlag) {
            break;
        }
    }
    closegraph(getcurrentwindow());
    return Mesh(m_workArea.mesh());
}

const int& ObjectCreator::getCloseFlag() const {
    return m_closeFlag;
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
    //mi ai zis prea tarziu de fete ca sa mai facem si asta:))))
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

bool ObjectCreator::generate(int index) {
    int getCurrentWindowNumber = getcurrentwindow();
    int helpWindow = initwindow(300, 200, Language::Text[(int)Lang::Generate][m_language].data(), getmaxwidth() / 2 - 150, getmaxheight() / 2 - 100, false, false);
    setcurrentwindow(helpWindow);
    setbkcolor(WHITE);
    bar(0, 0, 800, 600);
    setcolor(BLACK);
    outtextxy(0, 35, MyArray<char, 32>("deocamdata doar genereaza o sfera intr un if").data());
    outtextxy(0, 60, MyArray<char, 32>("gasesti mai jos in cod").data());
    TextButton backButton(45, 150, 60, 30, Language::Text[(int)Lang::Discard][m_language].data());
    backButton.drawTextButton(0, 0, LIGHTGRAY);
    TextButton generateButton(105, 150, 60, 30, Language::Text[(int)Lang::Generate][m_language].data());
    generateButton.drawTextButton(0, 0, LIGHTGRAY);
    int saveFlag = 0;
    while (true) {
        if (kbhit()) {
            char c = getch();
            if (c == 27) {
                break;
            }
        }
        else if(ismouseclick(WM_LBUTTONDOWN)) {
            int x_, y_;
            getmouseclick(WM_LBUTTONDOWN, x_, y_);
            if (backButton.hitCollision(x_, y_)) {
                break;
            }
            else if(generateButton.hitCollision(x_, y_)) {
                saveFlag = 1;
                break;
            }
        }
    }

    if (saveFlag) {
        Mesh tmp = generateSphere(100, 10, 10);
        /*daca vrei sa verifici cum arata meshu Mesh generat
        std::cout<<"\n\nCONNECTIONS in MESH\n";
        for(size_t i = 0; i < tmp.size(); ++i) {
            tmp[i].display();
            std::cout<<" - ";
            for (auto& pnt : tmp.adjListAt(i)) {
                tmp[pnt].display(false);
                std::cout<<"; ";
            }
            std::cout<<"\n\n";
        }*/
        m_workArea.mesh() = tmp;
        toolButtonsInit();
        init();
        renderLayerSelectButtons();
        resetLine();
    }
    closegraph(helpWindow);
    setcurrentwindow(getCurrentWindowNumber);
    return true;
}
