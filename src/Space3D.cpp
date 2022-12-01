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

Point3D Space3D::getProjectPoint(const Point3D& pct, const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const double& scale) const {
    double mx,my,mz;
    mx = pct.getX();
    my = pct.getY();
    mz = pct.getZ();
    double xr, yr, zr;
    Point3D P = m_cam.getPoint();
    xr = mx - P.getX();
    yr = my - P.getY();
    zr = mz - P.getZ();
    double dx, dy, dz;
    double aX = m_cam.getAngleX(), aY = m_cam.getAngleY(), aZ = m_cam.getAngleZ(), EZ = m_cam.getEZ();
    dx = cos(aX) * (sin(aZ * yr) + cos(aZ) * xr ) - sin(aY) * zr;
    dy = sin(aX) * (cos(aY) * zr + sin(aY) * (sin(aZ) * yr + cos(aZ) * xr)) + cos(aX) * (cos(aZ) * yr - sin(aZ) * xr);
    dz = cos(aX) * (cos(aY) * zr + sin(aY) * (sin(aZ) * yr + cos(aZ) * xr)) - sin(aX) * (cos(aZ) * yr - sin(aZ) * xr);
    return Point3D(dx,dy,dz);
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
    float xr, yr, zr; //temporare; pct.x - cam.x
    Point3D P = m_cam.getPoint();
    xr = mx - P.getX();
    yr = my - P.getY();
    zr = mz - P.getZ();
    double dx, dy, dz; //rotire euler pct 3D.

    //axa X roteste in genul "te uiti mai in jos/sus"
    //axa Y roteste in genul "iti rotesti capul intr o parte, dar tot in fata te uiti"
    //axa Z roteste in genul "te uiti in stanga/dreapta"
    double aX = m_cam.getAngleX(), aY = m_cam.getAngleY(), aZ = m_cam.getAngleZ(), EZ = m_cam.getEZ();
    dx = cos(aX) * (sin(aZ * yr) + cos(aZ) * xr ) - sin(aY) * zr;
    dy = sin(aX) * (cos(aY) * zr + sin(aY) * (sin(aZ) * yr + cos(aZ) * xr)) + cos(aX) * (cos(aZ) * yr - sin(aZ) * xr);
    dz = cos(aX) * (cos(aY) * zr + sin(aY) * (sin(aZ) * yr + cos(aZ) * xr)) - sin(aX) * (cos(aZ) * yr - sin(aZ) * xr);
    double xprim = EZ/dy * dx * yLen * (-1) + xCenter;
    double yprim = EZ/dy * dz * yLen + yCenter;
    //std::cout<<"punct:"<<dx<<" "<<dy<<" "<<dz<<"\n";
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
            const int yCenter = (y0 + y1) / 2;
            const int xLen = x1 - x0;
            const int yLen = y1 - y0;
            int xMovePosition = (xDrag - xCenter);
            int zMovePosition = (yLen / 2 - yDrag);

            //E STRICAT: merge doar cand vrea
            Point3D origine(0,0,0);
            Point3D auxPoint(m_meshes[i].centerPoint());
            Point3D projPoint(getProjectPoint(auxPoint,xCenter, yCenter, xLen, yLen, 1));
            Point3D finalPoint();

            float EZ = m_cam.getEZ();

            float dx1,dy1,dz1;
            dx1 = projPoint.getX();
            dy1 = projPoint.getY();
            dz1 = projPoint.getZ();
            std::cout<<"dxes: "<<dx1<<" "<<dy1<<" "<<dz1<<"\n";

            float aX = m_cam.getAngleX() ;
            float aY = m_cam.getAngleY() ;
            float aZ = m_cam.getAngleZ() ;

            float xr2, yr2, zr2;
            float xF = x, yF = y;
            float vx,vy;
            float lungime = Line3D(auxPoint,m_cam.getPoint()).getLength();
            vx = (xDrag - xF) / yLen * lungime;
            vy = (yF - yDrag) / yLen * lungime;

            Point3D v(vx,0,vy);
            Point3D projV = getProjectPoint(v,xCenter,yCenter,xLen,yLen,1);
            float xr,yr,zr;
            xr = dx1 + projV.getX();
            yr = dy1 + projV.getY();
            zr = dz1 + projV.getZ();

            Point3D camPoint(m_cam.getPoint());
            float x22,y22,z22;
            x22 = xr + 2 * camPoint.getX();
            y22 = yr + 2 * camPoint.getY();
            z22 = zr + 2 * camPoint.getZ();

            m_meshes[i].translate(x22 - auxPoint.getX(), y22 - auxPoint.getY(), z22 - auxPoint.getZ());
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
