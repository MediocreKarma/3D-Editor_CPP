#include "Space3D.h"
#include <iostream>

static const int PRIMARYCOLOR = 0;
static const int SECONDARYCOLOR = 1;
static const MyArray<MyArray<int, 2>, 2> themeColors = {
    {WHITE, BLACK}, {BLACK, WHITE}
};
static const int NO_COLOR = -1;

Space3D::Space3D(const double& maxRadius, const int& theme) :
    x0(), y0(), x1(), y1(), m_theme(theme), m_selected(-1), m_spinballSelected(false), m_meshes(), m_sections(), m_updated(), m_cam(maxRadius),
    m_buttonOX(), m_buttonOY(), m_buttonOZ(), m_donutOX(), m_donutOY(), m_donutOZ(), m_spinballButton(), m_linkedFile{0} {}

void Space3D::setCorners(const int& x0_, const int& y0_, const int& x1_, const int& y1_) {
    x0 = x0_;
    y0 = y0_;
    x1 = x1_;
    y1 = y1_;
}

size_t Space3D::size() const {
    return m_meshes.size();
}

void Space3D::addMesh(const Mesh& mesh) {
    m_meshes.push_back(mesh);
    m_sections.resize(size());
    m_updated.push_back(true);
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
    drawSpinball(x1 - 75, y0 + 75);
}

void Space3D::drawSpinball(const int& x, const int& y) {
    m_spinballButton = Button(x, y, 140, 140);
    m_spinballButton.drawLabel(themeColors[m_theme][PRIMARYCOLOR], themeColors[m_theme][SECONDARYCOLOR]);
    setlinestyle(SOLID_LINE, 0, 2);
    setcolor(LIGHTRED);
    CircularLabel labelOX = CircularLabel(x + 10 * cos(m_cam.angleX()), y + 60 * sin(m_cam.angleX()), 5);
    CircularLabel labelOY = CircularLabel(x + 60 * cos(m_cam.angleY()), y + 60 * sin(m_cam.angleY()), 5);
    CircularLabel labelOZ = CircularLabel(x + 60 * cos(m_cam.angleZ()), y + 10 * sin(m_cam.angleZ()), 5);
    circle(x, y, 60);
    labelOY.drawLabel(LIGHTRED, LIGHTRED);
    setcolor(LIGHTGREEN);
    ellipse(x, y, 0, 360, 10, 60);
    labelOX.drawLabel(LIGHTGREEN, LIGHTGREEN);
    setcolor(LIGHTBLUE);
    ellipse(x, y, 0, 360, 60, 10);
    labelOZ.drawLabel(LIGHTBLUE, LIGHTBLUE);
    if (m_spinballSelected) {
        showAngleOptions(x, y);
    }
}

void Space3D::showAngleOptions(const int& x, const int& y) {
    m_donutOX = DonutButton(x + 20, y + 120, 40, 20);
    m_donutOX.drawLabel(NO_COLOR, LIGHTGREEN);
    m_buttonOX = CircularButton(x + 20 + 40 * cos(m_cam.angleX()), y + 120 + 40 * sin(m_cam.angleX()), 7);
    m_buttonOX.drawLabel(LIGHTGREEN, LIGHTGREEN);
    m_donutOY = DonutButton(x + 20, y + 220, 40, 20);
    m_donutOY.drawLabel(NO_COLOR, LIGHTRED);
    m_buttonOY = CircularButton(x + 20 + 40 * cos(m_cam.angleY()), y + 220 + 40 * sin(m_cam.angleY()), 7);
    m_buttonOY.drawLabel(LIGHTRED, LIGHTRED);
    m_donutOZ = DonutButton(x + 20, y + 320, 40, 20);
    m_donutOZ.drawLabel(NO_COLOR, LIGHTBLUE);
    m_buttonOZ = CircularButton(x + 20 + 40 * cos(m_cam.angleZ()), y + 320 + 40 * sin(m_cam.angleZ()), 7);
    m_buttonOZ.drawLabel(LIGHTBLUE, LIGHTBLUE);
}


bool Space3D::checkAxisRotation(const int& x, const int& y) {
    int xDrag, yDrag;
    if (m_buttonOX.hitCollision(x, y)) {
        getDrag(xDrag, yDrag);
        if (m_donutOX.hitCollision(xDrag, yDrag)) {
            double angle = findRotation(xDrag, yDrag, m_donutOX, m_buttonOX);
            m_cam.modifyAngles(angle, 0, 0);
            m_updated.fill(true);
            return true;
        }
    }
    if (m_buttonOY.hitCollision(x, y)) {
        getDrag(xDrag, yDrag);
        if (m_donutOY.hitCollision(xDrag, yDrag)) {
            double angle = findRotation(xDrag, yDrag, m_donutOY, m_buttonOY);
            m_cam.modifyAngles(0, angle, 0);
            m_updated.fill(true);
            return true;
        }
    }
    if (m_buttonOZ.hitCollision(x, y)) {
        getDrag(xDrag, yDrag);
        if (m_donutOZ.hitCollision(xDrag, yDrag)) {
            double angle = findRotation(xDrag, yDrag, m_donutOZ, m_buttonOZ);
            m_cam.modifyAngles(0, 0, angle);
            m_updated.fill(true);
            return true;
        }
    }
    return false;
}

DOUBLE Space3D::findRotation(const int& xDrag, const int& yDrag, const DonutButton& angleDonut, CircularButton& button) {
    int xCircle = angleDonut.getX();
    int yCircle = angleDonut.getY();
    int xDiff = xDrag - angleDonut.getX();
    int yDiff = yDrag - angleDonut.getY();
    double magV = sqrt(xDiff * xDiff + yDiff * yDiff);
    int xOnCircle = xCircle + xDiff / magV * angleDonut.getRadius();
    int yOnCircle = yCircle + yDiff / magV * angleDonut.getRadius();
    return atan2(yOnCircle - yCircle, xOnCircle - xCircle) - atan2(button.getY() - yCircle, button.getX() - xCircle);
}

Point3D Space3D::normalisePoint(const Point3D& pct) const {
    double xr = pct.getX() - m_cam.position().getX();
    double yr = pct.getY() - m_cam.position().getY();
    double zr = pct.getZ() - m_cam.position().getZ();
    double aX = m_cam.angleX();
    double aY = m_cam.angleY();
    double aZ = m_cam.angleZ();
    int dx = cos(aY) * (sin(aZ) * yr + cos(aZ) * xr) - sin(aY) * zr;
    int dy = sin(aX) * (cos(aY) * zr + sin(aY) * (sin(aZ) * yr + cos(aZ) * xr)) + cos(aX) * (cos(aZ) * yr - sin(aZ) * xr);
    int dz = cos(aX) * (cos(aY) * zr + sin(aY) * (sin(aZ) * yr + cos(aZ) * xr)) - sin(aX) * (cos(aZ) * yr - sin(aZ) * xr);
    return Point3D(dx, dy, dz);
}

Point2D Space3D::projectPoint(const Point3D& pct) const {
    const int xCenter = (x0 + x1) / 2;
    const int yCenter = (y0 + y1) / 2;
    const int yLen = (y1 - y0);
    double xr = pct.getX() - m_cam.position().getX();
    double yr = pct.getY() - m_cam.position().getY();
    double zr = pct.getZ() - m_cam.position().getZ();
    double aX = m_cam.angleX();
    double aY = m_cam.angleY();
    double aZ = m_cam.angleZ();
    double EZ = m_cam.EZ();
    int dx = cos(aY) * (sin(aZ) * yr + cos(aZ) * xr ) - sin(aY) * zr;
    int dy = sin(aX) * (cos(aY) * zr + sin(aY) * (sin(aZ) * yr + cos(aZ) * xr)) + cos(aX) * (cos(aZ) * yr - sin(aZ) * xr);
    int dz = cos(aX) * (cos(aY) * zr + sin(aY) * (sin(aZ) * yr + cos(aZ) * xr)) - sin(aX) * (cos(aZ) * yr - sin(aZ) * xr);
    if (dy <= 0) {
        return Point2D(-100, -100);
    }
    int xprim = EZ * dx * yLen / dy + xCenter;
    int yprim = EZ * dz * yLen / dy * -1 + yCenter;
    return Point2D(round(xprim), round(yprim));
}

Section Space3D::projectSection(const Mesh& mesh) {
    MyVector<Point2D> projectedPoints;
    projectedPoints.resize(mesh.size());
    for(size_t i = 0; i < mesh.size(); i++) {
        projectedPoints[i] = projectPoint(mesh[i]);
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
        return Point2D((yBorder - yP) / m + xP, yBorder);
    }
    if (xQ < xBorder) {
        return Point2D(xBorder, m * (xBorder - xP) + yP);
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
}

bool Space3D::insideWorkArea(const int& x, const int& y) const {
    return x0 <= x && x <= x1 && y0 <= y && y <= y1;
}

bool Space3D::insideWorkArea(const Point2D& point) const {
    return insideWorkArea(point.getX(), point.getY());
}


//returns true if menu should redraw itself
bool Space3D::getCommand(const int& x, const int& y) {
    int dump;
    getmouseclick(WM_LBUTTONUP, dump, dump);
    if (m_spinballButton.hitCollision(x, y)) {
        m_spinballSelected = !m_spinballSelected;
        run();
        return 0;
    }
    if (m_spinballSelected) {
        if (checkAxisRotation(x, y)) {
            run();
            return 0;
        }
    }
    for (size_t i = 0; i < size(); ++i) {
        if (m_sections[i].grabButtonCollision(x, y)) {
            selectMesh(i);
            int xDrag, yDrag;
            if (isDragAndDrop(xDrag, yDrag) && !m_sections[i].grabButtonCollision(xDrag, yDrag)) {
                dragAndDrop(xDrag, yDrag);
                return 1;
            }
        }
    }
    return 0;
}

void Space3D::getDrag(int& xDrag, int& yDrag) const {
    while (!ismouseclick(WM_LBUTTONUP));
    getmouseclick(WM_LBUTTONUP, xDrag, yDrag);
}

void Space3D::dragAndDrop(const int& xDrag, const int& yDrag) {
    const double xCenter = (x0 + x1) / 2;
    const double yCenter = (y0 + y1) / 2;
    const int yLen = y1 - y0;

    Point3D centerPoint(m_meshes[m_selected].centerPoint());
    Point3D normalizedPoint(normalisePoint(centerPoint));

    //SCHEMA:
    //"Dezproiectam" obiectul de pe ecran, si l scoatem din sistemul de coordonate al camerei
    //(adica we undo the rotation si il departam de camera)
    //dy2 = dy1; sunt la fel de aproape de camera.
    //Scoatem dx2 si dz2 prin bx1 si bx2

    //xC, yC - centrul sect.; for accurate translations
    double xC = m_sections[m_selected].centerPoint().getX();
    double yC = m_sections[m_selected].centerPoint().getY();

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
    int tx = (cos(aY) * cos(aZ)) * dx2 + (sin(aX) * sin(aY) * cos(aZ) - cos(aX) * sin(aZ)) * dy2 + (cos(aX) * sin(aY)* cos(aZ) + sin(aX) * sin(aZ)) * dz2;
    int ty = cos(aY) * sin(aZ) * dx2 + (sin(aX) * sin(aY) * sin(aZ) + cos(aX) * cos(aZ)) * dy2 + (cos(aX) * sin(aY) * sin(aZ) - sin(aX) * cos(aZ)) * dz2;
    int tz = -1 * sin(aY) * dx2 + sin(aX) * cos(aY) * dy2 + cos(aX) * cos(aY) * dz2;

    //..si acum il scoatem complet din sistemul de coordonate al camerei.
    tx = tx + m_cam.position().getX();
    ty = ty + m_cam.position().getY();
    tz = tz + m_cam.position().getZ();

    //translatam
    m_meshes[m_selected].translate(tx - centerPoint.getX(), ty - centerPoint.getY(), tz - centerPoint.getZ());
    m_updated[m_selected] = true;
    run();
}

bool Space3D::isDragAndDrop(int& xDrag, int& yDrag) const {
    getDrag(xDrag, yDrag);
    return x0 + 8 <= xDrag && xDrag <= x1 - 8 && y0 + 8 <= yDrag && yDrag <= y1 - 8;
}

void Space3D::highlightMesh() {
    m_sections[m_selected].drawButton(LIGHTGREEN, LIGHTGREEN);
    for (int j = 0; j < m_selected; ++j) {
        m_sections[j].drawButton(RED, RED);
    }
    for (size_t j = m_selected + 1; j < size(); ++j) {
        m_sections[j].drawButton(RED, RED);
    }
}

void Space3D::selectMesh(const size_t& index) {
    m_selected = index;
    highlightMesh();
}

//deseneaza toate mesh-urile proiectate + centrele
void Space3D::draw() {
    setfillstyle(SOLID_FILL, themeColors[m_theme][PRIMARYCOLOR]);
    bar(x0, y0, x1, y1);
    render();
    setlinestyle(SOLID_LINE, 0, 1);
    for (size_t i = 0; i < size(); ++i) {
        m_sections[i].draw(themeColors[m_theme][SECONDARYCOLOR], RED, RED);
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
