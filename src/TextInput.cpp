#include "TextInput.h"
#include <iostream>

TextInputBox::TextInputBox(const int& areaBegin, const int& areaEnd, const int& height, const int& textColor, const int& fillColor, const int& maxSize, const char* acceptedInput) :
                m_areaBegin(areaBegin), m_areaEnd(areaEnd), m_displayBegin(0), m_displayEnd(0), m_height(height), m_index(0), m_textColor(textColor), m_fillColor(fillColor), m_maxSize(maxSize),
                m_pressedEnter(false), m_mouseclick{-1, -1}, m_text(nullptr), m_accepted(acceptedInput) {}

bool TextInputBox::isAccepted(const char& ch) const {
    if (m_accepted[0] == 0) {
        return true;
    }
    if (ch == 0 || ch == '\r' || ch == '\b') {
        return true;
    }
    for (const char& x : m_accepted) {
        if (x == ch) {
            return true;
        }
        if (x == 0) {
            break;
        }
    }
    return false;
}

void TextInputBox::getClick(int& x, int& y) const {
    x = m_mouseclick[0];
    y = m_mouseclick[1];
}

bool TextInputBox::pressedEnter() {
    if (m_pressedEnter) {
        m_pressedEnter = false;
        return true;
    }
    return false;
}

void TextInputBox::drawText() {
    clearText();
    MyArray<char, MAX_TEXT_SIZE> outText = displayText(m_displayBegin, m_displayEnd);
    outtextxy(m_areaBegin, m_height - textheight(outText.data()) / 2, outText.data());
}

bool TextInputBox::isClickInTextbox(const int& x, const int& y) {
    return x >= m_areaBegin - 5 && x <= m_areaEnd + 5 && y >= m_height - 10 && y <= m_height + 10;
}

MyArray<char, TextInputBox::MAX_TEXT_SIZE> TextInputBox::getText(const MyArray<char, 256>& startText) {
    m_text = startText;
    m_mouseclick = {-1, -1};
    size_t lenText = strlen(m_text.data());
    incrementBeginEnd(m_displayBegin, m_displayEnd, lenText);
    display(m_displayBegin, m_displayEnd);
    char ch;
    while (true) {
        while (!kbhit()) {
            if (ismouseclick(WM_LBUTTONDOWN)) {
                int x, y;
                getmouseclick(WM_LBUTTONDOWN, x, y);
                if (!isClickInTextbox(x, y)) {
                    m_mouseclick = {x, y};
                    return m_text;
                }
            }
        }
        ch = getch();
        if (isAccepted(ch)) {
            break;
        }
    }
    while ((!isspace(ch) || ch == 0 || ch == ' ')) {
        if (ch == '\b') {
            backspaceIndex(lenText);
            decrementBeginEnd(m_displayBegin, m_displayEnd);
        }
        else if (lenText != m_maxSize && isprint(ch)) {
            insertIndex(lenText, ch);
            incrementBeginEnd(m_displayBegin, m_displayEnd, lenText);
        }
        else if (ch == 0) {
            ch = getch();
            if (ch == KEY_LEFT) {
                if (m_index) {
                    --m_index;
                    decrementBeginEnd(m_displayBegin, m_displayEnd);
                }
            }
            else if (ch == KEY_RIGHT) {
                if (m_index < lenText) {
                    ++m_index;
                    incrementBeginEnd(m_displayBegin, m_displayEnd);
                }
            }
            else if (ch == KEY_DELETE) {
                m_text = MyArray<char, MAX_TEXT_SIZE>(nullptr);
                lenText = 0;
                m_index = 0;
            }
        }
        display(m_displayBegin, m_displayEnd);

        //am bagat continuturile lu checkwindowclose aici
        //ca n am stiut cum altfel sa bag si isclickintextbox
        //mai sus am lasat la fel
        //ca daca n ai nimic scris n are rost sa te joci cu cursoru
        //trebe facuta o functie getClick care returneaza 0 sau 1 sau 2, cred

        while (true) {
            while (!kbhit()) {
                if (ismouseclick(WM_LBUTTONDOWN)) {
                    int x, y;
                    getmouseclick(WM_LBUTTONDOWN, x, y);
                    if (!isClickInTextbox(x, y)) {
                        m_mouseclick = {x, y};
                        return m_text;
                    }
                }
            }
            ch = getch();
            if (isAccepted(ch)) {
                break;
            }
        }
    }
    m_pressedEnter = true;
    return m_text;
}

void TextInputBox::changeIndexByClick(const int& x, size_t& begin, size_t& end) {
    if (x <= m_areaBegin) {
        m_index = begin;
    }
    if (x >= m_areaBegin + textwidth(displayText(begin, end).data())) {
        m_index = end;
    }
    for (size_t index = begin; index < end; index++) {
        if (x <= m_areaBegin + textwidth(displayText(begin, index + 1).data())) {
            m_index = index;
            break;
        }
    }
    display(begin, end);
}

void TextInputBox::incrementBeginEnd(size_t& begin, size_t& end, const size_t& lenText) {
    while (end < m_index) {
        ++end;
    }
    while (end < lenText && textwidth(displayText(begin, end).data()) <= m_areaEnd - m_areaBegin) {
        ++end;
    }
    while (textwidth(displayText(begin, end).data()) > m_areaEnd - m_areaBegin) {
        ++begin;
    }
}

void TextInputBox::decrementBeginEnd(size_t& begin, size_t& end) {
    if (begin && begin >= m_index) {
        --begin;
    }
    while (textwidth(displayText(begin, end).data()) > m_areaEnd - m_areaBegin) {
        --end;
    }
}

void TextInputBox::backspaceIndex(size_t& len) {
    if (!m_index || !len) {
        return;
    }
    --len;
    for (size_t j = m_index - 1; j < len; ++j) {
        m_text[j] = m_text[j + 1];
    }
    m_text[len] = 0;
    --m_index;
}

void TextInputBox::insertIndex(size_t& len, const char& key) {
    for (int j = len; j > (int)m_index; --j) {
        m_text[j] = m_text[j - 1];
    }
    ++len;
    m_text[m_index] = key;
    ++m_index;
}

void TextInputBox::displayCursor(const MyArray<char, MAX_TEXT_SIZE>& outText, const size_t& from) {
    MyArray<char, MAX_TEXT_SIZE> textBeforeCursor = outText;
    textBeforeCursor[m_index - from] = 0;
    int len = textwidth(textBeforeCursor.data());
    line(m_areaBegin + len, m_height - 8, m_areaBegin + len, m_height + 8);
}

MyArray<char, TextInputBox::MAX_TEXT_SIZE> TextInputBox::displayText(const size_t& from, const size_t& to) {
    MyArray<char, MAX_TEXT_SIZE> outText = {0};
    for (size_t index = 0; index + from < to && m_text[index + from]; ++index) {
        outText[index] = m_text[index + from];
    }
    return outText;
}

void TextInputBox::display(const size_t& from, const size_t& to) {
    clearText();
    MyArray<char, MAX_TEXT_SIZE> outText = displayText(from, to);
    outtextxy(m_areaBegin, m_height - textheight(outText.data()) / 2, outText.data());
    displayCursor(outText, from);
}

void TextInputBox::clearText() {
    setfillstyle(SOLID_FILL, m_fillColor);
    bar(m_areaBegin - 5 + 1, m_height - 10 + 1, m_areaEnd + 5, m_height + 10);
}
