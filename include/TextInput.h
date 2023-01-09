#ifndef TEXTINPUT_H
#define TEXTINPUT_H

#include "MyVector.h"
#include "MyArray.h"
#include <graphics.h>
#include <winbgim.h>

class TextInputBox
{
    public:
        static const int MAX_TEXT_SIZE = 256;

        TextInputBox(const int areaBegin, const int areaEnd, const int height, const int textColor, const int fillColor, const int m_maxSize, const char* acceptedInput = nullptr);
        MyArray<char, MAX_TEXT_SIZE> getText(const MyArray<char, 256>& startText = nullptr);
        void getClick(int& x, int& y) const;
        bool pressedEnter();
        void drawText();

    private:
        int m_areaBegin;
        int m_areaEnd;
        size_t m_displayBegin;
        size_t m_displayEnd;
        int m_height;
        size_t m_index;
        int m_textColor;
        int m_fillColor;
        size_t m_maxSize;
        bool m_pressedEnter;
        MyArray<int, 2> m_mouseclick;
        MyArray<char, MAX_TEXT_SIZE> m_text;
        MyArray<char, MAX_TEXT_SIZE> m_accepted;

        bool checkClickInTextbox();
        void clearText();
        void backspaceIndex(size_t& len);
        void insertIndex(size_t& len, const char& key);
        MyArray<char, MAX_TEXT_SIZE> displayText(const size_t from, const size_t to);
        void displayCursor(const MyArray<char, MAX_TEXT_SIZE>& outText, const size_t from);
        void decrementBeginEnd(size_t& m_displayBegin, size_t& end);
        void incrementBeginEnd(size_t& m_displayBegin, size_t& end, const size_t lenText = 0);
        void changeIndexByClick(const int x, size_t& m_displayBegin, size_t& end);
        void display(const size_t from, const size_t to);
        bool isClickInTextbox(const int x, const int y);
        bool isAccepted(const char& ch) const;
};

class NumericInputBox {
    public:
        NumericInputBox(const int areaBegin, const int areaEnd, const int height, const int textColor,
                        const int fillColor);
        double getDoubleValue();
        int getIntegerValue();
        void getClick(int &x, int& y);
        bool isEmpty();

        static constexpr size_t MAX_NUMBER_SIZE = 12;

        using Text = MyArray<char, MAX_NUMBER_SIZE>;

    private:
        int m_areaBegin;
        int m_areaEnd;
        int m_height;
        int m_index;
        int m_len;
        Text m_numberTxt;
        MyArray<int, 2> m_mouseClick;
        bool m_hasPoint;

        bool isClickInTextbox(const int x, const int y);
        void checkClick();
        void backspaceIndex();
        void insertIndex(const char key);
        void clearText();
        void displayText();
        void displayCursor();
        void display();
        void changeIndexByClick(int x);
        bool isAcceptedForDouble(const char key) const;
        bool isAcceptedForInteger(const char key) const;
};

#endif // TEXTINPUT_H
