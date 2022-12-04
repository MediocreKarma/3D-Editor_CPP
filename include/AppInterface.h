#ifndef APPINTERFACE_H
#define APPINTERFACE_H

#include "Space3D.h"
#include <graphics.h>
#include <winbgim.h>
#include "Menu.h"

class AppInterface {
    public:
        AppInterface();
        AppInterface(const int& appWidth = 600, const int& appHeight = 600, const int& theme = 0, const int& languagePackage = 0);
        void run();
        void drawTools();
        void drawEditor();
        double maxRadius() const;

    private:
        static constexpr double m_maxRadius = -2000;
        //Pozitia default a camerei e in shapeData, hardcoded; nu stiu unde sa pun exact offsetul
        int m_appWidth;
        int m_appHeight;
        int m_theme;
        int m_languagePackage;

        void clearMouse();
};

#endif // APPINTERFACE_H
