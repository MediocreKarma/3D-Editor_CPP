#include "FileHandler.h"
#include <iostream>

FileHandler::FileHandler() :
    m_width(600), m_height(300), m_index(0), m_xButton(575, 25, 50, 50, "media\\xButton.jpg") {}

void FileHandler::initSaveWindow() {
    initwindow(m_width, m_height, "Save space", getmaxwidth() / 2 - m_width / 2, getmaxheight() / 2 - m_height / 2, false, false);
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(0, 0, m_width, m_height);
    setcolor(BLACK);
    setbkcolor(LIGHTGRAY);
    MyArray<char, 20> auxText = "Enter save path:";
    outtextxy(WriteAreaBegin - 5, m_height / 2 - 30, auxText.data());
    rectangle(WriteAreaBegin - 5, m_height / 2 - 10, WriteAreaEnd + 5, m_height / 2 + 10);
    m_xButton.drawImageButton();
    m_xButton.border(LIGHTGRAY);
    setfillstyle(SOLID_FILL, WHITE);
    setbkcolor(WHITE);
    setcolor(BLACK);
    clearText();
}

void FileHandler::initOpenWindow() {
    initwindow(m_width, m_height, "Open space", getmaxwidth() / 2 - m_width / 2, getmaxheight() / 2 - m_height / 2, false, false);
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(0, 0, m_width, m_height);
    setcolor(BLACK);
    setbkcolor(LIGHTGRAY);
    MyArray<char, 20> auxText = "Enter open path:";
    outtextxy(WriteAreaBegin - 5, m_height / 2 - 30, auxText.data());
    rectangle(WriteAreaBegin - 5, m_height / 2 - 10, WriteAreaEnd + 5, m_height / 2 + 10);
    setfillstyle(SOLID_FILL, WHITE);
    m_xButton.drawImageButton();
    m_xButton.border(LIGHTGRAY);
    setfillstyle(SOLID_FILL, WHITE);
    setbkcolor(WHITE);
    setcolor(BLACK);
    clearText();
}

bool FileHandler::isClickInTextbox(const int& x, const int& y) {
    return x >= WriteAreaBegin - 5 && x <= WriteAreaEnd + 5 && y >= m_height / 2 - 10 && y <=m_height / 2 + 10;
}

void FileHandler::clearText() {
    bar(WriteAreaBegin - 5 + 1, m_height / 2 - 10 + 1, WriteAreaEnd + 5, m_height / 2 + 10);
}

bool FileHandler::checkWindowClose() {
    while (!kbhit()) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if (m_xButton.hitCollision(x, y)) {
                return true;
            }
        }
    }
    return false;
}

bool FileHandler::checkClickInTextbox() {
    while (!kbhit()) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if (isClickInTextbox(x, y)) {
                return true;
            }
        }
    }
    return false;
}

MyArray<char, 256> FileHandler::getFilename() {
    MyArray<char, 256> text = {0};
    size_t lenText = 0;
    displayCursor({0}, 0);
    size_t displayBegin = 0;
    size_t displayEnd = 0;
    if(checkWindowClose()) {
        closegraph(CURRENT_WINDOW);
        return {0};
    }
    char ch = getch();
    while ((!isspace(ch) || ch == 0 || ch == ' ')) {
        if (ch == '\b') {
            backspaceIndex(text, lenText);
            decrementBeginEnd(text, displayBegin, displayEnd);
        }
        else if (lenText != 255 && isprint(ch)) {
            insertIndex(text, lenText, ch);
            incrementBeginEnd(text, displayBegin, displayEnd, lenText);
        }
        else if (ch == 0) {
            ch = getch();
            if (ch == KEY_LEFT) {
                if (m_index) {
                    --m_index;
                    decrementBeginEnd(text, displayBegin, displayEnd);
                }
            }
            else if (ch == KEY_RIGHT) {
                if (m_index < lenText) {
                    ++m_index;
                    incrementBeginEnd(text, displayBegin, displayEnd);
                }
            }
            else if (ch == KEY_DELETE) {
                text = MyArray<char, 256>();
                lenText = 0;
                m_index = 0;
            }
        }
        display(text, displayBegin, displayEnd);

        //am bagat continuturile lu checkwindowclose aici
        //ca n am stiut cum altfel sa bag si isclickintextbox
        //mai sus am lasat la fel
        //ca daca n ai nimic scris n are rost sa te joci cu cursoru
        //trebe facuta o functie getClick care returneaza 0 sau 1 sau 2, cred

        while (!kbhit()) {
            if (ismouseclick(WM_LBUTTONDOWN)) {
                int x, y;
                getmouseclick(WM_LBUTTONDOWN, x, y);
                if (m_xButton.hitCollision(x, y)) {
                    closegraph(CURRENT_WINDOW);
                    return {0};
                }
                else if (isClickInTextbox(x, y)) {
                    changeIndexByClick(x, text, displayBegin, displayEnd);
                }
            }
        }
        ch = getch();
    }
    closegraph(CURRENT_WINDOW);
    return text;
}

void FileHandler::changeIndexByClick(const int& x, const MyArray<char, 256>& text, size_t& begin, size_t& end) {
    if (x <= WriteAreaBegin) {
        m_index = begin;
    }
    if (x >= WriteAreaBegin + textwidth(displayText(text, begin, end).data())) {
        m_index = end;
    }
    for (size_t index = begin; index < end; index++) {
        if (x <= WriteAreaBegin + textwidth(displayText(text, begin, index + 1).data())) {
            m_index = index;
            break;
        }
    }
    display(text, begin, end);
}

void FileHandler::incrementBeginEnd(const MyArray<char, 256>& text, size_t& begin, size_t& end, const size_t& lenText) {
    while (end < m_index) {
        ++end;
    }
    while (end < lenText && textwidth(displayText(text, begin, end).data()) <= WriteAreaEnd - WriteAreaBegin) {
        ++end;
    }
    while (textwidth(displayText(text, begin, end).data()) > WriteAreaEnd - WriteAreaBegin) {
        ++begin;
    }
}

void FileHandler::decrementBeginEnd(const MyArray<char, 256>& text, size_t& begin, size_t& end) {
    if (begin && begin >= m_index) {
        --begin;
    }
    while (textwidth(displayText(text, begin, end).data()) > WriteAreaEnd - WriteAreaBegin) {
        --end;
    }
}

void FileHandler::backspaceIndex(MyArray<char, 256>& text, size_t& len) {
    if (!m_index || !len) {
        return;
    }
    --len;
    for (size_t j = m_index - 1; j < len; ++j) {
        text[j] = text[j + 1];
    }
    text[len] = 0;
    --m_index;
}

void FileHandler::insertIndex(MyArray<char, 256>& text, size_t& len, const char& key) {
    for (int j = len; j > (int)m_index; --j) {
        text[j] = text[j - 1];
    }
    ++len;
    text[m_index] = key;
    ++m_index;
}

void FileHandler::displayCursor(const MyArray<char, displayableText>& outText, const size_t& from) {
    MyArray<char, displayableText> textBeforeCursor = outText;
    textBeforeCursor[m_index - from] = 0;
    int len = textwidth(textBeforeCursor.data());
    line(WriteAreaBegin + len, m_height / 2 - 10, WriteAreaBegin + len, m_height / 2 + 10);
}

MyArray<char, FileHandler::displayableText> FileHandler::displayText(const MyArray<char, 256>& text, const size_t& from, const size_t& to) {
    MyArray<char, displayableText> outText = {0};
    for (size_t index = 0; index + from < to && text[index + from]; ++index) {
        outText[index] = text[index + from];
    }
    return outText;
}

void FileHandler::display(const MyArray<char, 256>& text, const size_t& from, const size_t& to) {
    clearText();
    MyArray<char, displayableText> outText = displayText(text, from, to);
    outtextxy(WriteAreaBegin, m_height / 2 - textheight(outText.data()) / 2, outText.data());
    displayCursor(outText, from);
}

//TODO: Add "Path with spaces" support
MyArray<char, 512> FileHandler::correctPath(const MyArray<char, 256>& text) {
    MyArray<char, 512> moddedText = {0};
    size_t i = 0, j = 0;
    while (text[i]) {
        moddedText[j++] = text[i];
        if (text[i] == '\\') {
            moddedText[j++] = '\\';
            if (text[i + 1] == '\\') {
                ++i;
            }
        }
        ++i;
    }
    if (text[i - 1] == '\\') {
        j -= 2;
    }
    addExtension(moddedText, j);
    return moddedText;
}

void FileHandler::addExtension(MyArray<char, 512>& text, size_t& len) {
    const MyArray<char, 8> extension = ".e3dspc";
    bool hasExtension = true;
    for (size_t i = 0; i < 7; ++i) {
        if (text[len - 7 + i] != extension[i]) {
            hasExtension = false;
            break;
        }
    }
    if (hasExtension) {
        return;
    }
    for (size_t i = 0; i < 8; ++i) {
        text[len++] = extension[i];
    }
}

MyArray<char, 512> FileHandler::userSavePath() {
    m_index = 0;
    initSaveWindow();
    return correctPath(getFilename());
}

MyArray<char, 512> FileHandler::userOpenPath() {
    m_index = 0;
    initOpenWindow();
    return correctPath(getFilename());
}

