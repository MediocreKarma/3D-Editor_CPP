#include "Space3D.h"

Space3D::Space3D() :
    m_maxRadius(), m_meshes() {}

Space3D::Space3D(MyVector<Mesh>& meshes, const double& maxRadius) :
     m_maxRadius(maxRadius), m_meshes(meshes) {}

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

/*
tot ce am scris aici este doar ca sa vad daca merge alg de proiectie
nimic nu e permanent, cred
*/

Point2D Space3D::projectPoint(const Point3D& pct, const int& xMij, const int& yMij, const int& xLung, const int& yLung) const {
    double xr = pct.getX();
    double zr = pct.getZ();
    double yr = pct.getY();
    double scale = (float(xLung) / getmaxx()) > float(yLung) / getmaxy() ? float(xLung) / getmaxx() : float(yLung) / getmaxy();
    double aa = (m_maxRadius - yr) / m_maxRadius;
    return Point2D((xr * aa * scale + double(xMij)), (yMij + yLung / 2 - (zr * aa  * scale + double(yMij))));
}

MyVector<Line2D> Space3D::project3D(const int& xMij, const int& yMij, const int& xLung, const int& yLung) {
    MyVector<Line2D> projection;
    for (size_t i = 0; i < m_meshes.size(); ++i) {
        //rotateOY(m_meshes[i], 30);
        //rotateOX(m_meshes[i], 60);
        for(size_t j = 0; j < m_meshes[i].size(); j++) {
            const Point3D P = m_meshes[i][j].getP();
            const Point3D Q = m_meshes[i][j].getQ();
            const Line2D le = Line2D(projectPoint(P, xMij, yMij, xLung, yLung), projectPoint(Q, xMij, yMij, xLung, yLung));
            projection.push_back(le);
        }
    }
    return projection;
}

void Space3D::draw3D(const int& xMij, const int& yMij, const int& xLung, const int& yLung){
    MyVector<Line2D> projection = project3D(xMij, yMij, xLung, yLung);
    for(size_t i = 0; i < projection.size(); i++){
        line(projection[i].getP().getX(), projection[i].getP().getY(), projection[i].getQ().getX(), projection[i].getQ().getY());
    }
}
