#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <graphics.h>
#include <winbgim.h>
#include "AppTools.h"
#include <conio.h>

class FileHandler {
    public:
        static const int displayableText = 100;
        static const int WriteAreaBegin = 100;
        static const int WriteAreaEnd = 500;


        FileHandler();
        MyArray<char, 512> userSavePath();
        MyArray<char, 512> userOpenPath();

    private:
        int m_width;
        int m_height;
        size_t m_index;
        ImageButton m_xButton;

        void initSaveWindow();
        void initOpenWindow();
        void clearText();
        MyArray<char, 256> getFilename();
        void backspaceIndex(MyArray<char, 256>& text, size_t& len);
        void insertIndex(MyArray<char, 256>& text, size_t& len, const char& key);
        MyArray<char, displayableText + 1> displayText(const MyArray<char, 256>& text, const size_t& from, const size_t& to);
        void displayCursor(const MyArray<char, displayableText + 1>& outText, const size_t& from);
        void decrementBeginEnd(const MyArray<char, 256>& text, size_t& begin, size_t& end);
        void incrementBeginEnd(const MyArray<char, 256>& text, size_t& begin, size_t& end, const size_t& lenText = 0);
        void display(const MyArray<char, 256>& text, const size_t& from, const size_t& to);
        MyArray<char, 512> correctPath(const MyArray<char, 256>& text);
        void addExtension(MyArray<char, 512>& text, size_t& len);
        bool checkWindowClose();
};

#endif // FILEHANDLER_H
