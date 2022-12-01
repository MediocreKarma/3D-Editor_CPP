#include "../include/Space3D.h"
#include "../include/SettingsMenuInterface.h"
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

Point2D Space3D::projectPoint(const Point3D& pct, const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const double& scale) const {
    //DEPRECATED
    /*
    double aa = (radius - y) / radius;
    return Point2D( x * scale * aa + xCenter, yLen / 2 - z * scale * aa);*/
    double mx,my,mz;
    mx = pct.getX();
    my = pct.getY();
    mz = pct.getZ();
    int xr, yr, zr; //temporare; pct.x - cam.x
    Point3D P = m_cam.getPoint();
    xr = mx - P.getX();
    yr = my - P.getY();
    zr = mz - P.getZ();
    double dx, dy, dz; //rotire euler pct 3D.

    //axa X roteste in genul "te uiti mai in jos/sus"
    //axa Y roteste in genul "iti rotesti capul intr o parte, dar tot in fata te uiti"
    //axa Z roteste in genul "te uiti in stanga/dreapta"
    double aX = m_cam.getAngleX();
    double aY = m_cam.getAngleY();
    double aZ = m_cam.getAngleZ();
    //tie ti tot zisesem ex dar e defapt ez. Scuze.
    double EZ = m_cam.getEZ();
    dx =  cos(aX) * cos(aZ) * xr + cos(aX) * sin(aZ) * yr - sin(aY) * zr;
    dy = (sin(aX) * sin(aY) * cos(aZ) - cos(aX) * sin(aZ)) * xr + (cos(aX) * cos(aZ) + sin(aX) * sin(aY) * sin(aZ)) * yr + sin(aX) * cos(aY) * zr;
    dz = (cos(aX) * sin(aY) * cos(aZ) - sin(aX) * sin(aZ)) * xr + (cos(aX) * sin(aY) * sin(aZ) - sin(aX) * cos(aZ)) * yr + cos(aX) * cos(aY) * zr;
    double xprim = EZ * dx * yLen / dy + xCenter;
    double yprim = EZ * dz * yLen / dy + yCenter;
    return Point2D(round(xprim), round(yprim));
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
    return Section(lines, pct2);
}

//proiecteaza fiecare mesh daca a fost updated
void Space3D::render(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const double& scale) {
    for (size_t i = 0; i < size(); ++i) {
        if (m_updated[i]) {
            m_sections[i] = projectSection(m_meshes[i], xCenter, yCenter, xLen, yLen, scale); /*m_meshes[i].project(xCenter, yCenter, xLen, yLen, m_cam, scale);*/
            m_updated[i] = false;
        }
    }
}

bool Space3D::insideWorkArea(const int& x, const int& y, const int& x0, const int& y0, const int& x1, const int& y1) const {
    return x0 <= x && x <= x1 && y0 <= y && y <= y1;
}

#include <iostream>

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
    const int xCenter = (x0 + x1) / 2;
    const int yLen = y1 - y0;
    const int yCenter = (y0 + y1) / 2;
    const int yr = m_meshes[m_selected].centerPoint().getY();
    std::cout << yr << '\n';
    const double aX = m_cam.getAngleX();
    const double aY = m_cam.getAngleY();
    const double aZ = m_cam.getAngleZ();
    const double EZ = m_cam.getEZ();
    const double Cx1 = cos(aX) * cos(aZ);
    const double Cx2 = cos(aX) * sin(aZ) * yr;
    const double Cx3 = -sin(aY);
    const double Cy1 = sin(aX) * sin(aY) * cos(aZ) - cos(aX) * sin(aZ);
    const double Cy2 = (cos(aX) * cos(aZ) + sin(aX) * sin(aY) * sin(aZ)) * yr;
    const double Cy3 = sin(aX) * cos(aY);
    const double Cz1 = cos(aX) * sin(aY) * cos(aZ) - sin(aX) * sin(aZ);
    const double Cz2 = (cos(aX) * sin(aY) * sin(aZ) - sin(aX) * cos(aZ)) * yr;
    const double Cz3 = cos(aX) * cos(aY);
    const double C1 = (xDrag - xCenter) / (EZ * yLen);
    const double C2 = (yDrag - yCenter) / (EZ * yLen);
    const double alpha1 = C1 * Cy1 - Cx1;
    const double beta1 = C1 * Cy3 - Cx3;
    const double delta1 = Cx2 - C1 * Cy2;
    const double alpha2 = C2 * Cy1 - Cz1;
    const double beta2 = C2 * Cy3 - Cz3;
    const double delta2 = Cz2 - C2 * Cy2;
    std::cout << alpha2 * beta1 << ' ' << alpha1 * beta2 << '\n';
    const int zr = (delta1 * alpha2 - delta2 * alpha1) / (alpha2 * beta1 - alpha1 * beta2);
    const int xr = (delta1 * beta2 - delta2 * beta1) / (alpha1 * beta2 - alpha2 * beta1);
    int realX = xr + m_cam.getPoint().getX();
    int realZ = zr + m_cam.getPoint().getZ();
    std::cout << realX << ' ' << realZ << '\n';
    m_meshes[m_selected].translate(realX - m_meshes[m_selected].centerPoint().getX(), 0, realZ - m_meshes[m_selected].centerPoint().getZ());
    m_updated[m_selected] = 1;
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
