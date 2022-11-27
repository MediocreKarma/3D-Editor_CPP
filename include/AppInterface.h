#ifndef APPINTERFACE_H
#define APPINTERFACE_H

#include <Space3D.h>
#include <graphics.h>
#include <winbgim.h>

class AppInterface {
    public:
        AppInterface();
        AppInterface(const int& appWidth = 600, const int& appHeight = 600, const int& theme = 0, const int& languagePackage = 0);
        void run();
        void project3D(Space3D& Space, const int& x1, const int& y1, const int& x2, const int& y2);
        void drawTools();
        void drawEditor();
        double maxRadius() const;

    private:
        static constexpr double m_maxRadius = 2000;
        static constexpr double FOV = 3.1415926 / 2;
        int m_appWidth;
        int m_appHeight;
        int m_theme;
        int m_languagePackage;
};

#endif // APPINTERFACE_H
