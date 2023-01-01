#ifndef APPTOOLS_H
#define APPTOOLS_H

#include "MyArray.h"
#include <graphics.h>
#include <winbgim.h>

namespace ColorSchemes {
    static const int PRIMARYCOLOR = 0;
    static const int SECONDARYCOLOR = 1;
    static const int ACCENTCOLOR = 2;
    static const int ACCENTCOLOR_COUNT = 5;
    //only works for RGB values; used for highlighting purposes
    int mixColors(const int& color1, const int& color2, const uint8_t& percentage);
    static const MyArray<int, ACCENTCOLOR_COUNT> accentColors = {
        /*RED, BLUE, PINK, YELLOW, GREEN - RGB values to use with above function*/
        RGB(219, 43, 57), RGB(110, 68, 255), RGB(249, 145, 204), RGB(233, 196, 106), RGB(42, 157, 143)
    };
    static const MyArray<MyArray<int, 3>, ACCENTCOLOR_COUNT * 2> themeColors = {
        //cand se schimba accentColor se va trece defapt la a doua jumatate a lu themeColors
        //ca sa nu mai schimbam in alte parti, si sa pastram oricum un singur index pt m_theme

        /*LIGHT MODE*/{WHITE, BLACK, accentColors[0]}, {WHITE, BLACK, accentColors[1]}, {WHITE, BLACK, accentColors[2]},
                      {WHITE, BLACK, accentColors[3]}, {WHITE, BLACK, accentColors[4]},
        /*DARK MODE */{BLACK, WHITE, accentColors[0]}, {BLACK, WHITE, accentColors[1]}, {BLACK, WHITE, accentColors[2]},
                      {BLACK, WHITE, accentColors[3]}, {BLACK, WHITE, accentColors[4]}
    };
    static const int NO_COLOR = -1;
}

class Label {
    public:
        Label();
        Label(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen);
        void drawLabel(const int& fillColor, const int& outlineColor = BLACK) const;
        void border(const int& outlineColor) const;
        void clear(const int& barColor) const;
        int getXCenter() const;
        int getYCenter() const;

    protected:
        int xCenter;
        int yCenter;
        int xLen;
        int yLen;
};

class CircularLabel {
    public:
        CircularLabel();
        CircularLabel(const int& xCenter, const int& yCenter, const int& radius);
        void drawLabel(const int& fillColor, const int& outlineColor = BLACK);
        void border(const int& outlineColor) const;
        void clear(const int& barColor) const;
        void move(const int& x, const int& y);
        int getX() const;
        int getY() const;
        int getRadius() const;

    protected:
        int xCenter;
        int yCenter;
        int radius;
};

class CircularButton : public CircularLabel {
    public:
        CircularButton();
        CircularButton(const int& xCenter, const int& yCenter, const int& radius);
        bool hitCollision(const int& x, const int& y) const;
};

class DonutButton : public CircularButton {
    public:
        DonutButton();
        DonutButton(const int& xCenter, const int& yCenter, const int& radius, const int& donutLen);
        bool hitCollision(const int& x, const int& y) const;

    private:
        CircularButton m_outerRing;
        CircularButton m_innerRing;
};

class TextLabel : public Label {
    public:
        TextLabel();
        TextLabel(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const char* text);
        void drawTextLabel(const int& txtFont, const int& txtSize, const int& fillColor);
    private:
        MyArray<char, 128> m_text;
        void drawText(const int& /*txtFont*/, const int& /*txtSize*/, const int& fillColor);
};

class Button : public Label {
    public:
        Button();
        Button(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen);
        bool hitCollision(const int& x, const int& y) const;
};

class ImageButton : public Button {
    public:
        ImageButton();
        ImageButton(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const char* filename);
        void drawImageButton();

    private:
        MyArray<char, 256> filename;
};

class TextButton : public Button {
    public:
        TextButton();
        TextButton(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const char* p);
        void drawTextButton(const int& txtFont, const int& txtSize, const int& fillColor, const bool& centerText = true);
        const char* getText();
        void modifyText(const MyArray<char, 32>& newText);
        void move(int x, int y);

    protected:
        MyArray<char, 32> m_text;

    private:
        void drawText(const int& /*txtFont*/, const int& /*txtSize*/, const int& fillColor, const bool& centerText = true);
};

template<size_t lenList>
class DropdownButton : public TextButton {
    public:
        DropdownButton() :
            TextButton(), m_index(), m_listWidth(), m_listHeight(), m_listVisibility(false), m_list() {}

        DropdownButton(const int& xCenter_, const int& yCenter_, const int& xLen_, const int& yLen_, const char* p, const int& listWidth, const int& listHeight) :
            TextButton(xCenter_, yCenter_, xLen_, yLen_, p), m_index(0), m_listWidth(listWidth), m_listHeight(listHeight), m_listVisibility(false), m_list() {}

        void addOption(const char* p) {
            int yOffset = yCenter + yLen / 2;
            int option_yLen = m_listHeight / lenList;
            m_list[m_index] = TextButton(xCenter - xLen / 2 + m_listWidth / 2, yOffset + (2 * m_index + 1) * option_yLen / 2, m_listWidth, option_yLen, p);
            ++m_index;
        }

        void changeOptionText(int index, const char* p) {
            int yOffset = yCenter + yLen / 2;
            int option_yLen = m_listHeight / lenList;
            m_list[index] =  TextButton(xCenter - xLen / 2 + m_listWidth / 2, yOffset + (2 * index + 1) * option_yLen / 2, m_listWidth, option_yLen, p);
        }

        void showList(const int& font, const int& fontSize, const int& fillColor, const bool& centerText = true) {
            for (size_t i = 0; i < lenList; ++i) {
                m_list[i].drawTextButton(font, fontSize, fillColor, centerText);
            }
            m_listVisibility = 1;
        }

        void hideList(const int& barColor) {
            m_listVisibility = 0;
            if (barColor == -1) {
                return;
            }
            setfillstyle(SOLID_FILL, barColor);
            bar(xCenter - xLen / 2 - 1, yCenter + yLen / 2 + 1, xCenter + xLen / 2 + 1, yCenter + yLen / 2 + m_listHeight + 1);
        }

        void toggleVisibillity(const int& barColor, const int& font, const int& fontSize, const int& fillColor) {
            if (!isListVisible()) {
                showList(font, fontSize, fillColor);
            }
            else {
                hideList(barColor);
            }
        }

        void move(int x, int y) {
            int deltax = x - xCenter;
            int deltay = y - yCenter;
            xCenter = x;
            yCenter = y;
            for (size_t i = 0; i < lenList; ++i) {
                m_list[i].move(m_list[i].getXCenter() + deltax, m_list[i].getYCenter() + deltay);
            }
        }

        int width() const {
            return (yLen > m_listWidth) ? yLen : m_listWidth;
        }

        int height() const {
            return m_listHeight + yLen;
        }

        bool isListVisible() const {
            return m_listVisibility;
        }

        void setVisibility(const bool& vis) {
            m_listVisibility = vis;
        }

        void changeMain(const int& index, const int& font, const int& fontSize, const int& fillColor) {
            const char* p = m_list[index].getText();
            changeText(p);
            drawTextButton(font, fontSize, fillColor);
        }

        int listHitCollision(const int& x, const int& y) {
            if (xCenter - xLen / 2 <= x && x <= xCenter - xLen / 2 + m_listWidth && yCenter + yLen / 2 <= y && y <= yCenter + yLen / 2 + m_listHeight) {
                return (y - yCenter - yLen / 2) / (m_listHeight / lenList);
            }
            return -1;
        }

        TextButton& operator [] (const size_t& index) {
            return m_list[index];
        }

    private:
        size_t m_index;
        int m_listWidth;
        int m_listHeight;
        bool m_listVisibility;
        MyArray<TextButton, lenList> m_list;

        void changeText(const char* newText) {
            m_text = newText;
        }
};

#endif // APPTOOLS_H
