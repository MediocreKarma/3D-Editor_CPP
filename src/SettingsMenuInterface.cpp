#include "../include/SettingsMenuInterface.h"
#include<iostream>
#include"../include/AppTools.h"
using namespace SettingsMenuInterface;


void SettingsMenuInterface::drawScreen() {
    setfillstyle(SOLID_FILL, BACKGROUND_COLOR);
    bar(0, 0, getmaxx(), getmaxy());
}

void SettingsMenuInterface::setTextSettings() {
    settextstyle(FONT, 0, FONT_SIZE);

}
template<class T>void SettingsMenuInterface::checkClick(MyArray<T,16> arr) {

    if (ismouseclick(WM_LBUTTONDOWN))
        {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            for (size_t i = 0; i < arr.size(); i++) {
                /*if (arr[i].isClicked(x,y)) {
                    std::cout << i << " " << x << " " << y<<"\n";*/
                    //tre sa vad cum fac cu isClicked ca tu ai schimbat in collision am vazut.
                    //idee: vreau sa fac templateul sa verifice daca i array de Dropdowns
                    //si da, cred ca am sa fac un array pentru singurul dropdown ever
                    //sau... pot sa fac pentru un singur element? dar daca tot ii template facut pt arrayuri...
                    //plus poate mai adaugam setari
                    //iar collisionul dropdownului sa returneze int (indexul elementului clickuit)
                    //si nu doar bool
                    //ca sa schimbam doar un parametru cu care vom apela o functie
                    //eu asa consider
                }

        }


}

void SettingsMenuInterface::run() {
    int height = getmaxheight(), width = getmaxwidth();
    initwindow(SETTINGS_WIDTH, SETTINGS_HEIGHT, SETTINGS_TITLE,width/2 - SETTINGS_WIDTH/2, height/2 - SETTINGS_HEIGHT/2);

    drawScreen();
    setTextSettings();

    MyArray<TextLabel, 3> labels;
    MyArray<TextButton,3> rectButtons;
    MyArray<char, 128> text = "Tema / Theme";
    const char* p = text.data();
    labels[0] = TextLabel(100, 50, 140, 50, LABEL_COLOR, p);
    labels[0].drawTextLabel(FONT,FONT_SIZE);
    text = "Limba/Language";
    labels[1] = TextLabel(100, 250, 140, 50, LABEL_COLOR, p);
    labels[1].drawTextLabel(FONT,FONT_SIZE);



    text = "Dark";
    rectButtons[0] = TextButton(500, 50, 140, 50,RGB(80,80,80),p);
    text = "Light";
    rectButtons[1] = TextButton(350,50,140,50,RGB(180,180,180),p);
    text = "Start";
    rectButtons[2] = TextButton(300,350,140,50,RGB(180,180,180),p);

    for(size_t i = 0; i < rectButtons.size(); i++){
        rectButtons[i].drawTextButton(FONT,FONT_SIZE);
    }


    text = "./media/RO.jpg";
    ImageButton im = ImageButton(350,250,140,50,0,p);
    im.drawImageButton();
    text = "./media/EN.jpg";
    ImageButton im2 = ImageButton(500,250,140,50,0,p);
    im2.drawImageButton();


    do {
        //daca ai idei mai bune pentru un click checker decat un template apelat pt toate arrayurile, sa mi zici
    } while (true);
}
