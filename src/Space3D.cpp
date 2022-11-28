#include "Space3D.h"

Space3D::Space3D() :
    m_maxRadius(), m_meshes(), m_sections() {}

Space3D::Space3D(MyVector<Mesh>& meshes, const double& maxRadius) :
     m_maxRadius(maxRadius), m_meshes(meshes), m_sections() {}

void Space3D::rotateOX(Mesh& lines, const double& alpha){
    for(size_t i = 0; i < lines.size(); i++){
        Point3D aux1 = lines[i].getP();
        Point3D aux2 = lines[i].getQ();
        aux1.rotateOX(alpha);
        aux2.rotateOX(alpha);
        lines[i].setP(aux1);
        lines[i].setQ(aux2);
    }
}

void Space3D::rotateOY(Mesh& lines, const double& alpha){
    for(size_t i = 0; i < lines.size(); i++){
        Point3D aux1 = lines[i].getP();
        Point3D aux2 = lines[i].getQ();
        aux1.rotateOY(alpha);
        aux2.rotateOY(alpha);
        lines[i].setP(aux1);
        lines[i].setQ(aux2);
    }
}

void Space3D::rotateOZ(Mesh& lines, const double& alpha){
    for(size_t i = 0; i < lines.size(); i++){
        Point3D aux1 = lines[i].getP();
        Point3D aux2 = lines[i].getQ();
        aux1.rotateOZ(alpha);
        aux2.rotateOZ(alpha);
        lines[i].setP(aux1);
        lines[i].setQ(aux2);
    }
}

Point2D Space3D::projectPoint(const Point3D& pct, const int& xMij, const int& yMij, const int& xLung, const int& yLung) const {
    double xr = pct.getX();
    double zr = pct.getZ();
    double yr = pct.getY();
    double scale = (double(xLung) / getmaxx()) > double(yLung) / getmaxy() ? double(xLung) / getmaxx() : double(yLung) / getmaxy();
    double aa = (m_maxRadius - yr) / m_maxRadius;
    return Point2D((xr * aa * scale + double(xMij)), (yMij + yLung / 2 - (zr * aa  * scale + double(yMij))));
}

Section Space3D::projectMesh(const int& xMij, const int& yMij, const int& xLung, const int& yLung) {
    Section projection;
    for (size_t i = 0; i < m_meshes.size(); ++i) {
        for(size_t j = 0; j < m_meshes[i].size(); j++) {
            const Point3D P = m_meshes[i][j].getP();
            const Point3D Q = m_meshes[i][j].getQ();
            const Line2D le = Line2D(projectPoint(P, xMij, yMij, xLung, yLung), projectPoint(Q, xMij, yMij, xLung, yLung));
            projection.addLine(le);
        }
    }
    return Section(projection);
}

void Space3D::drawProjection(const int& xMij, const int& yMij, const int& xLung, const int& yLung){
    Section projection = projectMesh(xMij, yMij, xLung, yLung);
    for(size_t i = 0; i < projection.size(); i++) {
        line(projection[i].getP().getX(), projection[i].getP().getY(), projection[i].getQ().getX(), projection[i].getQ().getY());
    }
}
