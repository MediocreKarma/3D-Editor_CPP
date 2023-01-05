#include "Space3D.h"
#include "Menu.h"
#include "ObjectCreator.h"

const double PI = 3.141926535897;
const double err = 0.0000000000000000000001;
const double moveErr = 0.007; //limita minima de perpendicularitate pentru moveMesh

Space3D::Space3D() :
    x0(), y0(), x1(), y1(), m_theme(), m_selected(-1), m_spinballSelected(false), m_fadedDrag(false),  m_objRotateDrag(false), m_meshes(), m_draggedMesh(), m_sections(), m_draggedSection(),
    m_updated(), m_cam(), m_buttonOX(), m_buttonOY(), m_buttonOZ(), m_donutOX(), m_donutOY(), m_donutOZ(), m_spinballButton(), m_arrowLeft(), m_arrowRight(), m_arrowUp(), m_arrowDown(),
    m_arrowSpinLeft(), m_arrowSpinRight(), m_meshContextMenu(), m_meshMenuVisible(false), m_spaceContextMenu(), m_spaceMenuVisible(false), m_gizmoButtons(), m_localTransforms(false), m_panelBtn(),
    m_transformLabels(), m_transformTextBtns(), m_linkedFile{0}, m_menuHolder(nullptr), m_objCreatorHolder(nullptr) {}

Space3D::Space3D(const double& maxRadius, const int& theme, Menu* menuHolder) :
    x0(), y0(), x1(), y1(), m_theme(theme), m_selected(-1), m_spinballSelected(false), m_fadedDrag(false),  m_objRotateDrag(false), m_meshes(), m_draggedMesh(), m_sections(), m_draggedSection(),
    m_updated(), m_cam(maxRadius), m_buttonOX(), m_buttonOY(), m_buttonOZ(), m_donutOX(), m_donutOY(), m_donutOZ(), m_spinballButton(), m_arrowLeft(), m_arrowRight(), m_arrowUp(), m_arrowDown(),
    m_arrowSpinLeft(), m_arrowSpinRight(), m_meshContextMenu(), m_meshMenuVisible(false), m_spaceContextMenu(), m_spaceMenuVisible(false), m_gizmoButtons(), m_localTransforms(false), m_panelBtn(),
    m_transformLabels(), m_transformTextBtns(),
    m_linkedFile{0}, m_menuHolder(menuHolder), m_objCreatorHolder(nullptr) {}

Space3D::Space3D(const double& maxRadius, const int& theme, ObjectCreator* objCreatorHolder) :
    x0(), y0(), x1(), y1(), m_theme(theme), m_selected(-1), m_spinballSelected(false), m_fadedDrag(false), m_objRotateDrag(false), m_meshes(), m_draggedMesh(), m_sections(), m_draggedSection(),
    m_updated(), m_cam(maxRadius), m_buttonOX(), m_buttonOY(), m_buttonOZ(), m_donutOX(), m_donutOY(), m_donutOZ(), m_spinballButton(), m_arrowLeft(), m_arrowRight(), m_arrowUp(), m_arrowDown(),
    m_arrowSpinLeft(), m_arrowSpinRight(), m_meshContextMenu(), m_meshMenuVisible(false), m_spaceContextMenu(), m_spaceMenuVisible(false), m_gizmoButtons(), m_localTransforms(false), m_panelBtn(),
    m_transformLabels(), m_transformTextBtns(),
    m_linkedFile{0}, m_menuHolder(nullptr), m_objCreatorHolder(objCreatorHolder){}

Space3D::Space3D(const Space3D& sp) :
    x0(sp.x0), y0(sp.y0), x1(sp.x1), y1(sp.y1), m_theme(sp.m_theme), m_selected(sp.m_selected), m_spinballSelected(sp.m_spinballSelected), m_fadedDrag(sp.m_fadedDrag),  m_objRotateDrag(false),
    m_meshes(sp.m_meshes), m_draggedMesh(sp.m_draggedMesh), m_sections(sp.m_sections), m_draggedSection(sp.m_draggedSection), m_updated(sp.m_updated), m_cam(sp.m_cam), m_buttonOX(sp.m_buttonOX),
    m_buttonOY(sp.m_buttonOY), m_buttonOZ(sp.m_buttonOZ), m_donutOX(sp.m_donutOX), m_donutOY(sp.m_donutOY), m_donutOZ(sp.m_donutOZ), m_spinballButton(sp.m_spinballButton), m_arrowLeft(sp.m_arrowLeft),
    m_arrowRight(sp.m_arrowRight), m_arrowUp(sp.m_arrowUp), m_arrowDown(sp.m_arrowDown), m_arrowSpinLeft(sp.m_arrowSpinLeft), m_arrowSpinRight(sp.m_arrowSpinRight), m_meshContextMenu(sp.m_meshContextMenu),
    m_meshMenuVisible(sp.m_meshMenuVisible), m_spaceContextMenu(sp.m_spaceContextMenu), m_spaceMenuVisible(sp.m_spaceMenuVisible), m_gizmoButtons(sp.m_gizmoButtons), m_localTransforms(sp.m_localTransforms),
    m_panelBtn(sp.m_panelBtn), m_transformLabels(sp.m_transformLabels), m_transformTextBtns(sp.m_transformTextBtns), m_linkedFile(sp.m_linkedFile), m_menuHolder(sp.m_menuHolder), m_objCreatorHolder(sp.m_objCreatorHolder) {}

Space3D& Space3D::operator = (const Space3D& sp) {
    m_theme = sp.m_theme;
    m_fadedDrag = false;
    m_draggedSection = sp.m_draggedSection;
    m_cam = sp.m_cam;
    m_meshes = sp.m_meshes;
    m_selected = sp.m_selected;
    m_updated = MyVector<bool>(sp.size(), true);
    m_linkedFile = sp.m_linkedFile;
    m_menuHolder = sp.m_menuHolder;
    m_objCreatorHolder = sp.m_objCreatorHolder;
    m_objRotateDrag = 0;
    m_localTransforms = sp.m_localTransforms;
    m_panelBtn = sp.m_panelBtn;
    m_transformTextBtns = sp.m_transformTextBtns;
    setCorners(sp.x0, sp.y0, sp.x1, sp.y1);
    return *this;
}

void Space3D::setCorners(const int& x0_, const int& y0_, const int& x1_, const int& y1_) {
    x0 = x0_;
    y0 = y0_;
    x1 = x1_;
    y1 = y1_;
    setButtons();
}

void Space3D::setButtons() {
    static const int LEN_BUTTON = 17;
    const int xCenter = (x0 + x1) / 2;
    const int yCenter = (y0 + y1) / 2;
    m_arrowUp = Button(xCenter, y0 + LEN_BUTTON, 60, 2 * LEN_BUTTON);
    m_arrowDown = Button(xCenter, y1 - LEN_BUTTON, 60, 2 * LEN_BUTTON);
    m_arrowLeft = Button(x0 + LEN_BUTTON, yCenter, 2 * LEN_BUTTON, 60);
    m_arrowRight = Button(x1 - LEN_BUTTON, yCenter, 2 * LEN_BUTTON, 60);
    m_arrowSpinRight = Button(x0 + 25, y0 + 25, 50, 50);
    m_arrowSpinLeft = Button(x0 + 25, y1 - 25, 50, 50);
    m_spinballButton = Button(x1 - 75, y0 + 75, 140, 140);
    const int xSpinballCenter = m_spinballButton.getXCenter();
    const int ySpinballCenter = m_spinballButton.getYCenter();
    m_donutOX = DonutButton(xSpinballCenter, ySpinballCenter + 120, 40, 20);
    m_donutOY = DonutButton(xSpinballCenter, ySpinballCenter + 220, 40, 20);
    m_donutOZ = DonutButton(xSpinballCenter, ySpinballCenter + 320, 40, 20);
    static const int MENU_BTN_HEIGHT = 24;
    m_meshContextMenu = DropdownButton<7>(-1000, -1000, 0, 0, "", 120, 6 * MENU_BTN_HEIGHT);
    m_meshContextMenu.addOption("Flip X");
    m_meshContextMenu.addOption("Flip Y");
    m_meshContextMenu.addOption("Flip Z");
    m_meshContextMenu.addOption("Scale");
    m_meshContextMenu.addOption("Edit mesh");
    m_meshContextMenu.addOption("Delete mesh");
    m_meshContextMenu.addOption("Reset transforms");
    m_spaceContextMenu = DropdownButton<2>(-1000, -1000, 0, 0, "", 180, 2 * MENU_BTN_HEIGHT);
    m_spaceContextMenu.addOption("Switch to local transform");
    m_spaceContextMenu.addOption("New mesh");
    static const int LABEL_WIDTH = 70;
    static const int LABEL_HEIGHT = 18;
    static const int PANEL_MARGIN = 10;
    static const int PANEL_WIDTH =  4 * LABEL_WIDTH  + 5 * PANEL_MARGIN;
    static const int PANEL_HEIGHT = 3 * LABEL_HEIGHT + 4 * PANEL_MARGIN;
    int              TOP_BEGIN = getmaxy() - PANEL_HEIGHT - PANEL_MARGIN;
    static const int LEFT_BEGIN = PANEL_MARGIN;
    m_panelBtn = Button(PANEL_MARGIN + PANEL_WIDTH / 2, TOP_BEGIN + PANEL_HEIGHT / 2, PANEL_WIDTH, PANEL_HEIGHT);
    int HORIZ_OFFSET = (PANEL_WIDTH - 2 * PANEL_MARGIN - LABEL_WIDTH) / 3;
    int VERT_OFFSET = (PANEL_HEIGHT) / 4;
    static MyArray<MyArray<char, 32>, 3> transformType = {"Scale", "Rotation", "Position"};
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            m_transformTextBtns[i][j] = TextButton(LEFT_BEGIN + 2 * PANEL_MARGIN + LABEL_WIDTH + LABEL_WIDTH / 2 + j * HORIZ_OFFSET,
                                                   TOP_BEGIN + (i + 1) * VERT_OFFSET, LABEL_WIDTH, LABEL_HEIGHT, "");
        }

       m_transformLabels[i] = TextLabel(LEFT_BEGIN + PANEL_MARGIN + LABEL_WIDTH / 2, TOP_BEGIN + (i + 1) * VERT_OFFSET, LABEL_WIDTH, LABEL_HEIGHT, transformType[i].data());
    }
}

void Space3D::updateTransform() {
    m_localTransforms = !m_localTransforms;
    MyArray<MyArray<char, 32>, 2> transformText = {"Switch to local transform", "Switch to global transform"};
    m_spaceContextMenu.changeOptionText(0, transformText[m_localTransforms].data());
}

void Space3D::callHandlerDrawer() {
    if (m_menuHolder) {
        m_menuHolder->draw();
    }
    else {
        m_objCreatorHolder->draw(true);
    }
}

size_t Space3D::size() const {
    return m_meshes.size();
}

void Space3D::addMesh(const Mesh& mesh) {
    m_meshes.push_back(mesh);
    m_sections.resize(size());
    m_updated.push_back(true);
}

void Space3D::removeMesh(const size_t& index) {
    m_meshes.erase(m_meshes.begin() + index);
    m_sections.erase(m_sections.begin() + index);
    m_updated.erase(m_updated.begin() + index);
}

void Space3D::setTheme(const int& theme) {
    m_theme = theme;
}

bool Space3D::fscan(FILE* fp) {
    size_t meshCount = 0;
    if (fscanf(fp, "Models: %u\n", &meshCount) != 1) {
        return 0;
    }
    m_meshes.resize(meshCount);
    for (size_t i = 0; i < meshCount; ++i) {
        if (!m_meshes[i].fscan(fp)) {
            return false;
        }
    }
    m_updated.resize(meshCount);
    m_updated.fill(true);
    m_sections.resize(meshCount);
    if (!m_cam.fscan(fp)) {
        return false;
    }
    return true;
}

void Space3D::fprint(FILE* fp) {
    fprintf(fp, "Models: %u\n", size());
    for (size_t i = 0; i < size(); ++i) {
        m_meshes[i].fprint(fp);
    }
    m_cam.fprint(fp);
}

void Space3D::run() {
    draw();
    drawRotationArrows();
    if (m_meshMenuVisible) {
        struct linesettingstype oldSettings;
        getlinesettings(&oldSettings);
        setlinestyle(0, 0, 1);
        m_meshContextMenu.showList(0, 0, ColorSchemes::mixColors(RGB(222, 222, 222), ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR], 10), 0);
        setlinestyle(oldSettings.linestyle, oldSettings.upattern, oldSettings.thickness);
    }
    if (m_spaceMenuVisible) {
        struct linesettingstype oldSettings;
        getlinesettings(&oldSettings);
        setlinestyle(0, 0, 1);
        m_spaceContextMenu.showList(0, 0, ColorSchemes::mixColors(RGB(222, 222, 222), ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR], 10), 0);
        setlinestyle(oldSettings.linestyle, oldSettings.upattern, oldSettings.thickness);
    }
    if (m_selected != -1) {
        drawGizmo();
        showMeshInfoPanel();
        drawSpinball();
        if (m_spinballSelected) {
            showAngleOptions();
            if(m_objRotateDrag) {
                drawAngleButtons();
            }
        }
    }
}

void Space3D::update() {
    m_updated.fill(true);
}

void drawGizmoArrow(const Point2D& p1, const Point2D& p2, int color) {
    struct linesettingstype lineinfo;
    getlinesettings(&lineinfo);
    setlinestyle(0, 0, 3);
    int oldColor = getcolor();
    setcolor(color);
    int x0 = p1.x, x1 = p2.x, y0 = p1.y, y1 = p2.y;
    line(x0, y0, x1, y1);
    line(x0, y0, x1, y1);
    const int arrLen = 7;
    double theta = atan2(y1 - y0, x1 - x0);
    const double phi_A = 0.785398163397448;
    int xA = x1 - arrLen * cos(theta + phi_A);
    int yA = y1 - arrLen * sin(theta + phi_A);
    const double phi_B = -0.785398163397448;
    int xB = x1 - arrLen * cos(theta + phi_B);
    int yB = y1 - arrLen * sin(theta + phi_B);
    line(xA, yA, x1, y1);
    line(xB, yB, x1, y1);
    setcolor(oldColor);
    setlinestyle(lineinfo.linestyle, lineinfo.upattern, lineinfo.thickness);
}

double Space3D::gizmoLength() const {
    static const double gizmoSize = 0.06f;
    return gizmoSize * ((m_cam.position() - m_meshes[m_selected].centerPoint()).norm());
}

void Space3D::updateGizmoButtons() {
    Point3D center(m_meshes[m_selected].centerPoint());
    MyArray<Point3D, 3> axisPoints {Point3D(1, 0, 0),
                                    Point3D(0, -1, 0),
                                    Point3D(0, 0, 1)};
    for (auto& pnt : axisPoints) {
        if (m_localTransforms) {
            pnt.rotateByUnitQuat(m_meshes[m_selected].quat());
        }
        pnt = pnt * gizmoLength();
    }
    MyArray<Point2D, 3> projectedAxisPoints;
    for (size_t i = 0; i < 3; ++i) {
        projectedAxisPoints[i] = projectPoint(axisPoints[i] + center, m_cam.quat(), m_cam.quat().inverse());
        m_gizmoButtons[i] = CircularButton(projectedAxisPoints[i].x, projectedAxisPoints[i].y, 7);
    }
}

void Space3D::drawGizmo() {
    updateGizmoButtons();
    Point2D projectedCenter(projectPoint(m_meshes[m_selected].centerPoint(), m_cam.quat(), m_cam.quat().inverse()));
    drawGizmoArrow(projectedCenter, Point2D(m_gizmoButtons[0].getX(), m_gizmoButtons[0].getY()), LIGHTGREEN);
    drawGizmoArrow(projectedCenter, Point2D(m_gizmoButtons[1].getX(), m_gizmoButtons[1].getY()), LIGHTRED);
    drawGizmoArrow(projectedCenter, Point2D(m_gizmoButtons[2].getX(), m_gizmoButtons[2].getY()), LIGHTCYAN);
}

void Space3D::drawRotationArrows() {
    const int BORDER_OFFSET = 5;
    const int POSITION_OFFSET = 25;
    const int xCenter = (x0 + x1) / 2;
    const int yCenter = (y0 + y1) / 2;
    setcolor(ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR]);
    setlinestyle(SOLID_LINE, 0, 2);
    line(x0 + BORDER_OFFSET, yCenter, x0 + POSITION_OFFSET, yCenter - POSITION_OFFSET);
    line(x0 + BORDER_OFFSET, yCenter, x0 + POSITION_OFFSET, yCenter + POSITION_OFFSET);
    line(x1 - BORDER_OFFSET, yCenter, x1 - POSITION_OFFSET, yCenter - POSITION_OFFSET);
    line(x1 - BORDER_OFFSET, yCenter, x1 - POSITION_OFFSET, yCenter + POSITION_OFFSET);
    line(xCenter, y0 + BORDER_OFFSET, xCenter - POSITION_OFFSET, y0 + POSITION_OFFSET);
    line(xCenter, y0 + BORDER_OFFSET, xCenter + POSITION_OFFSET, y0 + POSITION_OFFSET);
    line(xCenter, y1 - BORDER_OFFSET, xCenter - POSITION_OFFSET, y1 - POSITION_OFFSET);
    line(xCenter, y1 - BORDER_OFFSET, xCenter + POSITION_OFFSET, y1 - POSITION_OFFSET);
}

void Space3D::drawSpinball() {
    const int x = m_spinballButton.getXCenter();
    const int y = m_spinballButton.getYCenter();
    m_spinballButton.drawLabel(ColorSchemes::themeColors[m_theme][ColorSchemes::PRIMARYCOLOR], ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR]);
    setlinestyle(SOLID_LINE, 0, 2);
    setcolor(LIGHTRED);
    CircularLabel labelOX = CircularLabel(x + 10 * cos(m_meshes[m_selected].angleX()), y + 60 * sin(m_meshes[m_selected].angleX()), 5);
    CircularLabel labelOY = CircularLabel(x + 60 * cos(m_meshes[m_selected].angleY()), y + 60 * sin(m_meshes[m_selected].angleY()), 5);
    CircularLabel labelOZ = CircularLabel(x + 60 * cos(m_meshes[m_selected].angleZ()), y + 10 * sin(m_meshes[m_selected].angleZ()), 5);
    circle(x, y, 60);
    labelOY.drawLabel(LIGHTRED, LIGHTRED);
    setcolor(LIGHTGREEN);
    ellipse(x, y, 0, 360, 10, 60);
    labelOX.drawLabel(LIGHTGREEN, LIGHTGREEN);
    setcolor(LIGHTBLUE);
    ellipse(x, y, 0, 360, 60, 10);
    labelOZ.drawLabel(LIGHTBLUE, LIGHTBLUE);
}

void Space3D::showAngleOptions() {
    m_donutOX.drawLabel(ColorSchemes::NO_COLOR, LIGHTGREEN);
    m_donutOY.drawLabel(ColorSchemes::NO_COLOR, LIGHTRED);
    m_donutOZ.drawLabel(ColorSchemes::NO_COLOR, LIGHTBLUE);
}

void Space3D::drawAngleButtons() {
    switch(m_objRotateDrag) {
        case 1:
            m_buttonOX.drawLabel(LIGHTGREEN, LIGHTGREEN);
            break;
        case 2:
            m_buttonOY.drawLabel(LIGHTRED, LIGHTRED);
            break;
        case 3:
            m_buttonOZ.drawLabel(LIGHTBLUE, LIGHTBLUE);
            break;
        default:
            return;
    }
}

bool Space3D::checkCamMovement(const char& c) {
    const int distance = 25;
    Point3D auxPoint;
    switch (c) {
        case 'a':
            auxPoint = Point3D(-distance, 0, 0);
            break;
        case 'd':
            auxPoint = Point3D(distance, 0, 0);
            break;
        case 's':
            auxPoint = Point3D(0, -distance, 0);
            break;
        case 'w':
            auxPoint = Point3D(0, distance, 0);
            break;
        case 'e':
            auxPoint = Point3D(0, 0, distance);
            break;
        case 'q':
            auxPoint = Point3D(0, 0, -distance);
            break;
        default:
            return 0;
    }
    Point3D rotatedPoint = rotateByCamera(auxPoint);
    m_cam.modifyPosition(rotatedPoint.getX(), rotatedPoint.getY(), rotatedPoint.getZ());
    m_updated.fill(true);
    callHandlerDrawer();
    return 1;
}

bool Space3D::checkGizmo(int x_, int y_) {
    bool& isLocal = m_localTransforms;
    Mesh original = m_meshes[m_selected];
    int x = x_, y = y_;
    for (size_t i = 0; i < 3; ++i) {
        if (m_gizmoButtons[i].hitCollision(x, y)) {
            clearmouseclick(WM_LBUTTONUP);
            while (!ismouseclick(WM_LBUTTONUP)) {
                if (kbhit()) {
                    char c = getch();
                    if (c == 27) {
                        m_meshes[m_selected] = original;
                        m_updated[m_selected] = true;
                        callHandlerDrawer();
                        return true;
                    }
                }
                int xMove, yMove;
                getmouseclick(WM_MOUSEMOVE, xMove, yMove);
                if (insideWorkArea(xMove, yMove)) {
                    Point3D axis;
                    if (isLocal) {
                        axis = m_meshes[m_selected].localAxis(i);
                    }
                    else {
                        switch (i) {
                            case 0:
                                axis = Point3D(1, 0, 0);
                                break;
                            case 1:
                                axis = Point3D(0, 1, 0);
                                break;
                            case 2:
                                axis = Point3D(0, 0, 1);
                                break;
                            default:;
                        }
                    }
                    moveMeshHelper(xMove, yMove, i, isLocal);
                    axis = axis * gizmoLength();
                    if (i != 1) {
                        m_meshes[m_selected].translate(-axis.x, -axis.y, -axis.z);
                    }
                    else {
                        m_meshes[m_selected].translate(axis.x, axis.y, axis.z);
                    }
                    x = xMove;
                    y = yMove;
                    updateGizmoButtons();
                    callHandlerDrawer();
                }
            }
            return true;
        }
    }
    return false;
}

bool Space3D::checkObjectRotation(int x, int y) {
    double tempAngle = 0;
    const int xSpinball = m_spinballButton.getXCenter();
    const int ySpinball = m_spinballButton.getYCenter();
    if (m_donutOX.hitCollision(x, y)) {
        m_objRotateDrag = 1;
        double rotation1 = findRotation(x, y, m_donutOX, m_buttonOX);
        tempAngle += rotation1;
        m_buttonOX.move(xSpinball + 40 * cos(tempAngle), ySpinball + 120 + 40 * sin(tempAngle));
        int xDrag, yDrag;
        getmouseclick(WM_LBUTTONUP, xDrag, yDrag);
        int dragDiff = sqrt((xDrag - x) * (xDrag - x) + (yDrag - y) * (yDrag - y));
        while (!ismouseclick(WM_LBUTTONUP)) {
            getmouseclick(WM_MOUSEMOVE, xDrag, yDrag);
            if (m_donutOX.hitCollision(xDrag, yDrag) && dragDiff >= 5) {
                double rotation = findRotation(xDrag, yDrag, m_donutOX, m_buttonOX);
                if (!m_localTransforms) {
                    m_meshes[m_selected].rotate(rotation, 0, 0);
                }
                else {
                    m_meshes[m_selected].rotateOnAxis(m_meshes[m_selected].centerPoint(), m_meshes[m_selected].localAxis(0), rotation);
                }
                tempAngle += rotation;
                m_meshes[m_selected].rotateDisplayAngle();
                m_updated[m_selected] = true;
                m_buttonOX.move(xSpinball + 40 * cos(tempAngle), ySpinball + 120 + 40 * sin(tempAngle));
                callHandlerDrawer();
                x = xDrag;
                y = yDrag;
            }
            dragDiff = sqrt((xDrag - x) * (xDrag - x) + (yDrag - y) * (yDrag - y));
        }
        selectMesh(m_selected);
        callHandlerDrawer();
        m_objRotateDrag = 0;
        return true;
    }
    if (m_donutOY.hitCollision(x, y)) {
        m_objRotateDrag = 2;
        double rotation1 = findRotation(x, y, m_donutOY, m_buttonOY);
        tempAngle += rotation1;
        m_buttonOY.move(xSpinball + 40 * cos(tempAngle), ySpinball + 220 + 40 * sin(tempAngle));
        int xDrag, yDrag;
        getmouseclick(WM_LBUTTONUP, xDrag, yDrag);
        int dragDiff = sqrt((xDrag - x) * (xDrag - x) + (yDrag - y) * (yDrag - y));
        while (!ismouseclick(WM_LBUTTONUP)) {
            getmouseclick(WM_MOUSEMOVE, xDrag, yDrag);
            if (m_donutOY.hitCollision(xDrag, yDrag) && dragDiff >= 5) {
                double rotation = findRotation(xDrag, yDrag, m_donutOY, m_buttonOY);
                if (!m_localTransforms) {
                    m_meshes[m_selected].rotate(0, rotation, 0);
                }
                else {
                    m_meshes[m_selected].rotateOnAxis(m_meshes[m_selected].centerPoint(), m_meshes[m_selected].localAxis(1), rotation);
                }
                tempAngle += rotation;
                m_meshes[m_selected].rotateDisplayAngle();
                m_updated[m_selected] = true;
                m_buttonOY.move(xSpinball + 40 * cos(tempAngle), ySpinball + 220 + 40 * sin(tempAngle));
                callHandlerDrawer();
                x = xDrag;
                y = yDrag;
            }
            dragDiff = sqrt((xDrag - x) * (xDrag - x) + (yDrag - y) * (yDrag - y));
        }
        selectMesh(m_selected);
        callHandlerDrawer();
        m_objRotateDrag = 0;
        return true;
    }
    if (m_donutOZ.hitCollision(x, y)) {
        m_objRotateDrag = 3;
        double rotation1 = findRotation(x, y, m_donutOZ, m_buttonOZ);
        tempAngle += rotation1;
        m_buttonOZ.move(xSpinball + 40 * cos(tempAngle), ySpinball + 320 + 40 * sin(tempAngle));
        int xDrag, yDrag;
        getmouseclick(WM_LBUTTONUP, xDrag, yDrag);
        int dragDiff = sqrt((xDrag - x) * (xDrag - x) + (yDrag - y) * (yDrag - y));
        while (!ismouseclick(WM_LBUTTONUP)) {
            getmouseclick(WM_MOUSEMOVE, xDrag, yDrag);
            if (m_donutOZ.hitCollision(xDrag, yDrag) && dragDiff >= 5) {
                double rotation = findRotation(xDrag, yDrag, m_donutOZ, m_buttonOZ);
                if (!m_localTransforms) {
                    m_meshes[m_selected].rotate(0, 0, -rotation);
                }
                else {
                    m_meshes[m_selected].rotateOnAxis(m_meshes[m_selected].centerPoint(), m_meshes[m_selected].localAxis(2), -rotation);
                }
                tempAngle += rotation;
                m_meshes[m_selected].rotateDisplayAngle();
                m_updated[m_selected] = true;
                m_buttonOZ.move(xSpinball + 40 * cos(tempAngle), ySpinball + 320 + 40 * sin(tempAngle));
                callHandlerDrawer();
                x = xDrag;
                y = yDrag;
            }
            dragDiff = sqrt((xDrag - x) * (xDrag - x) + (yDrag - y) * (yDrag - y));
        }
        selectMesh(m_selected);
        callHandlerDrawer();
        m_objRotateDrag = 0;
        return true;
    }
    m_objRotateDrag = 0;
    return false;
}

bool Space3D::checkAxisRotation(const int& x, const int& y) {
    const double Grad_1 = 0.0078539816339745 / 1.5;
    clearmouseclick(WM_LBUTTONUP);
    if (m_arrowDown.hitCollision(x, y)) {
        while (!ismouseclick(WM_LBUTTONUP)) {
            m_cam.rotateOnAxis(0, -Grad_1);
            m_updated.fill(true);
            callHandlerDrawer();
        }
        return true;
    }
    if (m_arrowUp.hitCollision(x, y)) {
        while (!ismouseclick(WM_LBUTTONUP)) {
            m_cam.rotateOnAxis(0, Grad_1);
            m_updated.fill(true);
            callHandlerDrawer();
        }
        return true;
    }
    if (m_arrowLeft.hitCollision(x, y)) {
        while (!ismouseclick(WM_LBUTTONUP)) {
            m_cam.rotateOnAxis(2, Grad_1);
            m_updated.fill(true);
            callHandlerDrawer();
        }
        return true;
    }
    if (m_arrowRight.hitCollision(x, y)) {
        while (!ismouseclick(WM_LBUTTONUP)) {
            m_cam.rotateOnAxis(2, -Grad_1);
            m_updated.fill(true);
            callHandlerDrawer();
        }
        return true;
    }
    return false;
}

double Space3D::findRotation(const int& xDrag, const int& yDrag, const DonutButton& angleDonut, CircularButton& button) {
    int xCircle = angleDonut.getX();
    int yCircle = angleDonut.getY();
    int xDiff = xDrag - angleDonut.getX();
    int yDiff = yDrag - angleDonut.getY();
    double magV = sqrt(xDiff * xDiff + yDiff * yDiff);
    int xOnCircle = xCircle + xDiff / magV * angleDonut.getRadius();
    int yOnCircle = yCircle + yDiff / magV * angleDonut.getRadius();
    return atan2(yOnCircle - yCircle, xOnCircle - xCircle) - atan2(button.getY() - yCircle, button.getX() - xCircle);
}

MyArray<char, 32> Space3D::itoa(int x, const char* prefix) const {
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

int Space3D::atoi(MyArray<char, 256>& arr) {
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

void Space3D::updateTransformFields() {
    static MyArray<MyArray<char, 8>, 3> AXIS_PREFIXES = {"X : ", "Y : ", "Z : "};
    const MeshTransforms meshTrans = m_meshes[m_selected].transforms();
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            m_transformTextBtns[i][j].modifyText(itoa(::round(meshTrans[i][j]), AXIS_PREFIXES[j].data()));
        }
    }
}

void Space3D::showMeshInfoPanel() {
    m_panelBtn.drawLabel(WHITE, BLACK);
    updateTransformFields();
    for (size_t i = 0; i < 3; ++i) {
        m_transformLabels[i].drawTextLabel(0, 0, ColorSchemes::mixColors(ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR], RGB(155, 155, 155), 75));
        for (size_t j = 0; j < 3; ++j) {
            m_transformTextBtns[i][j].drawTextButton(3, 1, WHITE, false);
        }
    }
}

Point3D Space3D::rotateByCamera(const Point3D& pct) const {
    double xr = pct.getX();
    double yr = pct.getY();
    double zr = pct.getZ();
    Point3D aux = Point3D(xr, yr, zr).rotatedByUnitQuat(m_cam.quat());
    double dx = aux.getX();
    double dy = aux.getY();
    double dz = aux.getZ();
    return Point3D(dx, dy, dz);
}

Point3D Space3D::normalisePoint(const Point3D& pct,  const Quaternion& camQuat, const Quaternion& camInverse) const {
    double xr = pct.getX() - m_cam.position().getX();
    double yr = pct.getY() - m_cam.position().getY();
    double zr = pct.getZ() - m_cam.position().getZ();
    Quaternion pointQuat(0, Point3D(xr, yr, zr).toArray());
    Quaternion rotatedPct = Quaternion(camInverse * pointQuat * camQuat);
    double dx = rotatedPct[1];
    double dy = rotatedPct[2];
    double dz = rotatedPct[3];
    return Point3D(dx, dy, dz);
}

Point2D Space3D::projectPoint(const Point3D& pct, const Quaternion& camQuat, const Quaternion& camInverse) const {
    const int xCenter = (x0 + x1) / 2;
    const int yCenter = (y0 + y1) / 2;
    const int yLen = (y1 - y0);
    const int xr = pct.getX() - m_cam.position().getX();
    const int yr = pct.getY() - m_cam.position().getY();
    const int zr = pct.getZ() - m_cam.position().getZ();
    double EZ = m_cam.EZ();
    Quaternion pointQuat(0, Point3D(xr, yr, zr).toArray());
    Quaternion rotatedPct = Quaternion(camInverse * pointQuat * camQuat);
    double dx = rotatedPct[1];
    double dy = rotatedPct[2];
    double dz = rotatedPct[3];
    if (dy <= 0) {
        return Point2D(-100, -100);
    }
    double xprim = EZ * dx * yLen / dy + xCenter;
    double yprim = EZ * dz * yLen / dy * -1 + yCenter;
    return Point2D(xprim, yprim);
}

Section Space3D::projectSection(const Mesh& mesh) {
    if (mesh.size() == 0) {
        return Section(MyVector<Point2D>(), Point2D(-100, -100), mesh.adjacencyList());
    }
    MyVector<Point2D> projectedPoints;
    projectedPoints.resize(mesh.size());
    Quaternion camQuat = m_cam.quat();
    Quaternion camInverse = camQuat.inverse();
    for(size_t i = 0; i < mesh.size(); i++) {
        projectedPoints[i] = projectPoint(mesh[i], camQuat, camInverse);
    }
    if (!m_menuHolder) {
        return Section(projectedPoints, Point2D(-100, -100), mesh.adjacencyList());
    }
    Point2D sectionCenterPoint = projectPoint(mesh.centerPoint(), camQuat, camInverse);
    return Section(projectedPoints, sectionCenterPoint, mesh.adjacencyList());
}

Point2D Space3D::moveInsideWorkArea(const Point2D& P, const Point2D& Q, const int& xBorder, const int& yBorder) {
    int xP = P.getX();
    int yP = P.getY();
    int xQ = Q.getX();
    int yQ = Q.getY();
    double m = 1. * (yQ - yP) / (xQ - xP);
    if (yQ < yBorder) {
        return Point2D((yBorder - yP) / m + xP, (double)yBorder);
    }
    if (xQ < xBorder) {
        return Point2D((double)xBorder, m * (xBorder - xP) + yP);
    }
    return Q;
}

void Space3D::render() {
    for (size_t i = 0; i < size(); ++i) {
        if (m_updated[i]) {
            m_sections[i] = projectSection(m_meshes[i]);
            m_updated[i] = false;
        }
    }
    if (m_fadedDrag) {
        m_draggedSection = projectSection(m_draggedMesh);
    }
}

bool Space3D::insideWorkArea(const int& x, const int& y) const {
    return x0 <= x && x <= x1 && y0 <= y && y <= y1;
}

bool Space3D::insideWorkArea(const Point2D& point) const {
    return insideWorkArea(point.getX(), point.getY());
}

bool Space3D::getKeyCommand() {
    if (!kbhit()) {
        return false;
    }
    char x;
    do {
        x = getch();
    } while (kbhit());
    return checkKeyCommand(x);
}

bool Space3D::checkKeyCommand(const char& x) {
    //comenzi pt scale si move ca in blender. fscale si gmove
    if (m_selected != -1 && x == 'f') {
        scaleMesh();
        return true;
    }
    if (m_selected != -1 && x == 'g') {
        moveMesh();
        return true;
    }
    if (x == 'l') {
        updateTransform();
        return true;
    }
    if (m_meshMenuVisible) {
        m_meshMenuVisible = false;
    }
    if (x == 'z' && m_cam.EZ() < 1.0f / tan(30 / 2 * PI / 180))  {
        double radAngle = 2 * atan(1 / m_cam.EZ());
        radAngle -= 10 * PI / 180;
        m_cam.setEZ(1 / tanf(radAngle / 2));
        update();
        return true;
    }
    if (x == 'x' && m_cam.EZ() > 1.0f / tan(110 / 2 * PI / 180))  {
        double radAngle = 2 * atan(1 / m_cam.EZ());
        radAngle += 10 * PI / 180;
        m_cam.setEZ(1 / tanf(radAngle / 2));
        update();
        return true;
    }
    return checkCamMovement(x);
}

bool Space3D::getCommand(const int& x, const int& y) {
    if (m_selected != -1 && m_meshMenuVisible && m_meshContextMenu.listHitCollision(x, y) != -1) {
        m_meshMenuVisible = false;
        int rMenuIndex = m_meshContextMenu.listHitCollision(x, y);
        if (rMenuIndex >= 0 && rMenuIndex<= 2) {
            m_meshes[m_selected].mirror(rMenuIndex);
            m_updated[m_selected] = true;
        }
        if (rMenuIndex == 3) {
            scaleMesh();
        }
        if (rMenuIndex == 4) {
            int getCurrentWindowNumber = getcurrentwindow();
            Mesh aux = m_meshes[m_selected];
            Point3D center = aux.centerPoint();
            Quaternion quat = aux.quat();
            aux.resetRotation();
            aux.resetScale();
            ObjectCreator objCreator(aux, m_theme);
            aux = objCreator.run();
            if (objCreator.getCloseFlag() == 2) {
                aux.rotateByUnitQuat(quat);
                aux.translate(center);
                m_meshes[m_selected] = aux;
                m_updated[m_selected] = true;
            }
            setcurrentwindow(getCurrentWindowNumber);
        }
        if (rMenuIndex == 5) {
            removeMesh(m_selected);
            m_selected = -1;
        }
        if (rMenuIndex == 6) {
            m_meshes[m_selected].resetTransforms();
            m_updated[m_selected] = true;
        }
        return true;
    }
    else {
        if (m_meshMenuVisible) {
            m_meshMenuVisible = false;
        }
    }
    if (m_spaceContextMenu.listHitCollision(x, y) != -1 && m_spaceMenuVisible) {
        m_spaceMenuVisible = false;
        switch (m_spaceContextMenu.listHitCollision(x, y)) {
            case 0:
                updateTransform();
                break;
            case 1:
                int getCurrentWindowNumber = getcurrentwindow();
                ObjectCreator objCreator(m_theme);
                Mesh aux = objCreator.run();
                if (objCreator.getCloseFlag() == 2) {
                    addMesh(aux);
                }
                setcurrentwindow(getCurrentWindowNumber);
                break;
        }
        return true;
    }
    else {
        if (m_spaceMenuVisible) {
            m_spaceMenuVisible = false;
        }
    }
    if (m_selected != -1 && m_spinballButton.hitCollision(x, y)) {
        m_spinballSelected = !m_spinballSelected;
        return true;
    }
    if (m_spinballSelected) {
        if (checkObjectRotation(x, y)) {
            return true;
        }
    }
    if (checkAxisRotation(x, y)) {
            return true;
    }
    for (size_t i = 0; i < size(); ++i) {
        if (m_sections[i].grabButtonCollision(x, y)) {
            selectMesh(i);
            updateGizmoButtons();
            callHandlerDrawer();
            dragMesh();
            return true;
        }
    }
    if (m_selected != -1 && checkGizmo(x, y)) {
        return true;
    }
    if (m_panelBtn.hitCollision(x, y)) {
        //TODO: Textboxes, use setTransform to apply certain transform with indexes of clicked button
        //angles will have to be converted to radians
        //example: m_meshes[m_selected].setTransform(1, 2, 5 * PI / 180); - for setting Rotation (1) on the Z (2) axis to 5deg
        return true;
    }
    if (insideWorkArea(x, y) && m_selected != -1) {
        m_selected = -1;
        if (m_meshMenuVisible) {
            m_meshMenuVisible = false;
        }
        return true;
    }
    else if (insideWorkArea(x, y) && m_spaceContextMenu.listHitCollision(x, y) == -1) {
        m_spaceMenuVisible = false;
        return true;
    }
    return false;
}

bool Space3D::getRightClickCommand(const int& x, const int& y) {
    if (m_selected != -1 && m_spinballButton.hitCollision(x, y)) {
        m_meshes[m_selected].resetRotation();
        m_updated[m_selected] = true;
        return true;
    }
    for (size_t i = 0; i < size(); ++i) {
        if (m_sections[i].grabButtonCollision(x, y)) {
            if (m_spaceMenuVisible) {
                m_spaceMenuVisible = false;
            }
            selectMesh(i);
            if (x <= x1 - m_meshContextMenu.width() && y <= y1 - m_meshContextMenu.height()) {
                m_meshContextMenu.move(x, y);
            }
            else {
                m_meshContextMenu.move(x - m_meshContextMenu.width(), y - m_meshContextMenu.height());
            }
            m_meshMenuVisible = true;
            return true;
        }
    }
    if (m_meshMenuVisible) {
        m_meshMenuVisible = false;
        m_selected = -1;
    }
    if (insideWorkArea(x, y)) {
        m_selected = -1;
        if (x <= x1 - m_spaceContextMenu.width() && y <= y1 - m_spaceContextMenu.height()) {
            m_spaceContextMenu.move(x, y);
        }
        else {
            m_spaceContextMenu.move(x - m_spaceContextMenu.width(), y - m_spaceContextMenu.height());
        }
        m_spaceMenuVisible = true;
        return true;
    }
    return false;
}

void Space3D::dragMesh() {
    m_draggedMesh = m_meshes[m_selected];
    int xDrag, yDrag;
    getmouseclick(WM_MOUSEMOVE, xDrag, yDrag);
    getmouseclick(WM_LBUTTONUP, xDrag, yDrag);
    while (!ismouseclick(WM_LBUTTONUP)) {
        getmouseclick(WM_MOUSEMOVE, xDrag, yDrag);
        if (insideWorkArea(xDrag, yDrag)) {
            if (!m_sections[m_selected].grabButtonCollision(xDrag, yDrag)) {
                Quaternion camQuat = m_cam.quat();
                Quaternion camInverse = camQuat.inverse();
                dragAndDrop(xDrag, yDrag, m_draggedMesh, camQuat, camInverse);
                m_fadedDrag = true;
                m_menuHolder->draw();
            }
            else {
                m_menuHolder->draw();
            }
        }
    }
    if (!m_sections[m_selected].grabButtonCollision(xDrag, yDrag)) {
        m_meshes[m_selected] = m_draggedMesh;
        m_updated[m_selected] = true;
    }
}

void Space3D::drawDottedLine(int x0_, int y0_, int x1_, int y1_) {
    int olda = getactivepage();
    int oldv = getvisualpage();
    setactivepage(oldv);
    struct linesettingstype oldSettings;
    getlinesettings(&oldSettings);
    int oldColor = getcolor();
    setlinestyle(3, 0, 2);
    setcolor(RGB(128, 128, 128));
    line(x0_, y0_, x1_, y1_);
    setlinestyle(oldSettings.linestyle, oldSettings.upattern, oldSettings.thickness);
    setcolor(oldColor);
    setactivepage(olda);
}

void Space3D::scaleMesh() {
    short scaleAxis = -1;
    int xOrig, yOrig;
    xOrig = mousex();
    yOrig = mousey();
    int dx = abs(xOrig - m_sections[m_selected].centerPoint().x);
    int dy = abs(yOrig - m_sections[m_selected].centerPoint().y);
    if (insideWorkArea(xOrig, yOrig)) {
        drawDottedLine(m_sections[m_selected].centerPoint().x, m_sections[m_selected].centerPoint().y, xOrig, yOrig);
    }
    double distOrig = sqrt(dx * dx + dy * dy);
    double distCurrent = distOrig;
    double scaleFactor = 1;
    Mesh original = m_meshes[m_selected];
    while (!ismouseclick(WM_LBUTTONDOWN)) {
        if (kbhit()) {
            char x = getch();
            switch(x) {
                case 27:
                    m_meshes[m_selected] = original;
                    m_updated[m_selected] = true;
                    callHandlerDrawer();
                    return;
                //-1 - global; 0, 1, 2 - axele X, Y, Z
                case 'x':
                    m_meshes[m_selected] = original;
                    if (scaleAxis != 0) {
                        scaleAxis = 0;
                        m_meshes[m_selected].scaleAxis(scaleFactor, 0);
                    }
                    else {
                        scaleAxis = -1;
                        m_meshes[m_selected].scaleEven(scaleFactor);
                    }
                    m_updated[m_selected] = true;
                    callHandlerDrawer();
                    drawDottedLine(m_sections[m_selected].centerPoint().x, m_sections[m_selected].centerPoint().y, mousex(), mousey());
                    break;
                case 'y':
                    m_meshes[m_selected] = original;
                    if (scaleAxis != 1) {
                        scaleAxis = 1;
                        m_meshes[m_selected].scaleAxis(scaleFactor, 1);
                    }
                    else {
                        scaleAxis = -1;
                        m_meshes[m_selected].scaleEven(scaleFactor);
                    }
                    m_updated[m_selected] = true;
                    callHandlerDrawer();
                    drawDottedLine(m_sections[m_selected].centerPoint().x, m_sections[m_selected].centerPoint().y, mousex(), mousey());
                    break;
                case 'z':
                    m_meshes[m_selected] = original;
                    if (scaleAxis != 2) {
                        scaleAxis = 2;
                        m_meshes[m_selected].scaleAxis(scaleFactor, 2);
                    }
                    else {
                        scaleAxis = -1;
                        m_meshes[m_selected].scaleEven(scaleFactor);
                    }
                    m_updated[m_selected] = true;
                    callHandlerDrawer();
                    drawDottedLine(m_sections[m_selected].centerPoint().x, m_sections[m_selected].centerPoint().y, mousex(), mousey());
                    break;
                default:;
            }
            continue;
        }
        int xMove, yMove;
        getmouseclick(WM_MOUSEMOVE, xMove, yMove);
        if (xMove >= 0 && yMove >= 0 && insideWorkArea(xMove, yMove)) {
            dx = abs(xMove - m_sections[m_selected].centerPoint().x);
            dy = abs(yMove - m_sections[m_selected].centerPoint().y);
            distCurrent = sqrt(dx * dx + dy * dy);
            scaleFactor = distCurrent / distOrig;
            if (fabs(scaleFactor) > err && distCurrent > 5) {
                if (scaleAxis == -1) {
                    m_meshes[m_selected].scaleAxis(scaleFactor * original.scaleX() / m_meshes[m_selected].scaleX(), 0);
                    m_meshes[m_selected].scaleAxis(scaleFactor * original.scaleY() / m_meshes[m_selected].scaleY(), 1);
                    m_meshes[m_selected].scaleAxis(scaleFactor * original.scaleZ() / m_meshes[m_selected].scaleZ(), 2);
                }
                else {
                    switch (scaleAxis) {
                    case 0:
                        m_meshes[m_selected].scaleAxis(scaleFactor * original.scaleX() / m_meshes[m_selected].scaleX(), scaleAxis);
                        break;
                    case 1:
                        m_meshes[m_selected].scaleAxis(scaleFactor * original.scaleY() / m_meshes[m_selected].scaleY(), scaleAxis);
                        break;
                    case 2:
                        m_meshes[m_selected].scaleAxis(scaleFactor * original.scaleZ() / m_meshes[m_selected].scaleZ(), scaleAxis);
                        break;
                    default:;
                    }
                }
                m_updated[m_selected] = true;
                callHandlerDrawer();
                drawDottedLine(m_sections[m_selected].centerPoint().x, m_sections[m_selected].centerPoint().y, xMove, yMove);
            }
        }
    }
    clearmouseclick(WM_LBUTTONDOWN);
}

bool Space3D::moveMeshHelper(int xMove, int yMove, int moveAxis, bool isLocal) {
    MyArray<Point3D, 2> axisNormals{Point3D(0, 1, 0), Point3D(1, 0, 0)}; //normalele lu XZ si YZ, deci Y si X
    Quaternion quat = m_meshes[m_selected].quat();
    if (isLocal) {
        axisNormals[0].rotateByUnitQuat(quat);
        axisNormals[1].rotateByUnitQuat(quat);
    }
    if (insideWorkArea(xMove, yMove)) {
        Point3D center = m_meshes[m_selected].centerPoint();
        Point3D aux;
        switch (moveAxis) {
            case 0: { //cast ray onto XZ plane, find intersection, translate only by x
                Ray cursorRay(m_cam.position(), unprojectPoint(xMove, yMove, m_cam.quat()));
                if (fabs(dot(axisNormals[0], cursorRay.direction)) > moveErr) {
                    aux = rayCastOnPlane(xMove, yMove, axisNormals[0], center);
                    if (!isLocal) {
                        m_meshes[m_selected].translate(aux.x - center.x, 0, 0);
                    }
                    else { //unrotate, get only the x, rotate => rotated delta vector
                        aux -= center;
                        aux.rotateByUnitQuat(quat.inverse());
                        aux.y = aux.z = 0;
                        aux.rotateByUnitQuat(quat);
                        m_meshes[m_selected].translate(aux);
                    }
                }
                break;
            }
            case 1: { //cast ray onto YZ plane, same principle
                Ray cursorRay(m_cam.position(), unprojectPoint(xMove, yMove, m_cam.quat()));
                if (fabs(dot(axisNormals[1], cursorRay.direction)) > moveErr) {
                    aux = rayCastOnPlane(xMove, yMove, axisNormals[1], center);
                    if (!isLocal) {
                        m_meshes[m_selected].translate(0, aux.y - center.y, 0);
                    }
                    else {
                        aux -= center;
                        aux.rotateByUnitQuat(quat.inverse());
                        aux.x = aux.z = 0;
                        aux.rotateByUnitQuat(quat);
                        m_meshes[m_selected].translate(aux);
                    }
                }
                break;
            }
            case 2: {
                //alege dot-u mai diferit de 0 dintre dotu camNormal * XZNormal vs camNormal * YZNormal
                //Dot product ul imi va spune, cat de perpendicular e un plan fata de camera
                //Vrem cel mai perpendicular plan ca intersectia sa fie cat mai putin "vaga"
                Ray cursorRay(m_cam.position(), unprojectPoint(xMove, yMove, m_cam.quat()));
                const double dot1 = dot(cursorRay.direction, axisNormals[0]); //XZ
                const double dot2 = dot(cursorRay.direction, axisNormals[1]); //YZ
                bool chosenPlane = (fabs(dot1) > fabs(dot2) ? 0 : 1);
                if (fabs(dot(axisNormals[chosenPlane], cursorRay.direction)) > moveErr) {
                    aux = rayCastOnPlane(xMove, yMove, axisNormals[chosenPlane], center);
                    if (!isLocal) {
                        m_meshes[m_selected].translate(0, 0, aux.z - center.z);
                    }
                    else {
                        aux -= center;
                        aux.rotateByUnitQuat(quat.inverse());
                        aux.x = aux.y = 0;
                        aux.rotateByUnitQuat(quat);
                        m_meshes[m_selected].translate(aux);
                    }
                }
                break;
            }
            default:;
        }
        m_updated[m_selected] = true;
        return true;
    }
    return false;
}

void Space3D::moveMesh() {
    short moveAxis = 0;
    bool& isLocal = m_localTransforms;
    Mesh original = m_meshes[m_selected];
    int xMove = -1, yMove = -1;
    while (!ismouseclick(WM_LBUTTONDOWN)) {
        if (kbhit()) {
            char x = getch();
            switch(x) {
                case 27:
                    m_meshes[m_selected] = original;
                    m_updated[m_selected] = true;
                    callHandlerDrawer();
                    return;
                case 'l':
                    updateTransform();
                    m_meshes[m_selected] = original;
                    moveMeshHelper(xMove, yMove, moveAxis, isLocal);
                    callHandlerDrawer();
                    break;
                case 'x': {
                    moveAxis = 0;
                    m_meshes[m_selected] = original;
                    xMove = mousex(); yMove = mousey();
                    if (moveMeshHelper(xMove, yMove, moveAxis, isLocal)) {
                        callHandlerDrawer();
                    }
                    break;
                }
                case 'y': {
                    moveAxis = 1;
                    m_meshes[m_selected] = original;
                    xMove = mousex(); yMove = mousey();
                    if (moveMeshHelper(xMove, yMove, moveAxis, isLocal)) {
                        callHandlerDrawer();
                    }
                    break;
                }
                case 'z': {
                    moveAxis = 2;
                    m_meshes[m_selected] = original;
                    xMove = mousex(); yMove = mousey();
                    if (moveMeshHelper(xMove, yMove, moveAxis, isLocal)) {
                        callHandlerDrawer();
                    }
                    break;
                }
            }
        }
        getmouseclick(WM_MOUSEMOVE, xMove, yMove);
        if (insideWorkArea(xMove, yMove)) {
            if (moveMeshHelper(xMove, yMove, moveAxis, isLocal)) {
                callHandlerDrawer();
            }
        }
    }
    clearmouseclick(WM_LBUTTONDOWN);
}

Point3D Space3D::unprojectPoint(int x, int y, const Quaternion& camQuat, double dy) const {
    const double xCenter = (x0 + x1) / 2;
    const double yCenter = (y0 + y1) / 2;
    const int yLen = y1 - y0;
    double dx = (x - xCenter) * dy  / m_cam.EZ() / yLen;
    double dz = (y - yCenter) * -1 * dy  / m_cam.EZ() / yLen;
    Point3D aux(dx, dy, dz);
    aux.rotateByUnitQuat(camQuat);
    aux += Point3D(m_cam.position());
    return aux;
}

Point3D Space3D::rayCastOnPlane(int x, int y, const Point3D& planeNormal, const Point3D& planeCenter) {
    Point3D rayCenter = m_cam.position();
    Ray cameraRay(rayCenter, unprojectPoint(x, y, m_cam.quat()));
    const double t = dot(planeNormal, planeCenter - rayCenter) / dot(planeNormal, cameraRay.direction);
    Point3D projectedPoint = rayCenter + cameraRay.direction * t;
    return projectedPoint;
}

void Space3D::dragAndDrop(const int& xDrag, const int& yDrag, Mesh& mesh, const Quaternion& camQuat, const Quaternion& camInverse) {
    const double xCenter = (x0 + x1) / 2;
    const double yCenter = (y0 + y1) / 2;
    const int yLen = y1 - y0;
    Point3D centerPoint(mesh.centerPoint());
    Point3D normalizedPoint(normalisePoint(centerPoint, camQuat, camInverse));
    Point2D sectionCenterPoint = projectPoint(mesh.centerPoint(), camQuat, camInverse);
    double xC = sectionCenterPoint.getX();
    double yC = sectionCenterPoint.getY();
    double bx1 = (xC - xCenter) / yLen;
    double by1 = (yC - yCenter) / yLen * -1;
    double bx2 = (xDrag - xCenter) / yLen;
    double by2 = (yDrag - yCenter) / yLen * -1;
    double dx1 = normalizedPoint.getX();
    double dy1 = normalizedPoint.getY();
    double dz1 = normalizedPoint.getZ();
    double EZ = m_cam.EZ();
    double dy2 = dy1;
    double dx2 = dx1 + (bx2 - bx1) * dy2 / EZ;
    double dz2 = dz1 + (by2 - by1) * dy2 / EZ;
    Point3D aux = Point3D(dx2, dy2, dz2).rotatedByUnitQuat(camQuat);
    double tx = aux.getX();
    double ty = aux.getY();
    double tz = aux.getZ();
    tx = tx + m_cam.position().getX();
    ty = ty + m_cam.position().getY();
    tz = tz + m_cam.position().getZ();
    mesh.translate(tx - centerPoint.getX(), ty - centerPoint.getY(), tz - centerPoint.getZ());
}

bool Space3D::isDragAndDrop(const int& xDrag, const int& yDrag) const {
    return x0 + 8 <= xDrag && xDrag <= x1 - 8 && y0 + 8 <= yDrag && yDrag <= y1 - 8;
}

void Space3D::highlightMesh() {
    for (int i = 0; i < m_selected; ++i) {
        m_sections[i].draw(ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR], ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR], ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR]);
    }
    if (m_selected != -1) {
        int highlightedColor = ColorSchemes::mixColors(ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR], RGB(255, 255, 255), 60);
        m_sections[m_selected].draw(ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR], highlightedColor, highlightedColor);
    }
    for (size_t i = m_selected + 1; i < size(); ++i) {
        m_sections[i].draw(ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR], ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR], ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR]);
    }
}

void Space3D::selectMesh(const size_t& index) {
    m_selected = index;
    const int x = m_spinballButton.getXCenter();
    const int y = m_spinballButton.getYCenter();
    m_buttonOX = CircularButton(x + 40, y + 120, 7);
    m_buttonOY = CircularButton(x + 40, y + 220, 7);
    m_buttonOZ = CircularButton(x + 40, y + 320, 7);
    m_meshes[m_selected].rotateDisplayAngle();
}

void Space3D::draw() {
    setfillstyle(SOLID_FILL, ColorSchemes::themeColors[m_theme][ColorSchemes::PRIMARYCOLOR]);
    bar(x0, y0, x1, y1);
    render();
    setlinestyle(SOLID_LINE, 0, 1);
    highlightMesh();
    if (m_fadedDrag) {
        m_fadedDrag = 0;
        m_draggedSection.draw(RGB(150, 150, 150), ColorSchemes::NO_COLOR, ColorSchemes::NO_COLOR);
    }
}

MyArray<char, 512> Space3D::getLinkedFileName() {
    return m_linkedFile.data();
}

void Space3D::setLinkedFileName(const MyArray<char, 512>& name) {
    m_linkedFile = name;
}

bool Space3D::isLinkedWithFile() {
    return m_linkedFile[0];
}

Mesh& Space3D::meshAt(const size_t& index) {
    return m_meshes[index];
}

const int& Space3D::selected() const{
    return m_selected;
}

void Space3D::setMeshAt(const size_t& index, const Mesh& mesh) {
    m_meshes[index] = mesh;
    m_updated[index] = true;
}

const Mesh& Space3D::meshAt(const size_t& index) const {
    return m_meshes[index];
}

Section& Space3D::sectionAt(const size_t& index) {
    return m_sections[index];
}

const Section& Space3D::sectionAt(const size_t& index) const {
    return m_sections[index];
}

FixedSpace3D::FixedSpace3D(const int theme, const Mesh& mesh, ObjectCreator* objectCreatorHandler) :
    x0(), y0(), x1(), y1(), m_theme(theme), m_arrowLeft(), m_arrowRight(), m_arrowUp(), m_arrowDown(),
    m_objCreatorHolder(objectCreatorHandler), m_cam(), m_mesh(mesh), m_updated(true) {
    initButtons();
}

void FixedSpace3D::setCorners(const int x0_, const int y0_, const int x1_, const int y1_) {
    x0 = x0_;
    y0 = y0_;
    x1 = x1_;
    y1 = y1_;
    initButtons();
}

void FixedSpace3D::initButtons() {
    static const int LEN_BUTTON = 17;
    const int xCenter = (x0 + x1) / 2;
    const int yCenter = (y0 + y1) / 2;
    m_arrowUp = Button(xCenter, y0 + LEN_BUTTON, 60, 2 * LEN_BUTTON);
    m_arrowDown = Button(xCenter, y1 - LEN_BUTTON, 60, 2 * LEN_BUTTON);
    m_arrowLeft = Button(x0 + LEN_BUTTON, yCenter, 2 * LEN_BUTTON, 60);
    m_arrowRight = Button(x1 - LEN_BUTTON, yCenter, 2 * LEN_BUTTON, 60);
}

void FixedSpace3D::callHandlerDrawer() {
    m_objCreatorHolder->draw();
}

void FixedSpace3D::render() {
    projectSection();
}

void FixedSpace3D::draw() {
    const MyArray<int, 3>& tC = ColorSchemes::themeColors[m_theme];
    m_mesh.drawMesh(tC[ColorSchemes::PRIMARYCOLOR]);
    drawArrows();
}

void FixedSpace3D::drawArrows() {
    const int BORDER_OFFSET = 5;
    const int POSITION_OFFSET = 25;
    const int xCenter = (x0 + x1) / 2;
    const int yCenter = (y0 + y1) / 2;
    setcolor(ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR]);
    setlinestyle(SOLID_LINE, 0, 2);
    line(x0 + BORDER_OFFSET, yCenter, x0 + POSITION_OFFSET, yCenter - POSITION_OFFSET);
    line(x0 + BORDER_OFFSET, yCenter, x0 + POSITION_OFFSET, yCenter + POSITION_OFFSET);
    line(x1 - BORDER_OFFSET, yCenter, x1 - POSITION_OFFSET, yCenter - POSITION_OFFSET);
    line(x1 - BORDER_OFFSET, yCenter, x1 - POSITION_OFFSET, yCenter + POSITION_OFFSET);
    line(xCenter, y0 + BORDER_OFFSET, xCenter - POSITION_OFFSET, y0 + POSITION_OFFSET);
    line(xCenter, y0 + BORDER_OFFSET, xCenter + POSITION_OFFSET, y0 + POSITION_OFFSET);
    line(xCenter, y1 - BORDER_OFFSET, xCenter - POSITION_OFFSET, y1 - POSITION_OFFSET);
    line(xCenter, y1 - BORDER_OFFSET, xCenter + POSITION_OFFSET, y1 - POSITION_OFFSET);
}

bool FixedSpace3D::getCommand() {
    int x, y;
    getmouseclick(WM_LBUTTONDOWN, x, y);
    return getCommand(x, y);
}

bool FixedSpace3D::getCommand(const int x, const int y) {
    if (x == -1) {
        return false;
    }
    if (checkAxisRotation(x, y)) {
        return true;
    }
    return false;
}

bool FixedSpace3D::insideWorkArea(const int x, const int y) const {
    return x0 <= x && x <= x1 && y0 <= y && y <= y1;
}

bool FixedSpace3D::insideWorkArea(const Point2D& point) const {
    return insideWorkArea(point.x, point.y);
}

void FixedSpace3D::run() {
    render();
    draw();
}

Point2D FixedSpace3D::projectPoint(const IntegerPoint3D& point, const Quaternion& camQuat, const Quaternion& camInverse) const {
    const int xCenter = (x0 + x1) / 2;
    const int yCenter = (y0 + y1) / 2;
    const int yLen = (y1 - y0);
    const int xr = point.x - m_cam.position().getX();
    const int yr = point.y - m_cam.position().getY();
    const int zr = point.z - m_cam.position().getZ();
    double EZ = m_cam.EZ();
    Quaternion pointQuat(0, Point3D(xr, yr, zr).toArray());
    Quaternion rotatedPct = Quaternion(camInverse * pointQuat * camQuat);
    double dx = rotatedPct[1];
    double dy = rotatedPct[2];
    double dz = rotatedPct[3];
    if (dy <= 0) {
        return Point2D(-100, -100);
    }
    double xprim = EZ * dx * yLen / dy + xCenter;
    double yprim = EZ * dz * yLen / dy * -1 + yCenter;
    return Point2D(xprim, yprim);
}

void FixedSpace3D::projectSection() {
    Quaternion camQuat = m_cam.quat();
    Quaternion camInverse = camQuat.inverse();
    for (FixedMesh::iterator_type it = m_mesh.begin(); it != m_mesh.end(); ++it) {
        m_mesh.renderPoint(it, projectPoint(*it, camQuat, camInverse));
    }
}

bool FixedSpace3D::getKeyCommand() {
    if (!kbhit()) {
        return false;
    }
    char x;
    do {
        x = getch();
    } while (kbhit());
    return checkCamMovement(x);
}

bool FixedSpace3D::checkCamMovement(const char x) {
    const int distance = 25;
    Point3D auxPoint;
    switch (x) {
        case 'a':
            auxPoint = Point3D(-distance, 0, 0);
            break;
        case 'd':
            auxPoint = Point3D(distance, 0, 0);
            break;
        case 's':
            auxPoint = Point3D(0, -distance, 0);
            break;
        case 'w':
            auxPoint = Point3D(0, distance, 0);
            break;
        case 'e':
            auxPoint = Point3D(0, 0, distance);
            break;
        case 'q':
            auxPoint = Point3D(0, 0, -distance);
            break;
        default:
            return 0;
    }
    Point3D rotatedPoint = rotateByCamera(auxPoint);
    m_cam.modifyPosition(rotatedPoint.x, rotatedPoint.y, rotatedPoint.z);
    callHandlerDrawer();
    return 1;
}

bool FixedSpace3D::checkAxisRotation(const int x, const int y) {
    constexpr double GRAD_ONEHALF = 0.0078539816339745;
    clearmouseclick(WM_LBUTTONUP);
    if (m_arrowDown.hitCollision(x, y)) {
        while (!ismouseclick(WM_LBUTTONUP)) {
            m_cam.rotateOnAxis(0, -GRAD_ONEHALF);
            callHandlerDrawer();
        }
        return true;
    }
    if (m_arrowUp.hitCollision(x, y)) {
        while (!ismouseclick(WM_LBUTTONUP)) {
            m_cam.rotateOnAxis(0, GRAD_ONEHALF);;
            callHandlerDrawer();
        }
        return true;
    }
    if (m_arrowLeft.hitCollision(x, y)) {
        while (!ismouseclick(WM_LBUTTONUP)) {
            m_cam.rotateOnAxis(2, GRAD_ONEHALF);
            callHandlerDrawer();
        }
        return true;
    }
    if (m_arrowRight.hitCollision(x, y)) {
        while (!ismouseclick(WM_LBUTTONUP)) {
            m_cam.rotateOnAxis(2, -GRAD_ONEHALF);
            callHandlerDrawer();
        }
        return true;
    }
    return false;
}

FixedMesh& FixedSpace3D::mesh() {
    return m_mesh;
}

Point3D FixedSpace3D::rotateByCamera(const Point3D& pct) const {
    double xr = pct.x;
    double yr = pct.y;
    double zr = pct.z;
    Point3D aux = Point3D(xr, yr, zr).rotatedByUnitQuat(m_cam.quat());
    return Point3D(aux.x, aux.y, aux.z);
}
