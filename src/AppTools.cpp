#include "AppTools.h"

Label::Label() :
    xCenter(), yCenter(), xLen(), yLen() {}

Label::Label(const int& xCenter_, const int& yCenter_, const int& xLen_, const int& yLen_) :
    xCenter(xCenter_), yCenter(yCenter_), xLen(xLen_), yLen(yLen_) {}

void Label::drawLabel(const int& fillColor, const int& outlineColor) const {
    border(outlineColor);
    if (fillColor == -1) {
        return;
    }
    setfillstyle(SOLID_FILL, fillColor);
    bar(xCenter - xLen / 2 + 1, yCenter - yLen / 2 + 1, xCenter + xLen / 2, yCenter + yLen / 2);
}

void Label::border(const int& outlineColor) const {
    if (outlineColor == -1)
        return;
    setcolor(outlineColor);
    rectangle(xCenter - xLen / 2, yCenter - yLen / 2,
        xCenter + xLen / 2, yCenter + yLen / 2);
}

void Label::clear(const int& barColor) const {
    setfillstyle(SOLID_FILL, barColor);
    bar(xCenter - xLen / 2, yCenter - yLen / 2,
        xCenter + xLen / 2, yCenter + yLen / 2);
}

int Label::getXCenter() const {
    return xCenter;
}

int Label::getYCenter() const {
    return yCenter;
}

CircularLabel::CircularLabel() :
    xCenter(), yCenter(), radius() {}

CircularLabel::CircularLabel(const int& xCenter_, const int& yCenter_, const int& radius_) :
    xCenter(xCenter_), yCenter(yCenter_), radius(radius_) {}

void CircularLabel::drawLabel(const int& fillColor, const int& outlineColor) {
    border(outlineColor);
    if (fillColor == -1) {
        return;
    }
    setfillstyle(SOLID_FILL, fillColor);
    fillellipse(xCenter, yCenter, radius, radius);
}

void CircularLabel::border(const int& outlineColor) const {
    if (outlineColor == -1) {
        return;
    }
    setcolor(outlineColor);
    circle(xCenter, yCenter, radius);
}

void CircularLabel::clear(const int& barColor) const {
    setfillstyle(SOLID_FILL, barColor);
    ellipse(xCenter, yCenter, 0, 360, radius, radius);
}

void CircularLabel::move(const int& x, const int& y) {
    xCenter = x;
    yCenter = y;
}

int CircularLabel::getX() const {
    return xCenter;
}

int CircularLabel::getY() const {
    return yCenter;
}

int CircularLabel::getRadius() const {
    return radius;
}

CircularButton::CircularButton() :
    CircularLabel() {}

CircularButton::CircularButton(const int& xCenter_, const int& yCenter_, const int& radius_) :
    CircularLabel(xCenter_, yCenter_, radius_) {}

bool CircularButton::hitCollision(const int& x, const int& y) const {
    int xDist = x - xCenter;
    int yDist = y - yCenter;
    return xDist * xDist + yDist * yDist <= radius * radius;
}

DonutButton::DonutButton() :
    CircularButton(), m_outerRing(), m_innerRing() {}

DonutButton::DonutButton(const int& xCenter_, const int& yCenter_, const int& radius_, const int& donutLen) :
    CircularButton(xCenter_, yCenter_, radius_), m_outerRing(xCenter_, yCenter_, radius_ + donutLen / 2), m_innerRing(xCenter_, yCenter_, radius_ - donutLen / 2) {}

bool DonutButton::hitCollision(const int& x, const int& y) const {
    return m_outerRing.hitCollision(x, y) && !m_innerRing.hitCollision(x, y);
}

TextLabel::TextLabel() :
    Label(), m_text() {}

TextLabel::TextLabel(const int& xCenter_, const int& yCenter_, const int& xLen_, const int& yLen_, const char* text) :
    Label(xCenter_, yCenter_, xLen_, yLen_), m_text(text){}

void TextLabel::drawText(const int& txtFont, const int& txtSize, const int& bkColor) {
    settextstyle(txtFont, 0, txtSize);
    setbkcolor(bkColor);
    outtextxy(xCenter - textwidth(m_text.data()) / 2, yCenter - textheight(m_text.data()) / 2, m_text.data());
}

void TextLabel::drawTextLabel(const int& txtFont, const int& txtSize, const int& fillColor) {
    drawLabel(fillColor);
    drawText(txtFont, txtSize, fillColor);
}

Button::Button() = default;

Button::Button(const int& xCenter_, const int& yCenter_, const int& xLen_, const int& yLen_) :
    Label(xCenter_, yCenter_, xLen_, yLen_) {}

bool Button::hitCollision(const int& x, const int& y) const {
    return xCenter - xLen / 2 <= x && x <= xCenter + xLen / 2 &&
        yCenter - yLen / 2 <= y && y <= yCenter + yLen / 2;
}

ImageButton::ImageButton() :
    Button(), filename() {}

ImageButton::ImageButton(const int& xCenter_, const int& yCenter_, const int& xLen_, const int& yLen_, const char* filename_) :
    Button(xCenter_, yCenter_, xLen_, yLen_), filename(filename_) {}

void ImageButton::drawImageButton() {
    drawLabel(0);
    readimagefile(filename.data(), xCenter - xLen / 2 + 1, yCenter - yLen / 2 + 1 , xCenter + xLen / 2 - 1, yCenter + yLen / 2 - 1);
}

TextButton::TextButton() :
    Button(), m_text() {}

TextButton::TextButton(const int& xCenter_, const int& yCenter_, const int& xLen_, const int& yLen_, const char* p) :
    Button(xCenter_, yCenter_, xLen_, yLen_), m_text(p) {}

void TextButton::drawText(const int& txtFont, const int& txtSize, const int& bkColor) {
    settextstyle(txtFont, 0, txtSize);
    setbkcolor(bkColor);
    outtextxy(xCenter - textwidth(m_text.data()) / 2, yCenter - textheight(m_text.data()) / 2, m_text.data());
}

void TextButton::drawTextButton(const int& txtFont, const int& txtSize, const int& fillColor) {
    drawLabel(fillColor);
    drawText(txtFont, txtSize, fillColor);
}

const char* TextButton::getText() {
    return m_text.data();
}
