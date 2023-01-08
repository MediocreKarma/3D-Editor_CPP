#include "ObjectCreator.h"
#include <iostream>

const double pi = 3.1415926535897;

ObjectCreator::ObjectCreator(const Mesh& mesh, const int& theme, int language) :
    m_width(1000), m_height(800), m_theme(theme), m_language(language), m_layers(), m_layerSelectButtons(), m_selectedLayer(), m_addLayerButton(), m_deleteLayerButton(), m_dupeLayerButton(),
    m_minimizedSpaceButton(), m_toolButtons(),
    m_tool(Tool::MovePoint), workX0(32), workY0(32), workX1(800), workY1(800), m_workArea(theme, mesh, this),
    m_hovered(), m_assistLine(), m_assistLineDotted(false), m_layerScrollArrows(),
    m_layerTools(), m_closeFlag(false), m_discardButton(), m_saveButton(), m_generateButton(), m_pointDataButton() {
    toolButtonsInit();
    resetLine();
    m_workArea.setCorners(workX0, workY0, workX1, workY1);
    init();
}

void ObjectCreator::toolButtonsInit() {
    static const size_t TOOL_BTN_SIZE = 32;
    for (size_t i = 0; i < m_toolButtons.size(); ++i) {
        m_toolButtons[i] = Button(TOOL_BTN_SIZE / 2, 24 + TOOL_BTN_SIZE / 2 + TOOL_BTN_SIZE * i, TOOL_BTN_SIZE, TOOL_BTN_SIZE);
    }
    m_layerSelectButtons.resize((m_height - 200) / 40 + ((m_height - 200) % 40 >= 20 ? 0 : -1));
    for (size_t i = 0; i < m_layerSelectButtons.size(); ++i) {
        m_layerSelectButtons[i].txtButton = TextButton(m_width - 100 - 1, 200 + 20 + i * 40, 200 - 1, 40, "");
    }
    m_discardButton = TextButton(60 / 2, 24 / 2, 60, 24, Language::Text[(int)Lang::Discard][m_language].data());
    m_saveButton = TextButton(60 + 60 / 2, 24 / 2, 60, 24, Language::Text[(int)Lang::Save][m_language].data());
    m_generateButton = DropdownButton<4>(120 + 35, 24 / 2, 70, 24, Language::Text[(int)Lang::Generate][m_language].data(), 70, 24 * 4);
    m_generateButton.addOption(Language::Text[(int)Lang::Cube][m_language].data());
    m_generateButton.addOption(Language::Text[(int)Lang::Cone][m_language].data());
    m_generateButton.addOption(Language::Text[(int)Lang::Cylinder][m_language].data());
    m_generateButton.addOption(Language::Text[(int)Lang::Sphere][m_language].data());
}

void ObjectCreator::drawMinimizedSpaceCube() {
    setlinestyle(SOLID_LINE, 0, 1);
    int btnOffsetX = m_minimizedSpaceButton.getXCenter() - m_minimizedSpaceButton.getXLen() / 2 - 3;
    int btnOffsetY = m_minimizedSpaceButton.getYCenter() - m_minimizedSpaceButton.getYLen() / 2 - 14;
    line(38 + btnOffsetX, 162 + btnOffsetY, 35 + btnOffsetX, 71 + btnOffsetY);
    line(38 + btnOffsetX, 162 + btnOffsetY, 87 + btnOffsetX, 134 + btnOffsetY);
    line(38 + btnOffsetX, 162 + btnOffsetY, 120 + btnOffsetX, 182 + btnOffsetY);
    line(35 + btnOffsetX, 71 + btnOffsetY, 86 + btnOffsetX, 52 + btnOffsetY);
    line(35 + btnOffsetX, 71 + btnOffsetY, 121 + btnOffsetX, 85 + btnOffsetY);
    line(87 + btnOffsetX, 134 + btnOffsetY, 86 + btnOffsetX, 52 + btnOffsetY);
    line(87 + btnOffsetX, 134 + btnOffsetY, 163 + btnOffsetX, 150 + btnOffsetY);
    line(120 + btnOffsetX, 182 + btnOffsetY, 121 + btnOffsetX, 85 + btnOffsetY);
    line(120 + btnOffsetX, 182 + btnOffsetY, 163 + btnOffsetX, 150 + btnOffsetY);
    line(86 + btnOffsetX, 52 + btnOffsetY, 166 + btnOffsetX, 63 + btnOffsetY);
    line(121 + btnOffsetX, 85 + btnOffsetY, 166 + btnOffsetX, 63 + btnOffsetY);
    line(163 + btnOffsetX, 150 + btnOffsetY, 166 + btnOffsetX, 63 + btnOffsetY);
}

void ObjectCreator::drawToolButtonSymbol(const int xCenter, const int yCenter, const int btnWidth, const int toolIndex) {
    int btnTrueWidth = btnWidth / 2 - 8;
    int arrowHeadWidth = 2;
    int oldColor = getcolor();
    linesettingstype lineinfo;
    getlinesettings(&lineinfo);
    Tool index = (Tool)toolIndex;
    switch(index) {
        case Tool::NewPoint:
            setlinestyle(SOLID_LINE, 0, 4);
            setcolor(BLACK);
            line(xCenter - btnTrueWidth, yCenter, xCenter + btnTrueWidth, yCenter);
            line(xCenter, yCenter - btnTrueWidth, xCenter, yCenter + btnTrueWidth);
            break;
        case Tool::ConnectPoint: {
            setlinestyle(SOLID_LINE, 0, 4);
            setcolor(BLACK);
            setfillstyle(SOLID_FILL, BLACK);
            int x0 = xCenter - btnTrueWidth;
            int y0 = yCenter - btnTrueWidth;
            circle(x0, y0, 3);
            int x1 = xCenter + btnTrueWidth;
            int y1 = yCenter + btnTrueWidth;
            circle(x1, y1, 3);
            setlinestyle(SOLID_LINE, 0, 2);
            line(x0, y0, x1, y1);
            break;
        }
        case Tool::DeletePoint:
            setlinestyle(SOLID_LINE, 0, 4);
            setcolor(BLACK);
            line(xCenter - btnTrueWidth, yCenter - btnTrueWidth, xCenter + btnTrueWidth, yCenter + btnTrueWidth);
            line(xCenter + btnTrueWidth, yCenter - btnTrueWidth, xCenter - btnTrueWidth, yCenter + btnTrueWidth);
            break;
        case Tool::MovePoint:
            setlinestyle(SOLID_LINE, 0, 3);
            setcolor(BLACK);
            line(xCenter - btnTrueWidth, yCenter, xCenter + btnTrueWidth, yCenter);
            line(xCenter, yCenter - btnTrueWidth, xCenter, yCenter + btnTrueWidth);
            setlinestyle(SOLID_LINE, 0, 3);
            line(xCenter - arrowHeadWidth, yCenter - btnTrueWidth, xCenter, yCenter - btnTrueWidth - arrowHeadWidth);
            line(xCenter + arrowHeadWidth, yCenter - btnTrueWidth, xCenter, yCenter - btnTrueWidth - arrowHeadWidth);
            line(xCenter - arrowHeadWidth, yCenter + btnTrueWidth, xCenter, yCenter + btnTrueWidth + arrowHeadWidth);
            line(xCenter + arrowHeadWidth, yCenter + btnTrueWidth, xCenter, yCenter + btnTrueWidth + arrowHeadWidth);
            line(xCenter + btnTrueWidth, yCenter + arrowHeadWidth, xCenter + btnTrueWidth + arrowHeadWidth, yCenter);
            line(xCenter + btnTrueWidth, yCenter - arrowHeadWidth, xCenter + btnTrueWidth + arrowHeadWidth, yCenter);
            line(xCenter - btnTrueWidth, yCenter - arrowHeadWidth , xCenter - btnTrueWidth - arrowHeadWidth, yCenter);
            line(xCenter - btnTrueWidth, yCenter + arrowHeadWidth , xCenter - btnTrueWidth - arrowHeadWidth, yCenter);
            break;
        case Tool::CutLine: {
            setlinestyle(SOLID_LINE, 0, 4);
            setcolor(BLACK);
            setfillstyle(SOLID_FILL, BLACK);
            int x0 = xCenter - btnTrueWidth;
            int y0 = yCenter - btnTrueWidth;
            circle(x0, y0, 3);
            int x1 = xCenter + btnTrueWidth;
            int y1 = yCenter + btnTrueWidth;
            circle(x1, y1, 3);
            setlinestyle(SOLID_LINE, 0, 2);
            line(x0, y0, x1, y1);
            setlinestyle(DOTTED_LINE, 0, 2);
            line(x1, y0, x0, y1);
            break;
        }
        case Tool::EditPoint: {
            setcolor(BLACK);
            setlinestyle(SOLID_LINE, 0, 2);
            int pencilWidth = 2;
            int x0 = xCenter - btnTrueWidth;
            int y0 = yCenter - btnTrueWidth;
            int x1 = xCenter + btnTrueWidth;
            int y1 = yCenter + btnTrueWidth;
            line(x1 + pencilWidth, y0 + pencilWidth, x1 - pencilWidth, y0 - pencilWidth);
            line(x1 - pencilWidth, y0 - pencilWidth, x0 - pencilWidth + 3, y1 - pencilWidth - 3);
            line(x0 - pencilWidth + 3, y1 - pencilWidth - 3, x0 + pencilWidth + 3, y1 + pencilWidth - 3); //bottom line
            line(x0 + pencilWidth + 3, y1 + pencilWidth - 3, x1 + pencilWidth, y0 + pencilWidth);
            line(x0 + pencilWidth + 3, y1 + pencilWidth - 3, x0 - pencilWidth - 3 + 3, y1 + pencilWidth + 3 - 3); //varf
            line(x0 - pencilWidth - 3 + 3, y1 + pencilWidth + 3 - 3, x0 - pencilWidth + 3, y1 - pencilWidth - 3); //varf
        }
        default:;
    }
    setlinestyle(lineinfo.linestyle, lineinfo.upattern, lineinfo.thickness);
    setcolor(oldColor);
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
        m_layerSelectButtons[i].it = it++;
        ++i;
    }
    renderLayerSelectButtons();
    m_minimizedSpaceButton = Button(m_width - 100 - 1, 100 - 1, 200 - 1, 200 - 1);
    m_addLayerButton = Button(m_width - 180, 200 + 20 + m_layerSelectButtons.size() * 40, 40, 40);
    m_deleteLayerButton = Button(m_width - 140, 200 + 20 + m_layerSelectButtons.size() * 40, 40, 40);
    m_dupeLayerButton = Button(m_width - 100, 200 + 20 + m_layerSelectButtons.size() * 40, 40, 40);
    m_layerScrollArrows[0] = Button(m_width - 60, 200 + 20 + m_layerSelectButtons.size() * 40, 40, 40);
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
        int index = &button - m_toolButtons.begin();
        if (m_tool == (Tool)index) {
            button.drawLabel(ColorSchemes::mixColors(WHITE, ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR], 75), BLACK);
        }
        else {
            button.drawLabel(ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR], BLACK);
        }
        drawToolButtonSymbol(button.getXCenter(), button.getYCenter(), button.getXLen(), index);
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
    for (size_t i = 0; i < m_layerSelectButtons.size() && i < m_layers.size(); ++i) {
        if (m_layerSelectButtons[i].it == m_selectedLayer) {
            m_layerSelectButtons[i].txtButton.drawTextButton(1, 3, highlightedColor, false);
        }
        else {
            m_layerSelectButtons[i].txtButton.drawTextButton(1, 3, btnColor, false);
        }
    }
    setcolor(ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR]);
    setlinestyle(SOLID_LINE, 0, 2);
    setbkcolor(LIGHTGRAY);
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    int x = m_addLayerButton.getXCenter();
    int y = m_addLayerButton.getYCenter();
    static const int HALF_BTN_SIZE = 15;
    line(x - HALF_BTN_SIZE, y, x + HALF_BTN_SIZE, y);
    line(x, y - HALF_BTN_SIZE, x, y + HALF_BTN_SIZE);
    x = m_deleteLayerButton.getXCenter();
    line(x - HALF_BTN_SIZE, y, x + HALF_BTN_SIZE, y);
    x = m_dupeLayerButton.getXCenter();
    const int offset1 = -2, offset2 = 4;
    rectangle(x - HALF_BTN_SIZE / 2 + offset1, y - HALF_BTN_SIZE / 2 - offset1, x + HALF_BTN_SIZE / 2 + offset1, y + HALF_BTN_SIZE / 2 - offset1);
    rectangle(x - HALF_BTN_SIZE / 2 + offset2, y - HALF_BTN_SIZE / 2 - offset2, x + HALF_BTN_SIZE / 2 + offset2, y + HALF_BTN_SIZE / 2 - offset2);
    bar(x - HALF_BTN_SIZE / 2 + offset1 + 1, y - HALF_BTN_SIZE / 2 - offset1 + 1, x + HALF_BTN_SIZE / 2 + offset1 - 1, y + HALF_BTN_SIZE / 2 - offset1 - 1);
    x = m_layerScrollArrows[0].getXCenter();
    line(x - HALF_BTN_SIZE, y + HALF_BTN_SIZE / 2, x, y - HALF_BTN_SIZE / 2);
    line(x + HALF_BTN_SIZE, y + HALF_BTN_SIZE / 2, x, y - HALF_BTN_SIZE / 2);
    x = m_layerScrollArrows[1].getXCenter();
    line(x - HALF_BTN_SIZE, y - HALF_BTN_SIZE / 2, x, y + HALF_BTN_SIZE / 2);
    line(x + HALF_BTN_SIZE, y - HALF_BTN_SIZE / 2, x, y + HALF_BTN_SIZE / 2);
}

void ObjectCreator::renderLayerSelectButtons() {
    for (LayerSelectButtonData& data : m_layerSelectButtons) {
        if (!data.it) {
            return;
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
    MyArray<char, 32> headerText;
    setbkcolor(ColorSchemes::themeColors[m_theme][ColorSchemes::PRIMARYCOLOR]);
    setfillstyle(SOLID_FILL, ColorSchemes::themeColors[m_theme][ColorSchemes::PRIMARYCOLOR]);
    if (m_tool != Tool::EditPoint) {
        headerText = Language::Text[(int)Lang::Point][m_language];
        m_pointDataButton = Button(760, 75, 60, 90);
    }
    else {
        headerText = Language::Text[(int)Lang::Edit_Point][m_language];
        MyArray<char, 32> langText = "Editeaza punct:";//the wider one
        m_pointDataButton = Button(730, 75, textwidth(langText.data()) + 10, 90);
    }
    m_pointDataButton.drawLabel(ColorSchemes::themeColors[m_theme][ColorSchemes::PRIMARYCOLOR],
                                ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR]);
    MyArray<char, 32> xPosText = itoa(p.x, "X: "),
                      yPosText = itoa(p.y, "Y: "),
                      zPosText = itoa(p.z, "Z: ");
    int x0 = m_pointDataButton.getXCenter() - m_pointDataButton.getXLen() / 2 + 5;
    int y0 = m_pointDataButton.getYCenter() - m_pointDataButton.getYLen() / 2 + 5;
    int offset = 20;
    setbkcolor(ColorSchemes::themeColors[m_theme][ColorSchemes::PRIMARYCOLOR]);
    outtextxy(x0, y0, headerText.data());
    outtextxy(x0, y0 + offset, xPosText.data());
    outtextxy(x0, y0 + 2 * offset, yPosText.data());
    outtextxy(x0, y0 + 3 * offset, zPosText.data());
}

bool ObjectCreator::changePointData(const int x, const int y) {
    const int xCenter = (workX0 + workX1) / 2;
    const int yCenter = (workY0 + workY1) / 2;
    IntegerPoint3D aux = m_hovered->point;
    MyArray<int, 3> coords = {aux.x, aux.y, aux.z};
    MyArray<char, 32> zText = "Z: "; //widest one: if we accommodate for this one it'll be enough
    int x0 = m_pointDataButton.getXCenter() - m_pointDataButton.getXLen() / 2 + 5;
    int y0 = m_pointDataButton.getYCenter() - m_pointDataButton.getYLen() / 2 + 5;
    int x1 = m_pointDataButton.getXCenter() + m_pointDataButton.getXLen() / 2 - 5;
    int offset = 20;
    for (int i = 0; i < 3; ++i) {
        if (x >= x0 && y >= y0 + (i + 1) * offset && x <= x1 && y <= y0 + (i + 2) * offset) {
            setactivepage(getvisualpage());
            setcolor(LIGHTBLUE);
            NumericInputBox txtBox(x0 + textwidth(zText.data()),
                                   x1, y0 + (i + 1) * offset - 5 + 12,
                                   ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR],
                                   ColorSchemes::themeColors[m_theme][ColorSchemes::PRIMARYCOLOR]);
            int result = txtBox.getIntegerValue();
            coords[i] = result;
            IntegerPoint3D newPos(coords[0], coords[1], coords[2]);
            FixedMesh::iterator_type pointInMesh = m_workArea.mesh().find(newPos);
            if (pointInMesh != m_workArea.mesh().end()) {
                MyList<FixedMesh::iterator_type> toAdd;
                for (FixedMesh::iterator_type it : m_workArea.mesh().adjacentPoints(m_hovered)) {
                    toAdd.push_back(it);
                }
                for (FixedMesh::iterator_type it = m_workArea.mesh().begin(); it != m_workArea.mesh().end(); ++it) {
                    if (it != m_hovered && m_workArea.mesh().adjacentPoints(it).contains(m_hovered)) {
                        toAdd.push_back(it);
                    }
                }
                for (auto it : toAdd) {
                    m_workArea.mesh().addEdge(pointInMesh, it);
                }
                m_layers[m_hovered->point.z].data.erase(m_hovered);
                if (m_layers[aux.z].data.empty()) {
                    m_layers.erase(aux.z);
                    m_selectedLayer = m_layers.find(result);
                    centerLayerButton();
                    renderLayerSelectButtons();
                    m_selectedLayer = MyMap<int, LayerInfo>::iterator();
                }
                m_workArea.mesh().erasePoint(m_hovered);
                m_hovered = nullptr;
            }
            else {
                if (i == 2) {
                    m_layers[result].data.insert(m_hovered, CircularButton());
                    m_layers[aux.z].data.erase(m_hovered);
                    if (m_layers[aux.z].data.empty()) {
                        m_layers.erase(aux.z);
                    }
                    m_selectedLayer = m_layers.find(result);
                    centerLayerButton();
                    renderLayerSelectButtons();
                    m_selectedLayer = MyMap<int, LayerInfo>::iterator();
                }
                m_workArea.mesh().updatePointValue(m_hovered, newPos);
            }
            m_layers[coords[2]].updateButtons(xCenter, yCenter);
            setvisualpage(1 - getactivepage());
            return true;
        }
    }
    return false;
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
        setfillstyle(SOLID_FILL, LIGHTGRAY);
        bar(0, 0, m_width, m_discardButton.getYLen() + 2);
        bar(m_width - m_minimizedSpaceButton.getXLen() - 3, 0, m_width, m_height);
        bar(0, 0, 34, m_height);
        m_minimizedSpaceButton.drawLabel(LIGHTGRAY, ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR]);
        setcolor(BLACK);
        drawMinimizedSpaceCube();
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
        setfillstyle(SOLID_FILL, LIGHTGRAY);
        bar(0, 0, m_width, m_discardButton.getYLen() + 2);
        bar(m_width - m_minimizedSpaceButton.getXLen() - 3, 0, m_width, m_height);
        bar(0, 0, 34, m_height);
        m_minimizedSpaceButton.drawLabel(LIGHTGRAY, ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR]);
        setcolor(BLACK);
        drawMinimizedSpaceCube();
    }
    if (m_hovered) {
        drawPointData();
    }
    drawToolButtons();
    drawSelectLayers();
    swapbuffers();
}

void ObjectCreator::centerLayerButton() {
    if (m_layers.size() < m_layerSelectButtons.size()) {
        size_t i = 0;
        for (auto it = m_layers.begin(); it != m_layers.end(); ++it) {
            m_layerSelectButtons[i++].it = it;
        }
        renderLayerSelectButtons();
        return;
    }
    auto it = m_layers.begin();
    for (auto& data : m_layerSelectButtons) {
        data.it = it++;
    }
    for (size_t i = 0; i < m_layerSelectButtons.size() / 2; ++i) {
        if (m_layerSelectButtons[i].it == m_selectedLayer) {
            renderLayerSelectButtons();
            return;
        }
    }
    auto backIt = m_layers.end();
    --backIt;
    while (m_layerSelectButtons[m_layerSelectButtons.size() / 2].it != m_selectedLayer && m_layerSelectButtons.back().it != backIt) {
        moveLayerSelectsInterval(1);
    }
    renderLayerSelectButtons();
}

void ObjectCreator::addLayer() {
    int btnColor = RGB(160, 160, 160),
        highlightedColor = ColorSchemes::mixColors(btnColor, ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR], 50);
    auto it = m_layers.begin();
    int newZ = it->key - 1;
    m_layers.insert(newZ, LayerInfo());
    m_selectedLayer = m_layers.find(newZ);
    centerLayerButton();
    renderLayerSelectButtons();
    TextButton& editedLayer = m_layerSelectButtons[0].txtButton;
    draw();
    setactivepage(getvisualpage());
    MyArray<char, 32> prefix = Language::Text[(int)Lang::Layer_Z][m_language];
    NumericInputBox txtBox(editedLayer.getXCenter() - editedLayer.getXLen() / 2 +
                           textwidth(prefix.data()) + 5, editedLayer.getXCenter() + 90,
                           editedLayer.getYCenter(), BLACK, highlightedColor);
    int result = txtBox.getIntegerValue();
    setvisualpage(1 - getactivepage());
    if (m_layers.contains(result) && result != newZ) {
        m_layers.erase(newZ);
        m_selectedLayer = m_layers.find(result);
        centerLayerButton();
        renderLayerSelectButtons();
        int x, y;
        txtBox.getClick(x, y);
        if (x != -1) {
            getClickCommand(x, y);
        }
        draw();
        return;
    }
    else if (result != newZ) {
        m_layers.erase(newZ);
        m_layers.insert(result, LayerInfo());
        m_selectedLayer = m_layers.find(result);
    }
    centerLayerButton();
    renderLayerSelectButtons();
    int x, y;
    txtBox.getClick(x, y);
    if (x != -1) {
        getClickCommand(x, y);
    }
}

void ObjectCreator::deleteLayer() {
    for (auto& dataNode : m_selectedLayer->value.data) {
        FixedMesh::iterator_type it = dataNode.key;
        m_workArea.mesh().erasePoint(it);
    }
    m_layers.erase(m_selectedLayer);
    if (m_layers.size() >= 1) {
        m_selectedLayer = m_layers.begin();
    }
    else {
        m_selectedLayer = MyMap<int, LayerInfo>::iterator();
    }
    centerLayerButton();
    renderLayerSelectButtons();
}

void ObjectCreator::dupeLayer() {
    MyMap<int, LayerInfo>::iterator dupedLayer = m_selectedLayer;
    addLayer();
    int newZ = m_selectedLayer->key;
    for (auto& node : dupedLayer->value.data) {
        IntegerPoint3D newPoint(node.key->point.x, node.key->point.y, newZ);
        m_workArea.mesh().addPoint(newPoint);
        m_selectedLayer->value.data.insert(m_workArea.mesh().begin(), node.value);
    }
    for (auto& node : dupedLayer->value.data) {
        IntegerPoint3D newPoint(node.key->point.x, node.key->point.y, newZ);
        FixedMesh::iterator_type it = node.key;
        if (m_workArea.mesh().countConnections(it) < dupedLayer->value.data.size()) {
            for (FixedMesh::iterator_type it2 : m_workArea.mesh().adjacentPoints(it)) {
                if (it2->point.z != dupedLayer->key) {
                    continue;
                }
                IntegerPoint3D connectedPoint(it2->point.x, it2->point.y, newZ);
                m_workArea.mesh().addEdge(newPoint, connectedPoint);
            }
        }
        else {
            for (auto& node2 : dupedLayer->value.data) {
                FixedMesh::iterator_type it2 = node2.key;
                if (!m_workArea.mesh().adjacentPoints(it).contains(it2) && !m_workArea.mesh().adjacentPoints(it2).contains(it)) {
                    continue;
                }
                IntegerPoint3D connectedPoint(it2->point.x, it2->point.y, newZ);
                m_workArea.mesh().addEdge(newPoint, connectedPoint);
            }
        }
    }
}

void ObjectCreator::mergeLayers(const int movingIndex, MyMap<int, LayerInfo>::iterator destination) {
    const int xCenter = (workX0 + workX1) / 2;
    const int yCenter = (workY0 + workY1) / 2;
    auto& moving = m_layerSelectButtons[movingIndex].it;
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
            auto itDest = m_workArea.mesh().find(newPoint);
            for (auto& pnt : m_workArea.mesh().adjacentPoints(it)) {
                m_workArea.mesh().addEdge(itDest, pnt);
            }
            for (FixedMesh::iterator_type itBefore = m_workArea.mesh().begin(); itBefore != m_workArea.mesh().end(); ++itBefore) {
                if (m_workArea.mesh().adjacentPoints(itBefore).contains(it)) {
                    m_workArea.mesh().addEdge(itBefore, itDest);
                }
            }
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
    MyArray<char, 32> prefix = Language::Text[(int)Lang::Layer_Z][m_language];
    NumericInputBox txtBox(editedLayer.getXCenter() - editedLayer.getXLen() / 2 +
                           textwidth(prefix.data()) + 5, editedLayer.getXCenter() + 90,
                           editedLayer.getYCenter(), BLACK, highlightedColor);
    int result = txtBox.getIntegerValue();
    setvisualpage(1 - getactivepage());
    if (result == m_selectedLayer->key) {
        return;
    }
    if (m_layers.contains(result)) {
        mergeLayers(layerIndex, m_layers.find(result));
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
    if (m_layers[m_hovered->point.z].data.empty()) {
        m_layers.erase(m_hovered->point.z);
        m_selectedLayer = m_layers.begin();
        centerLayerButton();
        renderLayerSelectButtons();
        m_selectedLayer = MyMap<int, LayerInfo>::iterator();
    }
    m_workArea.mesh().erasePoint(m_hovered);
    m_hovered = FixedMesh::iterator_type();
}

void ObjectCreator::pointDeleter3D() {
    m_layers[m_hovered->point.z].data.erase(m_hovered);
    if (m_layers[m_hovered->point.z].data.empty()) {
        m_layers.erase(m_hovered->point.z);
        m_selectedLayer = m_layers.begin();
        centerLayerButton();
        renderLayerSelectButtons();
        m_selectedLayer = MyMap<int, LayerInfo>::iterator();
    }
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
    const Point2D layerCenter = m_selectedLayer->value.centerPoint;
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
        m_assistLine = Line2D(xCenter + xFirstPoint + layerCenter.x, yCenter - yFirstPoint - layerCenter.y, xDrag, yDrag);
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
        case Tool::EditPoint:
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
    if (m_addLayerButton.hitCollision(x, y)) {
        addLayer();
        return true;
    }
    if (m_deleteLayerButton.hitCollision(x, y) && m_selectedLayer) {
        deleteLayer();
        return true;
    }
    if (m_dupeLayerButton.hitCollision(x, y) && m_selectedLayer) {
        dupeLayer();
        return true;
    }
    if (m_generateButton.isListVisible()) {
        if (m_generateButton.listHitCollision(x, y) > -1) {
            int index = m_generateButton.listHitCollision(x, y);
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
            m_hovered = nullptr;
            return true;
        }
    }
    for (size_t i = 0;  i < m_layers.size() && i < m_layerSelectButtons.size(); ++i) {
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
        if (m_pointDataButton.hitCollision(x, y) && m_hovered) {
            return getDoubleClickCommand();
        }
        if (m_workArea.insideWorkArea(x, y) && m_tool == Tool::EditPoint) {
            m_hovered = nullptr;
            return true;
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
                return true;
            }
        }
        if (m_pointDataButton.hitCollision(x, y) && m_hovered) {
            return getDoubleClickCommand();
        }
        if (m_workArea.getCommand(x, y)) {
            return false;
        }
        if (m_workArea.insideWorkArea(x, y) && m_tool == Tool::CutLine) {
            m_assistLineDotted = true;
            lineCutter3D(x, y);
            return true;
        }
        if (m_workArea.insideWorkArea(x, y) && m_tool == Tool::EditPoint) {
            m_hovered = nullptr;
            return true;
        }
    }
    return false;
}

bool ObjectCreator::getHoverCommand(const int& xHov, const int& yHov) {
    if (m_tool != Tool::EditPoint) {
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
    if (m_pointDataButton.hitCollision(x, y) && m_hovered) {
        return changePointData(x, y);
    }
    for (size_t i = 0; i < m_layerSelectButtons.size() && i < m_layers.size(); ++i) {
        if (m_layerSelectButtons[i].txtButton.hitCollision(x, y)) {
            editLayer(i);
            return true;
        }
    }
    return false;
}

Mesh ObjectCreator::run() {
    initwindow(m_width, m_height, Language::Text[(int)Lang::Object_Creator][m_language].data(), 300, 25, false, false);
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
    return m_workArea.mesh().meshConversion();
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
    int helpWindow = initwindow(300, 200, Language::Text[(int)Lang::Cube + index][m_language].data(), getmaxwidth() / 2 - 150, getmaxheight() / 2 - 100, false, false);
    setcurrentwindow(helpWindow);
    setbkcolor(WHITE);
    bar(0, 0, 800, 600);
    setcolor(BLACK);
    TextButton backButton(58, 160, 60, 30, Language::Text[(int)Lang::Discard][m_language].data());
    backButton.drawTextButton(0, 0, LIGHTGRAY);
    TextButton generateButton(140, 160, 70, 30, Language::Text[(int)Lang::Generate][m_language].data());
    generateButton.drawTextButton(0, 0, LIGHTGRAY);
    MyArray<unsigned int, 3> params;
    Mesh tmp();
    params[0] = 200;
    params[1] = 100;
    params[2] = 12;
    if (index == 3) {
        params[0] = 100;
        params[1] = 12;
        params[2] = 12;
    }
    MyArray<TextButton, 3> paramBoxes;
    MyArray<TextLabel, 3> paramLabels;
    if (index == 0) {
        MyArray<char, 32> tmp;
        tmp = itoa(params[0], "");
        paramBoxes[0] = TextButton(160 + 50, 55 + 15, 100, 20, tmp.data());
    }
    else {
        MyArray<char, 32> tmp;
        tmp = itoa(params[0], "");
        paramBoxes[0] = TextButton(160 + 50, 15 + 15, 100, 20, tmp.data());
        tmp = itoa(params[1], "");
        paramBoxes[1] = TextButton(160 + 50, 55 + 15, 100, 20, tmp.data());
        tmp = itoa(params[2], "");
        paramBoxes[2] = TextButton(160 + 50, 95 + 15, 100, 20, tmp.data());
    }
    switch (index) {
        case 0: {
            paramLabels[0] = TextLabel(30 + 50, 55 + 15, 100, 20,  Language::Text[(int)Lang::Length][m_language].data());
            break;
        }
        case 1: {
            paramLabels[0] = TextLabel(30 + 50, 15 + 15, 100, 20,  Language::Text[(int)Lang::Height][m_language].data());
            paramLabels[1] = TextLabel(30 + 50, 55 + 15, 100, 20,  Language::Text[(int)Lang::Radius][m_language].data());
            paramLabels[2] = TextLabel(30 + 50, 95 + 15, 100, 20,  Language::Text[(int)Lang::Sides][m_language].data());
            break;
        }
        case 2: {
            paramLabels[0] = TextLabel(30 + 50, 15 + 15, 100, 20,  Language::Text[(int)Lang::Height][m_language].data());
            paramLabels[1] = TextLabel(30 + 50, 55 + 15, 100, 20,  Language::Text[(int)Lang::Radius][m_language].data());
            paramLabels[2] = TextLabel(30 + 50, 95 + 15, 100, 20,  Language::Text[(int)Lang::Sides][m_language].data());
            break;
        }
        case 3: {
            paramLabels[0] = TextLabel(30 + 50, 15 + 15, 100, 20,  Language::Text[(int)Lang::Radius][m_language].data());
            paramLabels[1] = TextLabel(30 + 50, 55 + 15, 100, 20,  Language::Text[(int)Lang::Segments][m_language].data());
            paramLabels[2] = TextLabel(30 + 50, 95 + 15, 100, 20,  Language::Text[(int)Lang::Rings][m_language].data());
            break;
        }
        default:;
    }
    for (size_t i = 0; (index == 0) ? (i < 1) : (i < 3); ++i) {
        paramBoxes[i].drawTextButton(0, 0, LIGHTGRAY, false);
        paramLabels[i].drawTextLabel(0, 0, LIGHTGRAY);
    }
    bool saveFlag = 0;
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
            if (generateButton.hitCollision(x_, y_)) {
                saveFlag = true;
                break;
            }
            for (size_t i = 0; (index == 0) ? (i < 1) : (i < 3); ++i)  {
                if (paramBoxes[i].hitCollision(x_, y_)) {
                    auto& btnRef = paramBoxes[i];
                    NumericInputBox txtBox(btnRef.getXCenter() - btnRef.getXLen() / 2 + 6,
                                           btnRef.getXCenter() + btnRef.getXLen() / 2 - 6,
                                           btnRef.getYCenter(), BLACK, LIGHTGRAY);
                    int result = txtBox.getIntegerValue();
                    if (result > 0 && result < 1000 && !txtBox.isEmpty()) {
                        params[i] = result;
                    }
                    MyArray<char, 32> tmpText;
                    tmpText = itoa(params[i], "");
                    int xCenter = paramBoxes[i].getXCenter(), yCenter = paramBoxes[i].getYCenter(),
                        xLen = paramBoxes[i].getXLen(), yLen = paramBoxes[i].getYLen();
                    paramBoxes[i] = TextButton(xCenter, yCenter, xLen, yLen, tmpText.data());
                    paramBoxes[i].drawTextButton(0, 0, LIGHTGRAY, false);
                }
            }
        }
    }
    if (saveFlag) {
        Mesh tmp;
        switch(index) {
            case 0:
                tmp = generateCube(params[0]);
                break;
            case 1:
                tmp = generateCone(params[0], params[1], params[2]);
                break;
            case 2:
                tmp = generateCylinder(params[0], params[1], params[2]);
                break;
            case 3:
                tmp = generateSphere(params[0], params[1], params[2]);
                break;
            default:;
        }
        m_workArea.mesh() = FixedMesh(tmp);
        m_selectedLayer = MyMap<int, LayerInfo>::iterator();
        toolButtonsInit();
        init();
        renderLayerSelectButtons();
        resetLine();
    }
    closegraph(helpWindow);
    setcurrentwindow(getCurrentWindowNumber);
    return true;
}
