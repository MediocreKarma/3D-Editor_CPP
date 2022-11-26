#include "AppTools.h"
#include<iostream>
Label::Label() :
    xCenter(), yCenter(), xLen(), yLen(), fillColor() {}

Label::Label(const int& xCenter_, const int& yCenter_, const int& xLen_, const int& yLen_, const int& fillColor_) :
    xCenter(xCenter_), yCenter(yCenter_), xLen(xLen_), yLen(yLen_), fillColor(fillColor_){}

void Label::drawLabel(const int& outlineColor) const {
    border(outlineColor);
    if (fillColor >= 0) {
        setfillstyle(SOLID_FILL, fillColor);
        bar(xCenter - xLen / 2 + 1, yCenter - yLen / 2 + 1,
            xCenter + xLen / 2, yCenter + yLen / 2);
    }
}

void Label::border(const int& outlineColor) const {
    setcolor(outlineColor);
    rectangle(xCenter - xLen / 2, yCenter - yLen / 2,
        xCenter + xLen / 2, yCenter + yLen / 2);
}

void Label::clear(const int& barColor) const {
    setfillstyle(SOLID_FILL, barColor);
    bar(xCenter - xLen / 2, yCenter - yLen / 2,
        xCenter + xLen / 2, yCenter + yLen / 2);
}

TextLabel::TextLabel() :
    Label(), m_text() {}

TextLabel::TextLabel(const int& xCenter_, const int& yCenter_, const int& xLen_, const int& yLen_, const int& fillColor_, const char* text) :
    Label(xCenter_, yCenter_, xLen_, yLen_, fillColor_), m_text(text){}

void TextLabel::drawText(const int& txtFont, const int& txtSize) {
    settextstyle(txtFont, 0, txtSize);
    setbkcolor(fillColor);
    outtextxy(xCenter - textwidth(m_text.data()) / 2, yCenter - textheight(m_text.data()) / 2, m_text.data());
}

void TextLabel::drawTextLabel(const int& txtFont, const int& txtSize) {
    drawLabel();
    drawText(txtFont, txtSize);
}

Button::Button() = default;

Button::Button(const int& xCenter_, const int& yCenter_, const int& xLen_, const int& yLen_, const int& fillColor_) :
    Label(xCenter_, yCenter_, xLen_, yLen_, fillColor_) {}

bool Button::hitCollision(const int& x, const int& y) const {
    return xCenter - xLen / 2 <= x && x <= xCenter + xLen / 2 &&
        yCenter - yLen / 2 <= y && y <= yCenter + yLen / 2;
}

ImageButton::ImageButton() :
    Button(), filename() {}

ImageButton::ImageButton(const int& xCenter_, const int& yCenter_, const int& xLen_, const int& yLen_, const int& fC_, const char* filename_) :
    Button(xCenter_, yCenter_, xLen_, yLen_, fC_), filename(filename_) {}

void ImageButton::drawImageButton() {
    drawLabel();
    readimagefile(filename.data(), xCenter - xLen / 2, yCenter - yLen / 2, xCenter + xLen / 2 - 1, yCenter + yLen / 2 - 1);
}

TextButton::TextButton() :
    Button(), m_text() {}

TextButton::TextButton(const int& xCenter_, const int& yCenter_, const int& xLen_, const int& yLen_,
    const int& fillColor_, const char* p) :
    Button(xCenter_, yCenter_, xLen_, yLen_, fillColor_), m_text(p) {}

void TextButton::drawText(const int& txtFont, const int& txtSize) {
    settextstyle(txtFont, 0, txtSize);
    setbkcolor(fillColor);
    outtextxy(xCenter - textwidth(m_text.data()) / 2, yCenter - textheight(m_text.data()) / 2, m_text.data());
}

void TextButton::drawTextButton(const int& txtFont, const int& txtSize) {
    drawLabel();
    drawText(txtFont, txtSize);
}
