#include "AppTools.h"
#include <Math.h>
#include <iostream>
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

Arrow::Arrow() :
    x0(-100), y0(-100), x1(-100), y1(-100), m_thickness(1) {}

Arrow::Arrow(const int& x0_, const int& y0_, const int& x1_, const int& y1_, const int& thickness) :
    x0(x0_), y0(y0_), x1(x1_), y1(y1_), m_thickness(thickness) {}

void Arrow::drawArrow(const int& color) const {
    //vezi tu cum modifici thicknessu ca mie nu mi iese nici de al dracu

    /*struct linesettingstype lineinfo;
    getlinesettings(&lineinfo);
    int oldThickness = lineinfo.thickness;*/
    //setlinestyle(0, 0, m_thickness);

    setcolor(color);
    line(x0, y0, x1, y1);
    line(x0, y0, x1, y1);
    const int arrLen = 7;
    double theta = atan2(y1 - y0, x1 - x0);
    const double phi_A = 0.785398163397448;
    int xA = x1 - arrLen * cos(theta + phi_A);
    int yA = y1 - arrLen * sin(theta + phi_A);
    const double phi_B = -0.785398163397448;
    int xB = x1 - arrLen * cos(theta + phi_B);
    int yB = y1 - arrLen * sin(theta + phi_B);
    line(xA, yA, x1, y1);
    line(xB, yB, x1, y1);

    //setlinestyle(lineinfo.linestyle, lineinfo.upattern, lineinfo.thickness);
}

AxisButton::AxisButton() :
    Arrow(), CircularLabel() {}

AxisButton::AxisButton(const int& x1_, const int& y1_, const int& x2_, const int& y2_, const int& thickness, const int& circleRadius) :
    Arrow(x1_,y1_,x2_,y2_, thickness), CircularLabel(x2_,y2_,circleRadius) {}

bool AxisButton::hitCollision(const int& x, const int& y) const{
    int xDist = x - xCenter;
    int yDist = y - yCenter;
    if(xDist * xDist + yDist * yDist <= radius * radius){
        return true;
    }
    //Otherwise, cineva wise a spus pe internet:
    //"The cross product divided by the length of the line gives
    //the distance of the point from the line."
    //si chiar are dreptate.
    //Trebuie ca distanta de la unde am dat click la unde e defapt linia
    //sa fie sub thickness/2.
    //Primul capat al liniei va fi acoperit de butonul nostru ros de drag and drop
    //Al doilea va fi acoperit de cercul pe care l punem in varf
    //Deci nici nu ne intereseaza acuratetea in capete

    double dxL = x1 - x0, dyL = y1 - y0;
    double dxP = x - x0, dyP = y - y0;

    double squareLineLength = dxL * dxL + dyL * dyL;
    double dotProd = dxP * dxL + dyP * dyL;
    double crossProduct = dyP * dxL - dxP * dyL;

    // distanta efectiva de la punct la linie
    double distance = abs(crossProduct) / sqrt(squareLineLength);

    return (distance <= m_thickness/2 && dotProd >= 0 && dotProd <= squareLineLength);
}

void AxisButton::drawButton(const int& color) {
    drawArrow(color);
    drawLabel(color, color);
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
