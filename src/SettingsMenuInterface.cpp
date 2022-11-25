#include "SettingsMenuInterface.h"
#include<iostream>
#include "graphics.h"
#include "winbgim.h"

using namespace SettingsMenuInterface;

RectangleButton::RectangleButton() = default;

RectangleButton::RectangleButton(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen) :
    m_xCenter(xCenter), m_yCenter(yCenter), m_xLen(xLen), m_yLen(yLen) {}

void RectangleButton::drawButton(const int& color = BLUE) {
    setfillstyle(SOLID_FILL, color);
    bar(m_xCenter - m_xLen / 2, m_yCenter - m_yLen / 2,
              m_xCenter + m_xLen / 2, m_yCenter + m_yLen / 2);
}

bool RectangleButton::isClicked(const int& x, const int& y) const {
    return m_xCenter - m_xLen / 2 <= x && m_xCenter + m_xLen / 2 >= x &&
            m_yCenter - m_yLen / 2 <= y && m_yCenter + m_yLen / 2 >= y;
}
TextButton::TextButton() = default;

TextButton::TextButton(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const MyArray<char,256>& text):
    RectangleButton(xCenter,yCenter,xLen,yLen), m_text(text){}

void TextButton::drawTextButton() {
    drawButton(BUTTON_COLOR);
    setbkcolor(BUTTON_COLOR);
    setcolor(TEXT_COLOR);
    outtextxy(m_xCenter, m_yCenter,m_text.data());
}

TextLabel::TextLabel() = default;
TextLabel::TextLabel(const int& xCenter, const int& yCenter, const int& xLen, const int& yLen, const MyArray<char, 256>& text):
    m_xCenter(xCenter), m_yCenter(yCenter), m_xLen(xLen), m_yLen(yLen), m_text(text){}

void TextLabel::drawTextLabel() {
    setfillstyle(SOLID_FILL, LABEL_COLOR);
    bar(m_xCenter - m_xLen / 2, m_yCenter - m_yLen / 2,
        m_xCenter + m_xLen / 2, m_yCenter + m_yLen / 2);

    setbkcolor(LABEL_COLOR);
    setcolor(TEXT_COLOR);
    outtextxy(m_xCenter, m_yCenter, m_text.data());

}

void SettingsMenuInterface::drawScreen() {
    setfillstyle(SOLID_FILL, BACKGROUND_COLOR);
    bar(0, 0, getmaxx(), getmaxy());

}

void SettingsMenuInterface::setTextSettings() {
    settextstyle(0, 0, 2);
    settextjustify(1, 1);
}

void SettingsMenuInterface::checkClick() {
    /*
    if (ismouseclick(WM_LBUTTONDOWN))
        {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            for (size_t i = 0; i < rectButtons.size(); i++) {
                if (rectButtons[i].isClicked(x,y)) {
                    std::cout << i << " " << x << " " << y<<"\n";
                }
            }
        }
    */
    //o mut aici pentru referinta
    //dar gen
    //creca trebe oricum sa fie un template function
    //DACA as putea sa fac myarray<myarrayuri> cu tipuri diferite, poate ar fi mai ok idk
    // totusi cred ca o sa hardcodam un picut clasele astea cu textButton si asa
    // pentru ca as vrea sa fie undeva o functie apelata care in functie de indexul elementului
    // apeleaza o alta functie (ce ziceai tu cu ifuri si elseuri)
}

void SettingsMenuInterface::run() {
    int height = getmaxheight(), width = getmaxwidth();
    initwindow(SETTINGS_WIDTH, SETTINGS_HEIGHT, SETTINGS_TITLE,width/2 - SETTINGS_WIDTH/2, height/2 - SETTINGS_HEIGHT/2);
    
    drawScreen();
    setTextSettings();
    //drawButtons();

    MyArray<TextLabel, 3> labels;
    MyArray<TextButton,3> rectButtons;
    labels[0] = TextLabel(100, 100, 120, 40, "Label!");
    labels[0].drawTextLabel();

    rectButtons[0] = TextButton(500, 100, 120, 40,"Buton!");
    rectButtons[0].drawTextButton();

    do {
        //sincer habar nu am cum o sa fac o singura functie
        // care mi face check la TOATE TIPURILE de butoane
        // daca ai tu vreo idee....
        // gen, da, as putea sa fac un template function
        // si sa mi l apelez de 3 ori pt cele 3 tipuri de butoane
        // dar oare e eficient?
        // daca ai o idee, sa mi zici
        //checkHover(rectButtons);
        //checkMouse(rectButtons);
    } while (true);
}
