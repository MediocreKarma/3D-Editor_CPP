#ifndef SPACE3D_H
#define SPACE3D_H
#include"../MyVector.h"
#include"ShapeData.h"

class Space3D
{
    public:
        Space3D();
        Space3D(MyVector<Line3D> mesh);
        //void draw(const int& xMij, const int& yMij);

    protected:

    private:
        MyVector<Line3D> m_mesh;

        Point2D projectPunct(const Point3D& pct, const int& xMij, const int& yMij) const;
        //pct3d=>pct2d
        //conform unei functii a lui patrut din carte
        //dar nu stiu cum se face efectiv

        MyVector<Line2D> project3D(const int& xMij, const int& yMij); //linie3d => linie2d
};

#endif // SPACE3D_H
