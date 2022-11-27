#ifndef MENUINTERFACE_H
#define MENUINTERFACE_H

#include "MyArray.h"
#include "MyVector.h"
#include <AppTools.h>
#include "graphics.h"
#include "winbgim.h"
#include"Space3D.h"
#include"ShapeData.h"

namespace MenuInterface{

    static const double razaMax = 20000;
    static const double FOV = 3.1415926/2;

    void run(const int& resX, const int& resY, const int& theme, const int& language);
    void project3D(Space3D& Space, const int& x1, const int& y1, const int& x2, const int& y2);

}


#endif // MENUINTERFACE_H
