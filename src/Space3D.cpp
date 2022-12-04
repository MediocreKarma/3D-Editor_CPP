#include "Space3D.h"
#include "AppTools.h"
#include "MyArray.h"
#include<iostream>
Space3D::Space3D(const double& maxRadius, const int& theme) :
    m_theme(theme), m_selected(-1), m_spinballSelected(false), m_meshes(), m_sections(), m_updated(), m_cam(maxRadius),
    m_buttonOX(), m_buttonOY(), m_buttonOZ(), m_donutOX(), m_donutOY(), m_donutOZ(), m_spinballButton(), m_axisButtons() {}

size_t Space3D::size() const {
    return m_meshes.size();
}

void Space3D::addMesh(const Mesh& mesh) {
    m_meshes.push_back(mesh);
    m_sections.resize(size());
    m_updated.push_back(true);
}

#include <iostream>

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
void Space3D::run(const int& x0, const int& y0, const int& x1, const int& y1) {
    const int xCenter = (x0 + x1) / 2;
    const int yCenter = (y0 + y1) / 2;
    const int xLen = x1 - x0;
    const int yLen = y1 - y0;
    draw(xCenter, yCenter, xLen, yLen);
    drawSpinball(x1 - 75, y0 + 75);
}

void Space3D::drawSpinball(const int& x, const int& y) {
    m_spinballButton = Button(x, y, 140, 140);
    m_spinballButton.drawLabel(SettingsMenuInterface::themeColors[m_theme][SettingsMenuInterface::PRIMARYCOLOR], SettingsMenuInterface::themeColors[m_theme][SettingsMenuInterface::SECONDARYCOLOR]);
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
    m_donutOX.drawLabel(SettingsMenuInterface::NO_COLOR, LIGHTGREEN);
    m_buttonOX = CircularButton(x + 20 + 40 * cos(m_cam.angleX()), y + 120 + 40 * sin(m_cam.angleX()), 5);
    m_buttonOX.drawLabel(LIGHTGREEN, LIGHTGREEN);
    m_donutOY = DonutButton(x + 20, y + 220, 40, 20);
    m_donutOY.drawLabel(SettingsMenuInterface::NO_COLOR, LIGHTRED);
    m_buttonOY = CircularButton(x + 20 + 40 * cos(m_cam.angleY()), y + 220 + 40 * sin(m_cam.angleY()), 5);
    m_buttonOY.drawLabel(LIGHTRED, LIGHTRED);
    m_donutOZ = DonutButton(x + 20, y + 320, 40, 20);
    m_donutOZ.drawLabel(SettingsMenuInterface::NO_COLOR, LIGHTBLUE);
    m_buttonOZ = CircularButton(x + 20 + 40 * cos(m_cam.angleZ()), y + 320 + 40 * sin(m_cam.angleZ()), 5);
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

Point2D Space3D::projectPoint(const Point3D& pct, const int& xCenter, const int& yCenter, const int& /*xLen*/, const int& yLen) const {
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
        dy = 0.0000000000000001;
    }
    int xprim = EZ * dx * yLen / dy + xCenter;
    int yprim = EZ * dz * yLen / dy * -1 + yCenter;
    return Point2D(round(xprim), round(yprim));
}

Section Space3D::projectSection(const Mesh& mesh, const int& xCenter, const int& yCenter, const int& xLen, const int& yLen) {
    const int x0 = xCenter - xLen / 2;
    const int x1 = xCenter + xLen / 2;
    const int y0 = yCenter - yLen / 2;
    const int y1 = yCenter + yLen / 2;
    MyVector<Line2D> lines;
    lines.reserve(mesh.size());
    for(size_t i = 0; i < mesh.size(); i++) {
        Point2D P = projectPoint(mesh[i].getP(), xCenter, yCenter, xLen, yLen);
        Point2D Q = projectPoint(mesh[i].getQ(), xCenter, yCenter, xLen, yLen);
        if (insideWorkArea(P, x0, y0, x1, y1) || insideWorkArea(Q, x0, y0, x1, y1)) {
            lines.push_back(Line2D(P, Q));
        }
    }
    const Point3D meshCenterPoint = mesh.centerPoint();
    Point2D sectionCenterPoint = projectPoint(meshCenterPoint, xCenter, yCenter, xLen, yLen);
    if (!insideWorkArea(sectionCenterPoint, x0, y0, x1, y1)) {
        sectionCenterPoint = Point2D(-100, -100);
    }
    return Section(lines, sectionCenterPoint);
}

//proiecteaza fiecare mesh daca a fost updated
void Space3D::render(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen) {
    for (size_t i = 0; i < size(); ++i) {
        if (m_updated[i]) {
            m_sections[i] = projectSection(m_meshes[i], xCenter, yCenter, xLen, yLen);
            m_updated[i] = false;
        }
    }
}

bool Space3D::insideWorkArea(const int& x, const int& y, const int& x0, const int& y0, const int& x1, const int& y1) const {
    return x0 <= x && x <= x1 && y0 <= y && y <= y1;
}

bool Space3D::insideWorkArea(const Point2D& point, const int& x0, const int& y0, const int& x1, const int& y1) const {
    return insideWorkArea(point.getX(), point.getY(), x0, y0, x1, y1);
}

void Space3D::getCommand(const int& x0, const int& y0, const int& x1, const int& y1) {
    if (!ismouseclick(WM_LBUTTONDOWN)) {
        return;
    }
    int x, y;
    getmouseclick(WM_LBUTTONUP, x, y);
    getmouseclick(WM_LBUTTONDOWN, x, y);
    if (m_spinballButton.hitCollision(x, y)) {
        m_spinballSelected = !m_spinballSelected;
        run(x0, y0, x1, y1);
        return;
    }
    if (m_spinballSelected) {
        if (checkAxisRotation(x, y)) {
            run(x0, y0, x1, y1);
            return;
        }
        m_spinballSelected = false;
    }
    for (size_t i = 0; i < size(); ++i) {
        if (m_sections[i].grabButtonCollision(x, y)) {
            selectMesh(i, x0, y0, x1, y1);
            int xDrag, yDrag;
            if (isDragAndDrop(xDrag, yDrag, x0, y0, x1, y1) && !m_sections[i].grabButtonCollision(xDrag, yDrag)) {
                dragAndDrop(xDrag, yDrag, x0, y0, x1, y1);
                return;
            }
        }
    }
    if(getDraggedAxis(x, y, x0, y0, x1, y1) >= 0) {
        int xDrag, yDrag;
        int axis = getDraggedAxis(x, y, x0, y0, x1, y1);
        if(isDragAndDrop(xDrag,yDrag, x0, y0, x1, y1)  && !m_sections[m_selected].grabButtonCollision(xDrag, yDrag)){
            moveMesh(axis, x, y, xDrag, yDrag, x0, y0, x1, y1);
            return;
        }
    }
}

int Space3D::getDraggedAxis(int& xDrag, int& yDrag, const int& x0, const int& y0, const int& x1, const int& y1) const {
    for(size_t i = 0; i < 3; ++i) {
        if(m_axisButtons[i].hitCollision(xDrag, yDrag)){
            return i;
        }
    }
    return -1;
}

void Space3D::getDrag(int& xDrag, int& yDrag) const {
    while (!ismouseclick(WM_LBUTTONUP));
    getmouseclick(WM_LBUTTONUP, xDrag, yDrag);
}

void Space3D::moveMesh(const int axis, const int& xDrag, const int& yDrag, const int& xDrop, const int& yDrop, const int& x0, const int& y0, const int& x1, const int& y1) {

    //axis = 0 pt x, 1 pt y, 2 pt z. Sa nu fac n-spe mii de functii. Fac una singura.
    //la axisCollision vreau sa fac gen. Sa mi dea indexul sagetelei apasate
    //Si l bag direct aci in functie

    const double xCenter = (x0 + x1) / 2;
    const double yCenter = (y0 + y1) / 2;
    const int yLen = y1 - y0;

    Point3D centerPoint(m_meshes[m_selected].centerPoint());
    Point3D normalizedPoint(normalisePoint(centerPoint));

    double xC = m_sections[m_selected].centerPoint().getX();
    double yC = m_sections[m_selected].centerPoint().getY();

    double xr = centerPoint.getX() - m_cam.position().getX();
    double yr = centerPoint.getY() - m_cam.position().getY();
    double zr = centerPoint.getZ() - m_cam.position().getZ();

    double bx1 = (xC - xCenter) / yLen;
    double by1 = (yC - yCenter) / yLen * -1;
    double bx2 = (xDrop - (xDrag - xC) - xCenter) / yLen;
    double by2 = (yDrop - (yDrag - yC) - yCenter) / yLen * -1;
    double dx1 = normalizedPoint.getX();
    double dy1 = normalizedPoint.getY();
    double dz1 = normalizedPoint.getZ();
    double EZ = m_cam.EZ();
    double dy2 = dy1;
    double dx2 = dx1 + (bx2 - bx1) * dy2 / EZ;
    double dz2 = dz1 + (by2 - by1) * dy2 / EZ;
    if(axis == 0) {
        //ce trebe sa fie la fel? dxu sa fie identic cu diferenta de pixeli.
        //Si sa fie pe aceleasi coordonate.
        double dx2 = dx1;

    }

    double aX = m_cam.angleX();
    double aY = m_cam.angleY();
    double aZ = m_cam.angleZ();
    int tx = (cos(aY) * cos(aZ)) * dx2 + (sin(aX) * sin(aY) * cos(aZ) - cos(aX) * sin(aZ)) * dy2 + (cos(aX) * sin(aY)* cos(aZ) + sin(aX) * sin(aZ)) * dz2;
    int ty = cos(aY) * sin(aZ) * dx2 + (sin(aX) * sin(aY) * sin(aZ) + cos(aX) * cos(aZ)) * dy2 + (cos(aX) * sin(aY) * sin(aZ) - sin(aX) * cos(aZ)) * dz2;
    int tz = -1 * sin(aY) * dx2 + sin(aX) * cos(aY) * dy2 + cos(aX) * cos(aY) * dz2;

    tx = tx + m_cam.position().getX();
    ty = ty + m_cam.position().getY();
    tz = tz + m_cam.position().getZ();

    if(axis == 0){
        m_meshes[m_selected].translate(tx - centerPoint.getX(), 0, 0);
    }
    if(axis == 1){
        m_meshes[m_selected].translate(0, ty - centerPoint.getY(), 0);
    }
    if(axis == 2){
        m_meshes[m_selected].translate(0, 0, tz - centerPoint.getZ());
    }
    m_updated[m_selected] = true;
    run(x0, y0, x1, y1);
}

void Space3D::dragAndDrop(const int& xDrag, const int& yDrag, const int& x0, const int& y0, const int& x1, const int& y1) {
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
    run(x0, y0, x1, y1);
}

bool Space3D::isDragAndDrop(int& xDrag, int& yDrag, const int& x0, const int& y0, const int& x1, const int& y1) const {
    getDrag(xDrag, yDrag);
    return insideWorkArea(xDrag, yDrag, x0, y0, x1, y1);
}

void Space3D::highlightMesh(const int& x0, const int& y0, const int& x1, const int& y1) {
    setAxisButtons(x0, y0, x1, y1);
    drawAxes();
    m_sections[m_selected].drawButton(LIGHTGREEN, LIGHTGREEN);
    for (int j = 0; j < m_selected; ++j) {
        m_sections[j].drawButton(RED, RED);
    }
    for (size_t j = m_selected + 1; j < size(); ++j) {
        m_sections[j].drawButton(RED, RED);
    }
}

void Space3D::setAxisButtons(const int& x0, const int& y0, const int& x1, const int& y1) {
    //Deocamdata facem DOAR cu o translatie constanta
    //Mai incolo facem sa se modifice gen dupa whatever the fuck idfk
    const double xCenter = (x0 + x1) / 2;
    const double yCenter = (y0 + y1) / 2;
    const int xLen = x1 - x0;
    const int yLen = y1 - y0;
    const Point3D point = m_meshes[m_selected].centerPoint();

    const int length = 200, thickness = 1, radius = 10; // le las aci deocamdata dar trebe mutate NEAPARAT
                                                       // ori ca parametri ori ca si constante in header

    Point3D x3D = Point3D(point.getX() + length, point.getY(), point.getZ());
    Point3D y3D = Point3D(point.getX(), point.getY() - length, point.getZ());
    Point3D z3D = Point3D(point.getX(), point.getY(), point.getZ() + length);

    Point2D center2D = m_sections[m_selected].centerPoint();
    Point2D x2D = projectPoint(x3D,xCenter,yCenter, xLen, yLen);
    Point2D y2D = projectPoint(y3D,xCenter,yCenter, xLen, yLen);
    Point2D z2D = projectPoint(z3D,xCenter,yCenter, xLen, yLen);

    m_axisButtons[0] = AxisButton(center2D.getX(), center2D.getY(), x2D.getX(), x2D.getY(), thickness, radius);
    m_axisButtons[1] = AxisButton(center2D.getX(), center2D.getY(), y2D.getX(), y2D.getY(), thickness, radius);
    m_axisButtons[2] = AxisButton(center2D.getX(), center2D.getY(), z2D.getX(), z2D.getY(), thickness, radius);
}

void Space3D::drawAxes() {
    m_axisButtons[0].drawButton(RED);
    m_axisButtons[1].drawButton(GREEN);
    m_axisButtons[2].drawButton(BLUE);
}

void Space3D::selectMesh(const size_t& index, const int& x0, const int& y0, const int& x1, const int& y1) {
    m_selected = index;
    run(x0, y0, x1, y1);
    highlightMesh(x0, y0, x1, y1);
}

//deseneaza toate mesh-urile proiectate + centrele
void Space3D::draw(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen) {
    setfillstyle(SOLID_FILL, SettingsMenuInterface::themeColors[m_theme][SettingsMenuInterface::PRIMARYCOLOR]);bar(xCenter - xLen / 2, yCenter - yLen / 2, xCenter + xLen / 2, yCenter + yLen / 2);
    render(xCenter, yCenter, xLen, yLen);
    setlinestyle(SOLID_LINE, 0, 1);
    for (size_t i = 0; i < size(); ++i) {
        m_sections[i].draw(SettingsMenuInterface::themeColors[m_theme][SettingsMenuInterface::SECONDARYCOLOR], RED, RED);
    }
}

