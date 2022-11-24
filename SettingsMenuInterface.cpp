#include <SettingsMenuInterface.h>
#include <MyArray.h>

static const int SETTINGS_WIDTH = 600;
static const int SETTINGS_HEIGHT = 400;
static const char* SETTINGS_TITLE = "Settings";
static const int BACKGROUND_COLOR = RGB(200, 200, 200);

class RectangleButton {
    public:
        RectangleButton() = default;

        RectangleButton(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen) :
            m_xCenter(xCenter), m_yCenter(yCenter), m_xLen(xLen), m_yLen(yLen) {}

        void drawButton(const int& color = BLACK) {
            setcolor(color);
            rectangle(m_xCenter - m_xLen / 2, m_yCenter - m_yLen / 2,
                      m_xCenter + m_xLen / 2, m_yCenter + m_yLen / 2);
        }

        bool isClicked(const int& x, const int& y) const {
            return m_xCenter - m_xLen / 2 <= x && m_xCenter + m_xLen / 2 <= x &&
                    m_yCenter - m_yLen / 2 <= y && m_yCenter + m_yLen / 2 <= y;
        }


    private:
        int m_xCenter;
        int m_yCenter;
        int m_xLen;
        int m_yLen;
};

class FlagButton : RectangleButton {

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

