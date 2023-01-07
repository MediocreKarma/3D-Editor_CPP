#include "FileHandler.h"

FileHandler::FileHandler(int language) :
    m_width(600), m_height(300), txtBox(100, 500, 150, BLACK, WHITE, 256),
    //recycling
    m_xButton(575, 20, 30, 30, "media\\buttonDelete.gif"), m_language(language) {}

void FileHandler::setLanguage(int language) {
    m_language = language;
}

void FileHandler::initSaveWindow() {
    txtBox = TextInputBox(100, 500, 150, BLACK, WHITE, 256);
    initwindow(m_width, m_height, Language::Text[(int)Lang::Save_Space][m_language].data(), getmaxwidth() / 2 - m_width / 2, getmaxheight() / 2 - m_height / 2, false, false);
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(0, 0, m_width, m_height);
    setcolor(BLACK);
    setbkcolor(LIGHTGRAY);
    MyArray<char, 32> auxText = Language::Text[(int)Lang::Enter_Save_Path][m_language];
    outtextxy(WriteAreaBegin - 5, m_height / 2 - 30, auxText.data());
    rectangle(WriteAreaBegin - 5, m_height / 2 - 10, WriteAreaEnd + 5, m_height / 2 + 10);
    m_xButton.drawImageButton();
    m_xButton.border(LIGHTGRAY);
    setfillstyle(SOLID_FILL, WHITE);
    setbkcolor(WHITE);
    setcolor(BLACK);
}

void FileHandler::initOpenWindow() {
    txtBox = TextInputBox(100, 500, 150, BLACK, WHITE, 256);
    initwindow(m_width, m_height, Language::Text[(int)Lang::Open_Space][m_language].data(), getmaxwidth() / 2 - m_width / 2, getmaxheight() / 2 - m_height / 2, false, false);
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(0, 0, m_width, m_height);
    setcolor(BLACK);
    setbkcolor(LIGHTGRAY);
    MyArray<char, 32> auxText = Language::Text[(int)Lang::Enter_Open_Path][m_language];
    outtextxy(WriteAreaBegin - 5, m_height / 2 - 30, auxText.data());
    rectangle(WriteAreaBegin - 5, m_height / 2 - 10, WriteAreaEnd + 5, m_height / 2 + 10);
    setfillstyle(SOLID_FILL, WHITE);
    m_xButton.drawImageButton();
    m_xButton.border(LIGHTGRAY);
    setfillstyle(SOLID_FILL, WHITE);
    setbkcolor(WHITE);
    setcolor(BLACK);
}

//TODO: Add "Path with spaces" support
MyArray<char, 512> FileHandler::correctPath(const MyArray<char, 256>& text) {
    MyArray<char, 512> moddedText = {0};
     if (!text[0]) {
        return moddedText;
    }
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
    initSaveWindow();
    MyArray<char, 256> savePath = txtBox.getText();
    while (!txtBox.pressedEnter()) {
        int x, y;
        txtBox.getClick(x, y);
        if (m_xButton.hitCollision(x, y)) {
            savePath.fill(0);
            break;
        }
        savePath = txtBox.getText(savePath);
    }
    closegraph(getcurrentwindow());
    return correctPath(savePath);
}

MyArray<char, 512> FileHandler::userOpenPath() {
    initOpenWindow();
    MyArray<char, 256> openPath = txtBox.getText();
    while (!txtBox.pressedEnter()) {
        int x, y;
        txtBox.getClick(x, y);
        if (m_xButton.hitCollision(x, y)) {
            openPath.fill(0);
            break;
        }
        openPath = txtBox.getText(openPath);
    }
    closegraph(getcurrentwindow());
    return correctPath(openPath);
}

