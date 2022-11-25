#include <SettingsMenuInterface.h>

using namespace SettingsMenuInterface;

RectangleButton::RectangleButton() = default;

RectangleButton::RectangleButton(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen) :
    m_xCenter(xCenter), m_yCenter(yCenter), m_xLen(xLen), m_yLen(yLen) {}

void RectangleButton::drawButton(const int& color = BLACK) {
    setcolor(color);
    rectangle(m_xCenter - m_xLen / 2, m_yCenter - m_yLen / 2,
              m_xCenter + m_xLen / 2, m_yCenter + m_yLen / 2);
}

bool RectangleButton::isClicked(const int& x, const int& y) const {
    return m_xCenter - m_xLen / 2 <= x && m_xCenter + m_xLen / 2 <= x &&
            m_yCenter - m_yLen / 2 <= y && m_yCenter + m_yLen / 2 <= y;
}


class FlagButton : RectangleButton {
    // nu stiu inca cum sa facem incarcarea pixelilor fiecarui steag
    // putem avea un fisier cu steag-uri din care sa generam informatiile
    // sau sa generam doar pentru cele 2(3) cazuri specifice: U.K., Ro., (Ru.?)
};

void SettingsMenuInterface::drawScreen() {
    setfillstyle(SOLID_FILL, BACKGROUND_COLOR);
    bar(0, 0, getmaxx(), getmaxy());
}

void SettingsMenuInterface::run() {
    initwindow(SETTINGS_WIDTH, SETTINGS_HEIGHT, SETTINGS_TITLE);
    drawScreen();
    MyArray<RectangleButton, 3> rectButtons;
}
