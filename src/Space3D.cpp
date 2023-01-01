#include "Space3D.h"
#include "Menu.h"
#include "ObjectCreator.h"
#include "Quaternion.h"

const double pi = 3.141926535897;
const double err = 0.0000000000000000000001;
const double moveErr = 0.007; //chosen by trial and error, used for moveMesh()

Space3D::Space3D() :
    x0(), y0(), x1(), y1(), m_theme(), m_selected(-1), m_spinballSelected(false), m_fadedDrag(false),  m_objRotateDrag(false), m_meshes(), m_draggedMesh(), m_sections(), m_draggedSection(),
    m_updated(), m_cam(), m_buttonOX(), m_buttonOY(), m_buttonOZ(), m_donutOX(), m_donutOY(), m_donutOZ(), m_spinballButton(), m_arrowLeft(), m_arrowRight(),
    m_arrowUp(), m_arrowDown(), m_arrowSpinLeft(), m_arrowSpinRight(), m_rightClickMenu(), m_rMenuOpen(false), m_gizmoButtons(), m_linkedFile{0}, m_menuHolder(nullptr), m_objCreatorHolder(nullptr) {}

Space3D::Space3D(const double& maxRadius, const int& theme, Menu* menuHolder) :
    x0(), y0(), x1(), y1(), m_theme(theme), m_selected(-1), m_spinballSelected(false), m_fadedDrag(false),  m_objRotateDrag(false), m_meshes(), m_draggedMesh(), m_sections(), m_draggedSection(),
    m_updated(), m_cam(maxRadius), m_buttonOX(), m_buttonOY(), m_buttonOZ(), m_donutOX(), m_donutOY(), m_donutOZ(), m_spinballButton(), m_arrowLeft(), m_arrowRight(),
    m_arrowUp(), m_arrowDown(), m_arrowSpinLeft(), m_arrowSpinRight(), m_rightClickMenu(), m_rMenuOpen(false), m_gizmoButtons(), m_linkedFile{0}, m_menuHolder(menuHolder), m_objCreatorHolder(nullptr) {}

Space3D::Space3D(const double& maxRadius, const int& theme, ObjectCreator* objCreatorHolder) :
    x0(), y0(), x1(), y1(), m_theme(theme), m_selected(-1), m_spinballSelected(false), m_fadedDrag(false), m_objRotateDrag(false), m_meshes(), m_draggedMesh(), m_sections(), m_draggedSection(),
    m_updated(), m_cam(maxRadius), m_buttonOX(), m_buttonOY(), m_buttonOZ(), m_donutOX(), m_donutOY(), m_donutOZ(), m_spinballButton(), m_arrowLeft(), m_arrowRight(),
    m_arrowUp(), m_arrowDown(), m_arrowSpinLeft(), m_arrowSpinRight(), m_rightClickMenu(), m_rMenuOpen(false), m_gizmoButtons(), m_linkedFile{0}, m_menuHolder(nullptr), m_objCreatorHolder(objCreatorHolder){}

Space3D::Space3D(const Space3D& sp) :
    x0(sp.x0), y0(sp.y0), x1(sp.x1), y1(sp.y1), m_theme(sp.m_theme), m_selected(sp.m_selected), m_spinballSelected(sp.m_spinballSelected), m_fadedDrag(sp.m_fadedDrag),  m_objRotateDrag(false),
    m_meshes(sp.m_meshes), m_draggedMesh(sp.m_draggedMesh), m_sections(sp.m_sections), m_draggedSection(sp.m_draggedSection),
    m_updated(sp.m_updated), m_cam(sp.m_cam), m_buttonOX(sp.m_buttonOX), m_buttonOY(sp.m_buttonOY), m_buttonOZ(sp.m_buttonOZ), m_donutOX(sp.m_donutOX), m_donutOY(sp.m_donutOY),
    m_donutOZ(sp.m_donutOZ), m_spinballButton(sp.m_spinballButton), m_arrowLeft(sp.m_arrowLeft), m_arrowRight(sp.m_arrowRight),
    m_arrowUp(sp.m_arrowUp), m_arrowDown(sp.m_arrowDown), m_arrowSpinLeft(sp.m_arrowSpinLeft), m_arrowSpinRight(sp.m_arrowSpinRight),
    m_rightClickMenu(sp.m_rightClickMenu), m_rMenuOpen(sp.m_rMenuOpen), m_gizmoButtons(sp.m_gizmoButtons), m_linkedFile(sp.m_linkedFile), m_menuHolder(sp.m_menuHolder), m_objCreatorHolder(sp.m_objCreatorHolder) {}

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
    m_rightClickMenu = DropdownButton<5>(-100, -100, 0, 0, "", 75, 5 * 20);
    m_rightClickMenu.addOption("Flip X");
    m_rightClickMenu.addOption("Flip Y");
    m_rightClickMenu.addOption("Flip Z");
    m_rightClickMenu.addOption("Scale");
    m_rightClickMenu.addOption("Edit mesh");
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

//porneste desenarea pe o parte a ecranului
void Space3D::run() {
    draw();
    drawRotationArrows();
    if (m_rMenuOpen) { //nu pot folosi toggleVisibility ca hideList imi deseneaza din pacate un bar peste
        struct linesettingstype oldSettings;
        getlinesettings(&oldSettings);
        setlinestyle(0, 0, 1);
        m_rightClickMenu.showList(0, 0, ColorSchemes::mixColors(RGB(222, 222, 222), ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR], 10), 0);
        setlinestyle(oldSettings.linestyle, oldSettings.upattern, oldSettings.thickness);
    }
    if (m_selected != -1) {
        drawGizmo();
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

void drawArrow(const Point2D& p1, const Point2D& p2, int color) {
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

void Space3D::updateGizmoButtons() {
    Point3D center(m_meshes[m_selected].centerPoint());
    MyArray<Point3D, 3> axes{Point3D(1, 0, 0), Point3D(0, 1, 0), Point3D(0, 0, 1)};
    const MyArray<Point3D, 2> corners(m_meshes[m_selected].getBoundingBoxCorners());
    const double xLen = fabs(corners[0].x - corners[1].x);
    const double yLen = fabs(corners[0].y - corners[1].y);
    const double zLen = fabs(corners[0].z - corners[1].z);
    const double maxLength = (xLen > yLen) ?
                             (xLen > zLen ? xLen : zLen) :
                             (yLen > zLen ? yLen : zLen);
    const double arrowLength = maxLength / 2;
    MyArray<Point3D, 3> axisPoints {Point3D(arrowLength, 0, 0),
                                    Point3D(0, -arrowLength, 0),
                                    Point3D(0, 0, arrowLength)};
    MyArray<Point2D, 3> projectedAxisPoints;
    for (size_t i = 0; i < 3; ++i) {
        projectedAxisPoints[i] = projectPoint(axisPoints[i] + center, m_cam.quat(), m_cam.quat().inverse());
        m_gizmoButtons[i] = CircularButton(projectedAxisPoints[i].x, projectedAxisPoints[i].y, 7);
    }
}

void Space3D::drawGizmo() {
    updateGizmoButtons();
    Point2D projectedCenter(projectPoint(m_meshes[m_selected].centerPoint(), m_cam.quat(), m_cam.quat().inverse()));
    drawArrow(projectedCenter, Point2D(m_gizmoButtons[0].getX(), m_gizmoButtons[0].getY()), LIGHTGREEN);
    drawArrow(projectedCenter, Point2D(m_gizmoButtons[1].getX(), m_gizmoButtons[1].getY()), LIGHTRED);
    drawArrow(projectedCenter, Point2D(m_gizmoButtons[2].getX(), m_gizmoButtons[2].getY()), LIGHTCYAN);
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
    //Am lasat in space3D angleX, angleY si angleZ - solely for displaying purposes
    //Transforma quaternionul curent in unghiuri Euler: yaw, pitch, roll
    //cu metoda toEuler din Quaternion
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
                m_meshes[m_selected].rotate(rotation, 0, 0);
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
                m_meshes[m_selected].rotate(0, rotation, 0);
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
                m_meshes[m_selected].rotate(0, 0, -rotation);
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
    const double Grad_1 = 0.0078539816339745;
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
    /*if (m_arrowSpinLeft.hitCollision(x, y)) {
        while (!ismouseclick(WM_LBUTTONUP)) {
            m_cam.rotateOnAxis(1, Grad_1);
            m_updated.fill(true);
            callHandlerDrawer();
        }
        return true;
    }
    if (m_arrowSpinRight.hitCollision(x, y)) {
        while (!ismouseclick(WM_LBUTTONUP)) {
            m_cam.rotateOnAxis(1, -Grad_1);
            m_updated.fill(true);
            callHandlerDrawer();
        }
        return true;
    }*/
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

//proiecteaza fiecare mesh daca a fost updated
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
    if (m_rMenuOpen) {
        m_rMenuOpen = false;
    }
    return checkCamMovement(x);
}

//returns true if menu should redraw itself
bool Space3D::getCommand(const int& x, const int& y) {
    if (m_selected != -1 && m_rMenuOpen && m_rightClickMenu.listHitCollision(x, y) != -1) {
        //TODO: switch
        int rMenuIndex = m_rightClickMenu.listHitCollision(x, y);
        if (rMenuIndex >= 0 && rMenuIndex<= 2) {
            m_rMenuOpen = false;
            m_meshes[m_selected].mirror(rMenuIndex);
            m_updated[m_selected] = true;
        }
        if (m_rightClickMenu.listHitCollision(x, y) == 3) {
            m_rMenuOpen = false;
            scaleMesh();
        }
        if (m_rightClickMenu.listHitCollision(x, y) == 4) {
            m_rMenuOpen = false;
            //copiat aproape direct din Menu::getCommand()
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
        return true;
    }
    else {
        if (m_rMenuOpen) {
            m_rMenuOpen = false;
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
            if (m_rMenuOpen) {
                m_rMenuOpen = false;
            }
            callHandlerDrawer();
            dragMesh();
            return true;
        }
    }
    if (insideWorkArea(x, y) && m_selected != -1) {
        m_selected = -1;
        if (m_rMenuOpen) {
            m_rMenuOpen = false;
        }
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
            selectMesh(i);
            if (x <= x1 - m_rightClickMenu.width() && y <= y1 - m_rightClickMenu.height()) {
                m_rightClickMenu.move(x, y);
            }
            else {
                m_rightClickMenu.move(x - m_rightClickMenu.width(), y - m_rightClickMenu.height());
            }
            m_rMenuOpen = true;
            return true;
        }
    }
    if (m_rMenuOpen) {
        m_rMenuOpen = false;
        m_selected = -1;
        return true;
    }
    return true;
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
    //swapbuffers trickery; deseneaza un "assistLine" peste imag curenta si aduce setarile inapoi
    //am facut o functie literalmente pt ca ma enerva sa am de vreo... 5 ori? bucata asta de cod in scaleMesh()
    swapbuffers();
    struct linesettingstype oldSettings;
    getlinesettings(&oldSettings);
    int oldColor = getcolor();
    setlinestyle(3, 0, 2);
    setcolor(RGB(128, 128, 128));
    line(x0_, y0_, x1_, y1_);
    setlinestyle(oldSettings.linestyle, oldSettings.upattern, oldSettings.thickness);
    setcolor(oldColor);
    swapbuffers();
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
                case 27: //ESC for "discard"
                    m_meshes[m_selected] = original;
                    m_updated[m_selected] = true;
                    callHandlerDrawer();
                    return;
                //-1 - global; 0, 1, 2 - axele X, Y, Z. Mereu locale
                //nu cred ca afecteaza asa tare faptu ca copiem mereu meshu Original in selected
                //dar daca vrei neaparat, pot sa schimb asta
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
}

void Space3D::moveMeshHelper(int xMove, int yMove, int moveAxis, bool isLocal) {
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
        callHandlerDrawer();
    }
}

void Space3D::moveMesh() {
    //nu ma folosesc de draggedMesh... ma gandesc cum am sa implementez move cu un gizmo si vad daca am sa schimb cu un draggedMesh
    //TODO: draw Axis? perhaps? da nici nu stiu cum as face
    short moveAxis = 0;
    bool isLocal = 0;
    Mesh original = m_meshes[m_selected];
    int xMove = -1, yMove = -1;
    while (!ismouseclick(WM_LBUTTONDOWN)) {
        if (kbhit()) {
            char x = getch();
            switch(x) {
                case 27: //ESC for "discard"
                    m_meshes[m_selected] = original;
                    m_updated[m_selected] = true;
                    callHandlerDrawer();
                    return;
                case 'l':
                    isLocal = !isLocal;
                    m_meshes[m_selected] = original;
                    moveMeshHelper(xMove, yMove, moveAxis, isLocal);
                    break;
                case 'x': {
                    moveAxis = 0;
                    m_meshes[m_selected] = original;
                    xMove = mousex(); yMove = mousey();
                    moveMeshHelper(xMove, yMove, 0, isLocal);
                    break;
                }
                case 'y': {
                    moveAxis = 1;
                    m_meshes[m_selected] = original;
                    xMove = mousex(); yMove = mousey();
                    moveMeshHelper(xMove, yMove, 1, isLocal);
                    break;
                }
                case 'z': {
                    moveAxis = 2;
                    m_meshes[m_selected] = original;
                    xMove = mousex(); yMove = mousey();
                    moveMeshHelper(xMove, yMove, 2, isLocal);
                    break;
                }
            }
        }
        getmouseclick(WM_MOUSEMOVE, xMove, yMove);
        if (insideWorkArea(xMove, yMove)) {
            moveMeshHelper(xMove, yMove, moveAxis, isLocal);
        }
    }
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

//deseneaza toate mesh-urile proiectate + centrele
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
