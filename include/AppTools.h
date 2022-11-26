#ifndef APPTOOLS_H
#define APPTOOLS_H

#include "../include/MyArray.h"
#include "graphics.h"
#include "winbgim.h"
#include <iostream>

class Label {
    public:
        Label();
        Label(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const int& fillColor);
        void drawLabel(const int& outlineColor = BLACK) const;
        void border(const int& outlineColor) const;
        void clear(const int& barColor) const;

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
        TextLabel(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const int& fillColor, const char* text);
        void drawTextLabel(const int& txtFont, const int& txtSize);
    private:
        MyArray<char, 128> m_text;
        void drawText(const int& txtFont, const int& txtSize);
};

class Button : public Label {
    public:
        Button();
        Button(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const int& fillColor);
        bool hitCollision(const int& x, const int& y) const;
};

class ImageButton : public Button {
    public:
        ImageButton();
        ImageButton(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const int& fC, const char* filename);
        void drawImageButton();

    private:
        MyArray<char, 128> filename;
};

class TextButton : public Button {
    public:
        TextButton();
        TextButton(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const int& fillColor, const char* p);
        void drawTextButton(const int& txtFont, const int& txtSize);

    protected:
        MyArray<char, 32> m_text;

    private:
        void drawText(const int& txtFont, const int& txtSize);
};

template<size_t lenList>
class DropdownButton : public TextButton {
    public:
        DropdownButton() :
            TextButton(), m_index(), m_listHeight(), m_listVisibility(false), m_list() {}

        DropdownButton(const int& xCenter_, const int& yCenter_, const int& xLen_, const int& yLen_,
                       const int& fillColor_, const char* p, const int& listHeight) :
            TextButton(xCenter_, yCenter_, xLen_, yLen_, fillColor_, p), m_index(0), m_listHeight(listHeight), m_listVisibility(false), m_list() {}

        void addOption(const char* p, const int& bkColor) {
            int yOffset = yCenter + yLen / 2;
            int option_yLen = m_listHeight / lenList;
            m_list[m_index] = TextButton(xCenter, yOffset + (2 * m_index + 1) * option_yLen / 2, xLen, option_yLen, bkColor, p);
            ++m_index;
        }

        void showList(const int& font, const int& fontSize) {
            if (isListVisible()) {
                return;
            }
            for (size_t i = 0; i < lenList; ++i) {
                m_list[i].drawTextButton(font, fontSize);
            }
            m_listVisibility = 1;
        }

        void hideList(const int& barColor) {
            if (!isListVisible()) {
                return;
            }
            setfillstyle(SOLID_FILL, barColor);
            bar(xCenter - xLen / 2, yCenter + yLen / 2 + 1,
            xCenter + xLen / 2 + 1, yCenter + yLen / 2 + m_listHeight + 1);
            m_listVisibility = 0;
        }

        void toggleVisibillity(const int& barColor, const int& font, const int& fontSize) {
            if (!isListVisible()) {
                showList(font, fontSize);
            }
            else {
                std::cout << "here";
                hideList(barColor);
            }
        }

        bool isListVisible() const {
            return m_listVisibility;
        }

        bool listHitCollision(const int& x, const int& y) const {
            return xCenter - xLen / 2 <= x && x <= xCenter + xLen / 2 &&
                    yCenter + yLen / 2 <= y && yCenter + yLen / 2 + m_listHeight;
        }

        TextButton& operator [] (const size_t& index) {
            return m_list[index];
        }

    private:
        size_t m_index;
        int m_listHeight;
        bool m_listVisibility;
        MyArray<TextButton, lenList> m_list;

        void changeText(const char* newText) {
            m_text = newText;
        }
};

#endif // APPTOOLS_H
