#include "Space3D.h"
#include "Menu.h"
#include "ObjectCreator.h"
#include <iostream>

Space3D::Space3D() :
    x0(), y0(), x1(), y1(), m_theme(), m_selected(-1), m_spinballSelected(false), m_fadedDrag(false), m_meshes(), m_draggedMesh(), m_sections(), m_draggedSection(),
    m_updated(), m_cam(), m_buttonOX(), m_buttonOY(), m_buttonOZ(), m_donutOX(), m_donutOY(), m_donutOZ(), m_spinballButton(), m_arrowLeft(), m_arrowRight(),
    m_arrowUp(), m_arrowDown(), m_arrowSpinLeft(), m_arrowSpinRight(), m_linkedFile{0}, m_menuHolder(nullptr), m_objCreatorHolder(nullptr) {}

Space3D::Space3D(const double& maxRadius, const int& theme, Menu* menuHolder) :
    x0(), y0(), x1(), y1(), m_theme(theme), m_selected(-1), m_spinballSelected(false), m_fadedDrag(false), m_meshes(), m_draggedMesh(), m_sections(), m_draggedSection(),
    m_updated(), m_cam(maxRadius), m_buttonOX(), m_buttonOY(), m_buttonOZ(), m_donutOX(), m_donutOY(), m_donutOZ(), m_spinballButton(), m_arrowLeft(), m_arrowRight(),
    m_arrowUp(), m_arrowDown(), m_arrowSpinLeft(), m_arrowSpinRight(), m_linkedFile{0}, m_menuHolder(menuHolder), m_objCreatorHolder(nullptr) {}

Space3D::Space3D(const double& maxRadius, const int& theme, ObjectCreator* objCreatorHolder) :
    x0(), y0(), x1(), y1(), m_theme(theme), m_selected(-1), m_spinballSelected(false), m_fadedDrag(false), m_meshes(), m_draggedMesh(), m_sections(), m_draggedSection(),
    m_updated(), m_cam(maxRadius), m_buttonOX(), m_buttonOY(), m_buttonOZ(), m_donutOX(), m_donutOY(), m_donutOZ(), m_spinballButton(), m_arrowLeft(), m_arrowRight(),
    m_arrowUp(), m_arrowDown(), m_arrowSpinLeft(), m_arrowSpinRight(), m_linkedFile{0}, m_menuHolder(nullptr), m_objCreatorHolder(objCreatorHolder) {}

Space3D::Space3D(const Space3D& sp) :
    x0(sp.x0), y0(sp.y0), x1(sp.x1), y1(sp.y1), m_theme(sp.m_theme), m_selected(sp.m_selected), m_spinballSelected(sp.m_spinballSelected), m_fadedDrag(sp.m_fadedDrag),
    m_meshes(sp.m_meshes), m_draggedMesh(sp.m_draggedMesh), m_sections(sp.m_sections), m_draggedSection(sp.m_draggedSection),
    m_updated(sp.m_updated), m_cam(sp.m_cam), m_buttonOX(sp.m_buttonOX), m_buttonOY(sp.m_buttonOY), m_buttonOZ(sp.m_buttonOZ), m_donutOX(sp.m_donutOX), m_donutOY(sp.m_donutOY),
    m_donutOZ(sp.m_donutOZ), m_spinballButton(sp.m_spinballButton), m_arrowLeft(sp.m_arrowLeft), m_arrowRight(sp.m_arrowRight),
    m_arrowUp(sp.m_arrowUp), m_arrowDown(sp.m_arrowDown), m_arrowSpinLeft(sp.m_arrowSpinLeft), m_arrowSpinRight(sp.m_arrowSpinRight), m_linkedFile(sp.m_linkedFile),
    m_menuHolder(sp.m_menuHolder), m_objCreatorHolder(sp.m_objCreatorHolder) {}

Space3D& Space3D::operator = (const Space3D& sp) {
    m_theme = sp.m_theme;
    m_fadedDrag = sp.m_fadedDrag;
    m_draggedMesh = sp.m_draggedMesh;
    m_draggedSection = sp.m_draggedSection;
    m_cam = sp.m_cam;
    m_meshes = sp.m_meshes;
    m_sections = sp.m_sections;
    m_updated = sp.m_updated;
    m_selected = sp.m_selected;
    m_linkedFile = sp.m_linkedFile;
    m_menuHolder = sp.m_menuHolder;
    m_objCreatorHolder = sp.m_objCreatorHolder;
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
    m_donutOX = DonutButton(xSpinballCenter + 20, ySpinballCenter + 120, 40, 20);
    m_donutOY = DonutButton(xSpinballCenter + 20, ySpinballCenter + 220, 40, 20);
    m_donutOZ = DonutButton(xSpinballCenter + 20, ySpinballCenter + 320, 40, 20);
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
    if (m_selected != -1) {
        drawSpinball();
        if (m_spinballSelected) {
            showAngleOptions();
        }
    }
}

void Space3D::update() {
    m_updated.fill(true);
}

void Space3D::drawRotationArrows() {
    const int BORDER_OFFSET = 5;
    const int POSITION_OFFSET = 25;
    const int xCenter = (x0 + x1) / 2;
    const int yCenter = (y0 + y1) / 2;
    setcolor(ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR]);
    setlinestyle(SOLID_LINE, 0, 2);
    //m_arrowLeft.border(WHITE);
    line(x0 + BORDER_OFFSET, yCenter, x0 + POSITION_OFFSET, yCenter - POSITION_OFFSET);
    line(x0 + BORDER_OFFSET, yCenter, x0 + POSITION_OFFSET, yCenter + POSITION_OFFSET);
    //m_arrowRight.border(WHITE);
    line(x1 - BORDER_OFFSET, yCenter, x1 - POSITION_OFFSET, yCenter - POSITION_OFFSET);
    line(x1 - BORDER_OFFSET, yCenter, x1 - POSITION_OFFSET, yCenter + POSITION_OFFSET);
    //m_arrowUp.border(WHITE);
    line(xCenter, y0 + BORDER_OFFSET, xCenter - POSITION_OFFSET, y0 + POSITION_OFFSET);
    line(xCenter, y0 + BORDER_OFFSET, xCenter + POSITION_OFFSET, y0 + POSITION_OFFSET);
    //m_arrowDown.border(WHITE);
    line(xCenter, y1 - BORDER_OFFSET, xCenter - POSITION_OFFSET, y1 - POSITION_OFFSET);
    line(xCenter, y1 - BORDER_OFFSET, xCenter + POSITION_OFFSET, y1 - POSITION_OFFSET);
    //m_arrowSpinRight.border(WHITE);
    ellipse(x0 + 25, y0 + 25, 90, 360, 15, 15);
    line(x0 + 27, y0 + 11, x0 + 20, y0 + 4);
    line(x0 + 27, y0 + 11, x0 + 20, y0 + 18);
    //m_arrowSpinLeft.border(WHITE);
    ellipse(x0 + 25, y1 - 25, 180, 450, 15, 15);
    line (x0 + 23, y1 - 39, x0 + 30, y1 - 46);
    line (x0 + 23, y1 - 39, x0 + 30, y1 - 32);
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
    m_buttonOX.drawLabel(LIGHTGREEN, LIGHTGREEN);

    m_donutOY.drawLabel(ColorSchemes::NO_COLOR, LIGHTRED);
    m_buttonOY.drawLabel(LIGHTRED, LIGHTRED);

    m_donutOZ.drawLabel(ColorSchemes::NO_COLOR, LIGHTBLUE);
    m_buttonOZ.drawLabel(LIGHTBLUE, LIGHTBLUE);
}


bool Space3D::checkAxisRotation(const int& x, const int& y) {
    const double Grad_1 = 0.0078539816339745;
    clearmouseclick(WM_LBUTTONUP);
    if (m_arrowDown.hitCollision(x, y)) {
        while (!ismouseclick(WM_LBUTTONUP)) {
            m_cam.modifyAngles(-Grad_1, 0, 0);
            m_updated.fill(true);
            callHandlerDrawer();
        }
        return true;
    }
    if (m_arrowUp.hitCollision(x, y)) {
        while (!ismouseclick(WM_LBUTTONUP)) {
            m_cam.modifyAngles(+Grad_1, 0, 0);
            m_updated.fill(true);
            callHandlerDrawer();
        }
        return true;
    }
    if (m_arrowLeft.hitCollision(x, y)) {
        while (!ismouseclick(WM_LBUTTONUP)) {
            m_cam.modifyAngles(0, 0, +Grad_1);
            m_updated.fill(true);
            callHandlerDrawer();
        }
        return true;
    }
    if (m_arrowRight.hitCollision(x, y)) {
        while (!ismouseclick(WM_LBUTTONUP)) {
            m_cam.modifyAngles(0, 0, -Grad_1);
            m_updated.fill(true);
            callHandlerDrawer();
        }
        return true;
    }
    if (m_arrowSpinLeft.hitCollision(x, y)) {
        while (!ismouseclick(WM_LBUTTONUP)) {
            m_cam.modifyAngles(0, +Grad_1, 0);
            m_updated.fill(true);
            callHandlerDrawer();
        }
        return true;
    }
    if (m_arrowSpinRight.hitCollision(x, y)) {
        while (!ismouseclick(WM_LBUTTONUP)) {
            m_cam.modifyAngles(0, -Grad_1, 0);
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

Point3D Space3D::rotateByCamera(const Point3D& pct) const {
    //Folosit in cam movement; "deznormalizeaza" rotatia vectorilor canonici in relatie cu camera, pt a fi accurate in 3D
    //si a parea practic ca se misca pe axele locale ale camerei
    double xr = pct.getX();
    double yr = pct.getY();
    double zr = pct.getZ();
    double aX = m_cam.angleX();
    double aY = m_cam.angleY();
    double aZ = m_cam.angleZ();
    double dx = (-sin(aX) * sin(aY) * sin(aZ) + cos(aZ) * cos(aY)) * xr +  (-sin(aZ) * cos(aX)) * yr + (sin(aY) * cos(aZ) + sin(aX) * sin(aZ) * cos(aY)) * zr;
    double dy = (sin(aX) * sin(aY) * cos(aZ) + cos(aY) * sin(aZ)) * xr + (cos(aX) * cos(aZ)) * yr +  (sin(aY) * sin(aZ) - sin(aX) * cos(aZ) * cos(aY))* zr;
    double dz = -sin(aY) * cos(aX) * xr + sin(aX) * yr + cos(aX) * cos(aY) * zr;
    return Point3D(dx, dy, dz);
}

Point3D Space3D::normalisePoint(const Point3D& pct) const {
    double xr = pct.getX() - m_cam.position().getX();
    double yr = pct.getY() - m_cam.position().getY();
    double zr = pct.getZ() - m_cam.position().getZ();
    double aX = m_cam.angleX();
    double aY = m_cam.angleY();
    double aZ = m_cam.angleZ();
    double dx = (-sin(aX) * sin(aY) * sin(aZ) + cos(aZ) * cos(aY)) * xr + (sin(aX) * sin(aY) * cos(aZ) + cos(aY) * sin(aZ)) * yr - sin(aY) * cos(aX) * zr;
    double dy = -sin(aZ) * cos(aX) * xr + (cos(aX) * cos(aZ)) * yr + sin(aX) * zr;
    double dz = (sin(aY) * cos(aZ) + sin(aX) * sin(aZ) * cos(aY)) * xr + (sin(aY) * sin(aZ) - sin(aX) * cos(aZ) * cos(aY)) * yr + cos(aX) * cos(aY) * zr;
    return Point3D(dx, dy, dz);
}

Point2D Space3D::projectPoint(const Point3D& pct) const {
    const int xCenter = (x0 + x1) / 2;
    const int yCenter = (y0 + y1) / 2;
    const int yLen = (y1 - y0);
    const int xr = pct.getX() - m_cam.position().getX();
    const int yr = pct.getY() - m_cam.position().getY();
    const int zr = pct.getZ() - m_cam.position().getZ();
    double aX = m_cam.angleX();
    double aY = m_cam.angleY();
    double aZ = m_cam.angleZ();
    double EZ = m_cam.EZ();
    double dx = (-sin(aX) * sin(aY) * sin(aZ) + cos(aZ) * cos(aY)) * xr + (sin(aX) * sin(aY) * cos(aZ) + cos(aY) * sin(aZ)) * yr - sin(aY) * cos(aX) * zr;
    double dy = -sin(aZ) * cos(aX) * xr + (cos(aX) * cos(aZ)) * yr + sin(aX) * zr;
    double dz = (sin(aY) * cos(aZ) + sin(aX) * sin(aZ) * cos(aY)) * xr + (sin(aY) * sin(aZ) - sin(aX) * cos(aZ) * cos(aY)) * yr + cos(aX) * cos(aY) * zr;
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
    for(size_t i = 0; i < mesh.size(); i++) {
        projectedPoints[i] = projectPoint(mesh[i]);
    }
    if (!m_menuHolder) {
        return Section(projectedPoints, Point2D(-100, -100), mesh.adjacencyList());
    }
    Point2D sectionCenterPoint = projectPoint(mesh.centerPoint());
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
    if (m_donutOX.hitCollision(x, y)) {
        int xDrag, yDrag;
        getmouseclick(WM_LBUTTONUP, xDrag, yDrag);
        int dragDiff = sqrt((xDrag - x) * (xDrag - x) + (yDrag - y) * (yDrag - y));
        while (!ismouseclick(WM_LBUTTONUP)) {
            getmouseclick(WM_MOUSEMOVE, xDrag, yDrag);
            if (m_donutOX.hitCollision(xDrag, yDrag) && dragDiff >= 5) {
                double rotation = findRotation(xDrag, yDrag, m_donutOX, m_buttonOX);
                m_meshes[m_selected].rotate(rotation, 0, 0);
                selectMesh(m_selected);
                m_updated[m_selected] = true;
                callHandlerDrawer();
                x = xDrag;
                y = yDrag;
            }
            dragDiff = sqrt((xDrag - x) * (xDrag - x) + (yDrag - y) * (yDrag - y));
        }
        return true;
    }
    if (m_donutOY.hitCollision(x, y)) {
        int xDrag, yDrag;
        getmouseclick(WM_LBUTTONUP, xDrag, yDrag);
        int dragDiff = sqrt((xDrag - x) * (xDrag - x) + (yDrag - y) * (yDrag - y));
        while (!ismouseclick(WM_LBUTTONUP)) {
            getmouseclick(WM_MOUSEMOVE, xDrag, yDrag);
            if (m_donutOY.hitCollision(xDrag, yDrag) && dragDiff >= 5) {
                double rotation = findRotation(xDrag, yDrag, m_donutOY, m_buttonOY);
                m_meshes[m_selected].rotate(0, rotation, 0);
                selectMesh(m_selected);
                m_updated[m_selected] = true;
                callHandlerDrawer();
                x = xDrag;
                y = yDrag;
            }
            dragDiff = sqrt((xDrag - x) * (xDrag - x) + (yDrag - y) * (yDrag - y));
        }
        return true;
    }
    if (m_donutOZ.hitCollision(x, y)) {
        int xDrag, yDrag;
        getmouseclick(WM_LBUTTONUP, xDrag, yDrag);
        int dragDiff = sqrt((xDrag - x) * (xDrag - x) + (yDrag - y) * (yDrag - y));
        while (!ismouseclick(WM_LBUTTONUP)) {
            getmouseclick(WM_MOUSEMOVE, xDrag, yDrag);
            if (m_donutOZ.hitCollision(xDrag, yDrag) && dragDiff >= 5) {
                double rotation = findRotation(xDrag, yDrag, m_donutOZ, m_buttonOZ);
                m_meshes[m_selected].rotate(0, 0, rotation);
                selectMesh(m_selected);
                m_updated[m_selected] = true;
                callHandlerDrawer();
                x = xDrag;
                y = yDrag;
            }
            dragDiff = sqrt((xDrag - x) * (xDrag - x) + (yDrag - y) * (yDrag - y));
        }
        return true;
    }
    return false;
}

bool Space3D::getKeyCommand() {
    if (!kbhit()) {
        return false;
    }
    return checkCamMovement(getch());
}

//returns true if menu should redraw itself
bool Space3D::getCommand(const int& x, const int& y) {
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
            callHandlerDrawer();
            dragMesh();
            return true;
        }
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
                dragAndDrop(xDrag, yDrag, m_draggedMesh);
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

void Space3D::dragAndDrop(const int& xDrag, const int& yDrag, Mesh& mesh) {
    const double xCenter = (x0 + x1) / 2;
    const double yCenter = (y0 + y1) / 2;
    const int yLen = y1 - y0;

    Point3D centerPoint(mesh.centerPoint());
    Point3D normalizedPoint(normalisePoint(centerPoint));

    //SCHEMA:
    //"Dezproiectam" obiectul de pe ecran, si l scoatem din sistemul de coordonate al camerei
    //(adica we undo the rotation si il departam de camera)
    //dy2 = dy1; sunt la fel de aproape de camera.
    //Scoatem dx2 si dz2 prin bx1 si bx2

    //xC, yC - centrul sect.; for accurate translations
    Point2D sectionCenterPoint = projectPoint(mesh.centerPoint());
    double xC = sectionCenterPoint.getX();
    double yC = sectionCenterPoint.getY();

    /*Inversam ecuatiile astea din projectPoint si getProjectPoint:
    pentru a afla b-urile, adica coordonatele proiectiei punctului
    intr-un sistem de coordonate de la (-1,-1) la (1,1),
    in loc de (x1,y1) la (x2, y2):
    */

    double bx1 = (xC - xCenter) / yLen;
    double by1 = (yC - yCenter) / yLen * -1;
    double bx2 = (xDrag - xCenter) / yLen;
    double by2 = (yDrag - yCenter) / yLen * -1;
    double dx1 = normalizedPoint.getX();
    double dy1 = normalizedPoint.getY();
    double dz1 = normalizedPoint.getZ();
    double EZ = m_cam.EZ();
    //DY DIFERIT de 0 ca asa l vrem. In proiectie vom stabili defapt ce se intampla
    //daca camera e prea aproape de un obiect
    double dy2 = dy1;
    double dx2 = dx1 + (bx2 - bx1) * dy2 / EZ;
    double dz2 = dz1 + (by2 - by1) * dy2 / EZ;

    //Aici inversam rotatia camerei...
    double aX = m_cam.angleX();
    double aY = m_cam.angleY();
    double aZ = m_cam.angleZ();
    double tx = ( -sin(aX) * sin(aY) * sin(aZ) + cos(aZ) * cos(aY) ) * dx2 +  (-sin(aZ) * cos(aX)) * dy2 + (sin(aY) * cos(aZ) + sin(aX) * sin(aZ) * cos(aY)) * dz2;
    double ty =  (sin(aX) * sin(aY) * cos(aZ) + cos(aY) * sin(aZ)) * dx2 + (cos(aX) * cos(aZ)) * dy2 +  (sin(aY) * sin(aZ) - sin(aX) * cos(aZ) * cos(aY))* dz2;
    double tz =  (- sin(aY) * cos(aX)) * dx2 + sin(aX)  * dy2 + cos(aX) * cos(aY) * dz2;

    //..si acum il scoatem complet din sistemul de coordonate al camerei.
    tx = tx + m_cam.position().getX();
    ty = ty + m_cam.position().getY();
    tz = tz + m_cam.position().getZ();

    //translatam
    mesh.translate(tx - centerPoint.getX(), ty - centerPoint.getY(), tz - centerPoint.getZ());
}

bool Space3D::isDragAndDrop(const int& xDrag, const int& yDrag) const {
    return x0 + 8 <= xDrag && xDrag <= x1 - 8 && y0 + 8 <= yDrag && yDrag <= y1 - 8;
}

void Space3D::highlightMesh() {
    for (int i = 0; i < m_selected; ++i) {
        m_sections[i].draw(ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR], RED, RED);
    }
    if (m_selected != -1) {
        m_sections[m_selected].draw(ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR], GREEN, GREEN);
    }
    for (size_t i = m_selected + 1; i < size(); ++i) {
        m_sections[i].draw(ColorSchemes::themeColors[m_theme][ColorSchemes::SECONDARYCOLOR], RED, RED);
    }
}

void Space3D::selectMesh(const size_t& index) {
    m_selected = index;
    const int x = m_spinballButton.getXCenter();
    const int y = m_spinballButton.getYCenter();

    m_buttonOX = CircularButton(x + 20 + 40 * cos(m_meshes[m_selected].angleX()), y + 120 + 40 * sin(m_meshes[m_selected].angleX()), 7);

    m_buttonOY = CircularButton(x + 20 + 40 * cos(m_meshes[m_selected].angleY()), y + 220 + 40 * sin(m_meshes[m_selected].angleY()), 7);

    m_buttonOZ = CircularButton(x + 20 + 40 * cos(m_meshes[m_selected].angleZ()), y + 320 + 40 * sin(m_meshes[m_selected].angleZ()), 7);
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

const Mesh& Space3D::meshAt(const size_t& index) const {
    return m_meshes[index];
}

Section& Space3D::sectionAt(const size_t& index) {
    return m_sections[index];
}

const Section& Space3D::sectionAt(const size_t& index) const {
    return m_sections[index];
}
