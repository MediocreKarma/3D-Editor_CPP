#include "Space3D.h"
#include "SettingsMenuInterface.h"
#include<iostream>
#include<math.h>
Space3D::Space3D(const double& maxRadius, const int& theme) :
    m_maxRadius(maxRadius), m_theme(theme), m_selected(-1), m_meshes(), m_sections(), m_updated(), m_cam(maxRadius) {}

size_t Space3D::size() const {
    return m_meshes.size();
}

void Space3D::addMesh(const Mesh& mesh) {
    m_meshes.push_back(mesh);
    m_sections.resize(size());
    m_updated.push_back(true);
}

//porneste desenarea pe o parte a ecranului
void Space3D::run(const int& x0, const int& y0, const int& x1, const int& y1) {
    const int xCenter = (x0 + x1) / 2;
    const int yCenter = (y0 + y1) / 2;
    const int xLen = x1 - x0;
    const int yLen = y1 - y0;
    draw(xCenter, yCenter, xLen, yLen);
}

Point3D Space3D::getProjectPoint(const Point3D& pct, const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const double& scale) const {
    double mx,my,mz;
    mx = pct.getX();
    my = pct.getY();
    mz = pct.getZ();
    float xr, yr, zr;
    Point3D P = m_cam.getPoint();
    xr = mx - P.getX();
    yr = my - P.getY();
    zr = mz - P.getZ();
    double dx, dy, dz;
    double aX = m_cam.getAngleX(), aY = m_cam.getAngleY(), aZ = m_cam.getAngleZ(), EZ = m_cam.getEZ();
    dx = cos(aY) * (sin(aZ) * yr + cos(aZ) * xr ) - sin(aY) * zr;
    dy = sin(aX) * (cos(aY) * zr + sin(aY) * (sin(aZ) * yr + cos(aZ) * xr)) + cos(aX) * (cos(aZ) * yr - sin(aZ) * xr);
    dz = cos(aX) * (cos(aY) * zr + sin(aY) * (sin(aZ) * yr + cos(aZ) * xr)) - sin(aX) * (cos(aZ) * yr - sin(aZ) * xr);
    return Point3D(dx,dy,dz);
}


Point2D Space3D::projectPoint(const Point3D& pct, const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const double& scale) const {
    double mx,my,mz;
    mx = pct.getX();
    my = pct.getY();
    mz = pct.getZ();
    float xr, yr, zr;
    Point3D P = m_cam.getPoint();
    xr = mx - P.getX();
    yr = my - P.getY();
    zr = mz - P.getZ();
    double dx, dy, dz; //rotire euler pct 3D.

    //axa X roteste in genul "te uiti mai in jos/sus"
    //axa Y roteste in genul "iti rotesti capul intr o parte, dar tot in fata te uiti"
    //axa Z roteste in genul "te uiti in stanga/dreapta"
    double aX = m_cam.getAngleX(), aY = m_cam.getAngleY(), aZ = m_cam.getAngleZ(), EZ = m_cam.getEZ();
    dx = cos(aY) * (sin(aZ) * yr + cos(aZ) * xr ) - sin(aY) * zr;
    dy = sin(aX) * (cos(aY) * zr + sin(aY) * (sin(aZ) * yr + cos(aZ) * xr)) + cos(aX) * (cos(aZ) * yr - sin(aZ) * xr);
    dz = cos(aX) * (cos(aY) * zr + sin(aY) * (sin(aZ) * yr + cos(aZ) * xr)) - sin(aX) * (cos(aZ) * yr - sin(aZ) * xr);

    double xprim = EZ/dy * dx * yLen * (-1) + xCenter;
    double yprim = EZ/dy * dz * yLen + yCenter;
    return Point2D(round(xprim),round(yprim));
}

Section Space3D::projectSection(Mesh& mesh, const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const double& scale) {
    MyVector<Line2D> lines;
    lines.reserve(mesh.size());
    for(size_t i = 0; i < mesh.size(); i++) {
        const Point2D P = projectPoint(mesh.getEdges()[i].getP(), xCenter, yCenter, xLen, yLen, scale);
        const Point2D Q = projectPoint(mesh.getEdges()[i].getQ(), xCenter, yCenter, xLen, yLen, scale);
        lines.push_back(Line2D(P, Q));
    }
    const Point3D pct = mesh.centerPoint();
    const Point2D pct2 = projectPoint(pct, xCenter, yCenter, xLen, yLen, scale);
    return Section(lines,pct2);
}

//proiecteaza fiecare mesh daca a fost updated
void Space3D::render(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const double& scale) {
    for (size_t i = 0; i < size(); ++i) {
        if (m_updated[i]) {
            m_sections[i] = projectSection(m_meshes[i], xCenter, yCenter, xLen, yLen, scale);
            m_updated[i] = false;
        }
    }
}

bool Space3D::insideWorkArea(const int& x, const int& y, const int& x0, const int& y0, const int& x1, const int& y1) const {
    return x0 <= x && x <= x1 && y0 <= y && y <= y1;
}

//verifica daca s a dat click pe butonul din centrul sectiunii. Daca da, foloseste translate
//pe 2 axe dar proportional cu coord Y, care ramane constanta
void Space3D::getCommand(const int& x0, const int& y0, const int& x1, const int& y1) {
    if (!ismouseclick(WM_LBUTTONDOWN)) {
        return;
    }
    int x, y;
    getmouseclick(WM_LBUTTONUP, x, y);
    getmouseclick(WM_LBUTTONDOWN, x, y);
    for (size_t i = 0; i < size(); ++i) {
        if (m_sections[i].grabButtonCollision(x, y)) {
            selectMesh(i);
            int xDrag, yDrag;
            if (isDragAndDrop(xDrag, yDrag)) {
                dragAndDrop(xDrag, yDrag, x0, y0, x1, y1);
            }
        }
    }
}

void Space3D::dragAndDrop(const int& xDrag, const int& yDrag, const int& x0, const int& y0, const int& x1, const int& y1) {
    const double xCenter = (x0 + x1) / 2;
    const double yCenter = (y0 + y1) / 2;
    const int xLen = x1 - x0;
    const int yLen = y1 - y0;

    Point3D auxPoint(m_meshes[m_selected].centerPoint());
    Point3D projPoint(getProjectPoint(auxPoint,xCenter, yCenter, xLen, yLen, 1));

    //SCHEMA:
    //"Dezproiectam" obiectul de pe ecran, si l scoatem din sistemul de coordonate al camerei
    //(adica we undo the rotation si il departam de camera)
    //dy2 = dy1; sunt la fel de aproape de camera.
    //Scoatem dx2 si dz2 prin bx1 si bx2

    //xC, yC - centrul sect.; for accurate translations
    double xC = m_sections[m_selected].centerPoint().getX();
    double yC = m_sections[m_selected].centerPoint().getY();

    /*Inversam ecuatiile astea din projectPoint si getProjectPoint:

    double xprim = EZ/dy * dx * yLen * (-1) + xCenter;
    double yprim = EZ/dy * dz * yLen + yCenter;

    pentru a afla b-urile, adica coordonatele proiectiei punctului
    intr-un sistem de coordonate de la (-1,-1) la (1,1),
    in loc de (x1,y1) la (x2, y2):
    */

    double bx1 = (xC - xCenter) / (-1) / yLen; //EZ/dy * dx1.
    double by1 = (yC - yCenter) / yLen;        //EZ/dy * dz1.
    double bx2 = (xDrag - xCenter) * (-1) / yLen; //EZ/dy * dx2.
    double by2 = (yDrag - yCenter) / yLen;        //EZ/dy * dz2.
    double dx1 = projPoint.getX();
    double dy1 = projPoint.getY();
    double dz1 = projPoint.getZ();
    double EZ = m_cam.getEZ();
    double dx2, dy2, dz2;
    //DY DIFERIT de 0 ca asa l vrem. In proiectie vom stabili defapt ce se intampla
    //daca camera e prea aproape de un obiect
    dy2 = dy1;
    dx2 = dx1 + (bx2 - bx1) * dy2 / EZ;
    dz2 = dz1 + (by2 - by1) * dy2 / EZ;

    //Aici inversam rotatia camerei...
    double tx,ty,tz;

    double aX = m_cam.getAngleX(), aY = m_cam.getAngleY(), aZ = m_cam.getAngleZ();
    tx = (cos(aY) * cos(aZ)) * dx2 + (sin(aX) * sin(aY) * cos(aZ) - cos(aX) * sin(aZ)) * dy2 + (cos(aX) * sin(aY)* cos(aZ) + sin(aX) * sin(aZ)) * dz2;
    ty = cos(aY) * sin(aZ) * dx2 + (sin(aX) * sin(aY) * sin(aZ) + cos(aX) * cos(aZ)) * dy2 + (cos(aX) * sin(aY) * sin(aZ) - sin(aX) * cos(aZ)) * dz2;
    tz = (-1) * sin(aY) * dx2 + sin(aX) * cos(aY) * dy2 + cos(aX) * cos(aY) * dz2;

    //..si acum il scoatem complet din sistemul de coordonate al camerei.
    Point3D P = m_cam.getPoint();
    tx = tx + P.getX();
    ty = ty + P.getY();
    tz = tz + P.getZ();

    //translatam
    m_meshes[m_selected].translate(tx - auxPoint.getX(), ty - auxPoint.getY(), tz - auxPoint.getZ());
    m_updated[m_selected] = true;
    run(x0, y0, x1, y1);
}

bool Space3D::isDragAndDrop(int& xDrag, int& yDrag) const {
    while (!ismouseclick(WM_LBUTTONUP));
    getmouseclick(WM_LBUTTONUP, xDrag, yDrag);
    return !m_sections[m_selected].grabButtonCollision(xDrag, yDrag);
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
void Space3D::draw(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen) {
    setfillstyle(SOLID_FILL, SettingsMenuInterface::themeColors[m_theme][SettingsMenuInterface::PRIMARYCOLOR]);bar(xCenter - xLen / 2, yCenter - yLen / 2, xCenter + xLen / 2, yCenter + yLen / 2);
    render(xCenter, yCenter, xLen, yLen);
    for (size_t i = 0; i < size(); ++i) {
        m_sections[i].draw(SettingsMenuInterface::themeColors[m_theme][SettingsMenuInterface::SECONDARYCOLOR], RED, RED);
    }
}
// Deprecated
/*
void Space3D::rotateOX(Mesh& lines, const double& alpha) {
    for(size_t i = 0; i < lines.size(); i++){
        Point3D aux1 = lines[i].getP();
        Point3D aux2 = lines[i].getQ();
        aux1.rotateOX(alpha);
        aux2.rotateOX(alpha);
        lines[i].setP(aux1);
        lines[i].setQ(aux2);
    }
}

void Space3D::rotateOY(Mesh& lines, const double& alpha) {
    for(size_t i = 0; i < lines.size(); i++){
        Point3D aux1 = lines[i].getP();
        Point3D aux2 = lines[i].getQ();
        aux1.rotateOY(alpha);
        aux2.rotateOY(alpha);
        lines[i].setP(aux1);
        lines[i].setQ(aux2);
    }
}

void Space3D::rotateOZ(Mesh& lines, const double& alpha) {
    for(size_t i = 0; i < lines.size(); i++){
        Point3D aux1 = lines[i].getP();
        Point3D aux2 = lines[i].getQ();
        aux1.rotateOZ(alpha);
        aux2.rotateOZ(alpha);
        lines[i].setP(aux1);
        lines[i].setQ(aux2);
    }
}
*/
