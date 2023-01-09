#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <graphics.h>
#include <winbgim.h>
#include "AppTools.h"
#include "TextInput.h"
#include "MyVector.h"

class FileHandler {
    public:
        static const int WriteAreaBegin = 100;
        static const int WriteAreaEnd = 500;

        FileHandler(int language = 0);
        MyArray<char, 512> userSavePath();
        MyArray<char, 512> userOpenPath();
        void setLanguage(int language);

    private:
        int m_width;
        int m_height;
        TextInputBox txtBox;
        ImageButton m_xButton;
        int m_language;

        void initSaveWindow();
        void initOpenWindow();
        void clearText();
        MyArray<char, 512> correctPath(const MyArray<char, 256>& text);
        void addExtension(MyArray<char, 512>& text, size_t& len);
        bool checkWindowClose();
        bool isClickInTextbox(const int x, const int y);
        bool checkClickInTextbox();
        void changeIndexByClick(const int x, const MyArray<char, 256>& text, size_t& m_displayBegin, size_t& end);

};

#endif // FILEHANDLER_H
