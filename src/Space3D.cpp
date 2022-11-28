#include "Space3D.h"

Space3D::Space3D(const double& maxRadius, const int& theme) :
    m_maxRadius(maxRadius), m_theme(theme), m_meshes(), m_sections(), m_updated() {}

Space3D::Space3D(MyVector<Mesh>& meshes, const double& maxRadius) :
     m_maxRadius(maxRadius), m_meshes(meshes), m_sections(meshes.size()), m_updated(meshes.size(), true) {}

size_t Space3D::size() const {
    return m_meshes.size();
}

void Space3D::addMesh(const Mesh& mesh) {
    m_meshes.push_back(mesh);
    m_sections.resize(size());
    m_updated.push_back(true);
}

void Space3D::run(const int& x0, const int& y0, const int& x1, const int& y1) {
    const int xCenter = (x0 + x1) / 2;
    const int yCenter = (y0 + y1) / 2;
    const int xLen = x1 - x0;
    const int yLen = y1 - y0;
    draw(xCenter, yCenter, xLen, yLen);
}

void Space3D::render(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const double& scale) {
    for (size_t i = 0; i < size(); ++i) {
        if (m_updated[i]) {
            m_sections[i] = m_meshes[i].project(xCenter, yCenter, xLen, yLen, m_maxRadius, scale);
            m_updated[i] = false;
        }
    }
}

bool Space3D::insideWorkArea(const int& x, const int& y, const int& x0, const int& y0, const int& x1, const int& y1) const {
    return x0 <= x && x <= x1 && y0 <= y && y <= y1;
}

#include <iostream>

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
                return;
            }
            double aa = (m_maxRadius - m_meshes[i].centerPoint().getY()) / m_maxRadius;
            const int xCenter = (x0 + x1) / 2;
            const int yLen = y1 - y0;
            int xMovePosition = (xDrag - xCenter) / aa;
            int zMovePosition = (yLen / 2 - yDrag) / aa;
            m_meshes[i].translate(xMovePosition - m_meshes[i].centerPoint().getX(), 0, zMovePosition - m_meshes[i].centerPoint().getZ());
            m_updated[i] = true;
            run(x0, y0, x1, y1);
        }
    }
}

void Space3D::draw(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen) {
    setfillstyle(SOLID_FILL, 15 * (1 - m_theme));
    bar(xCenter - xLen / 2, yCenter - yLen / 2, xCenter + xLen / 2, yCenter + yLen / 2);
    //drawGrid(xCenter, yCenter, xLen, yLen)
    render(xCenter, yCenter, xLen, yLen);
    for (size_t i = 0; i < size(); ++i) {
        m_sections[i].draw(m_theme, RED, RED);
    }
}

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

