#include "../include/Space3D.h"
#include "../include/SettingsMenuInterface.h"
#include<iostream>
#include<math.h>
Space3D::Space3D(const double& maxRadius, const int& theme) :
    m_maxRadius(maxRadius), m_theme(theme), m_meshes(), m_sections(), m_updated(), m_cam(maxRadius) {}

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
    float mx,my,mz;
    mx = pct.getX();
    my = pct.getY();
    mz = pct.getZ();
    int xr, yr, zr; //temporare; pct.x - cam.x
    Point3D P = m_cam.getPoint();
    xr = mx - P.getX();
    yr = my - P.getY();
    zr = mz - P.getZ();
    float dx, dy, dz; //rotire euler pct 3D.

    //axa X roteste in genul "te uiti mai in jos/sus"
    //axa Y roteste in genul "iti rotesti capul intr o parte, dar tot in fata te uiti"
    //axa Z roteste in genul "te uiti in stanga/dreapta"
    float aX = m_cam.getAngleX() ;
    float aY = m_cam.getAngleY() - 3.14/12;
    float aZ = m_cam.getAngleZ() ;
    //tie ti tot zisesem ex dar e defapt ez. Scuze.
    float EZ = m_cam.getEZ();
    dx = cos(aX) * (sin(aZ * yr) + cos(aZ) * xr ) - sin(aY) * zr;
    dy = sin(aX) * (cos(aY) * zr + sin(aY) * (sin(aZ) * yr + cos(aZ) * xr)) + cos(aX) * (cos(aZ) * yr - sin(aZ) * xr);
    dz = cos(aX) * (cos(aY) * zr + sin(aY) * (sin(aZ) * yr + cos(aZ) * xr)) - sin(aX) * (cos(aZ) * yr - sin(aZ) * xr);
    float xprim = EZ/dy * dx * yLen + xCenter;
    float yprim = EZ/dy * dz * yLen + yCenter;
    return Point2D(xprim,yprim);
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
            m_sections[i].drawButton(LIGHTGREEN, LIGHTGREEN);
            for (size_t j = 0; j < size(); ++j) {
                if (i == j) {
                    continue;
                }
                m_sections[j].drawButton(RED, RED);
            }
            while (!ismouseclick(WM_LBUTTONUP));
            int xDrag, yDrag;
            getmouseclick(WM_LBUTTONUP, xDrag, yDrag);
            if (m_sections[i].grabButtonCollision(xDrag, yDrag)) {
                    //daca s a dat mouseUP tot pe ala
                return;
            }

            double aa = (m_maxRadius - m_meshes[i].centerPoint().getY()) / m_maxRadius;
            const int xCenter = (x0 + x1) / 2;
            const int yLen = y1 - y0;
            int xMovePosition = (xDrag - xCenter) / aa;
            int zMovePosition = (yLen / 2 - yDrag) / aa;

            //trebe calculat defapt in functie de camera. Deci. Les go
            //ce avem? avem i la dispozitie. Deci, centerPoint of m_meshes[i] ig.
            /*Point3D punctAux(m_meshes[i].centerPoint());
            Point3D origine(0,0,0)
            deci. cumva vom roti vectorul in functie de cum e rotita camera. nu stiu cum. bag picioarele
            si dupaia translatam efectiv cu x si y
            si aflam care i treaba
            */

            m_meshes[i].translate(xMovePosition - m_meshes[i].centerPoint().getX(), 0, zMovePosition - m_meshes[i].centerPoint().getZ());
            m_updated[i] = true;
            run(x0, y0, x1, y1);
        }
    }
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
