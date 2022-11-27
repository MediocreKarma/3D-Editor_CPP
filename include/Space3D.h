#ifndef SPACE3D_H
#define SPACE3D_H
#include"../MyVector.h"
#include"ShapeData.h"

class Space3D
{
    public:
        Space3D();
        Space3D(MyVector<Line3D>& mesh);
        void draw3D(const int& xMij, const int& yMij, const int& xLung, const int& yLung);

    protected:

    private:
        MyVector<Line3D> m_mesh;

        void rotateOX(MyVector<Line3D>& lines, double alpha);
        void rotateOY(MyVector<Line3D>& lines, double alpha);
        void rotateOZ(MyVector<Line3D>& lines, double alpha);

        Point2D projectPunct(const Point3D& pct, const int& xMij, const int& yMij, const int& xLung, const int& yLung) const;
        //pct3d=>pct2d
        //conform unei functii a lui patrut din carte
        //dar nu stiu cum se face efectiv

        MyVector<Line2D> project3D(const int& xMij, const int& yMij, const int& xLung, const int& yLung); //linie3d => linie2d
};

#endif // SPACE3D_H
