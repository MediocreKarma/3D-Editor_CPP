#ifndef APPTOOLS_H
#define APPTOOLS_H

#include "../include/MyArray.h"
#include "graphics.h"
#include "winbgim.h"

class Label {
    public:
        Label();
        Label(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const int& fillColor);
        void drawLabel(const int& outlineColor = BLACK);
        void clear(const int& barColor);

    protected:
        int xCenter;
        int yCenter;
        int xLen;
        int yLen;
        int fillColor;
};
class TextLabel : public Label {
public:
    TextLabel();
    TextLabel(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const int& fillColor, const char*& text);
    void drawTextLabel(const int& txtFont, const int& txtSize);
private:
    MyArray<char, 128> m_text;
    void drawText(const int& txtFont, const int& txtSize);
};

class Button : public Label {
    public:
        Button();
        Button(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const int& fillColor);
        bool hitCollision(const int& x, const int& y);
};
class ImageButton : public Button {
public:
    ImageButton();
    ImageButton(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const int& fC, const char*& filename);

    void drawImageButton();
private:
    MyArray<char, 128> filename;
};
class TextButton : public Button {
    public:
        TextButton();
        TextButton(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const int& fillColor, const char* p);
        void drawTextButton(const int& txtFont, const int& txtSize);
    private:
        MyArray<char, 32> m_text;

        void drawText(const int& txtFont, const int& txtSize);
};

template<size_t lenList>
class DropdownButton : public TextButton {
    public:
        DropdownButton() = default;

        DropdownButton(const int& xCenter_, const int& yCenter_, const int& xLen_, const int& yLen_,
                       const int& fillColor_, const char* p, const int& listWidth, const int& listHeight) :
            TextButton(xCenter_, yCenter_, xLen_, yLen_, fillColor_, p), m_index(0), m_listWidth(listWidth), m_listHeight(listHeight), isListVisible(false), m_list() {}

        void addOption(const char* p) {
            int yOffset = yCenter + yLen / 2;
            int option_yLen = m_listHeight / lenList;
            m_list[m_index] = TextButton(xCenter, yOffset + (2 * m_index + 1) * option_yLen / 2, m_listWidth, option_yLen, LIGHTGRAY, p);
            ++m_index;
        }

        void drawList(const int& color) {
            if (isListVisible) {
                return;
            }
            setfillstyle(SOLID_FILL, color);
            bar(xCenter + xLen / 2, yCenter + yLen / 2,
            xCenter + xLen / 2, yCenter + yLen / 2 + m_listHeight);
            for (size_t i = 0; i < lenList; ++i) {
                m_list[i].drawTextButton(1, 2);
            }
            isListVisible = 1;
        }

        void hideList(const int& barColor) {
            if (!isListVisible) {
                return;
            }
            setfillstyle(SOLID_FILL, barColor);
            bar(xCenter - xLen / 2, yCenter + yLen / 2 + 1,
            xCenter + xLen / 2 + 1, yCenter + yLen / 2 + m_listHeight + 1);
            isListVisible = 0;
        }

        bool listHitCollision(const int& x, const int& y) {
            return xCenter - xLen / 2;
        }

    private:
        size_t m_index;
        int m_listWidth;
        int m_listHeight;
        bool isListVisible;
        MyArray<TextButton, lenList> m_list;
};

#endif // APPTOOLS_H
