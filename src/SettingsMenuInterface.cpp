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

MyArray<TextLabel, LABEL_SIZE> SettingsMenuInterface::initLabels() {
    MyArray<TextLabel, LABEL_SIZE> labels = {
        TextLabel(100, 50, 140, 50, LABEL_COLOR, "Tema / Theme"),
        TextLabel(100, 250, 140, 50, LABEL_COLOR, "Limba / Language")
    };
    return labels;
}

MyArray<TextButton, TEXTBUTTON_SIZE> SettingsMenuInterface::initThemeButtons() {
    MyArray<TextButton, TEXTBUTTON_SIZE> themeButtons = {
        TextButton(500, 50, 140, 50, RGB(80, 80, 80), "Dark"),
        TextButton(350, 50, 140, 50, RGB(180, 180, 180), "Light")
    };
    return themeButtons;
}

MyArray<ImageButton, FLAG_SIZE> SettingsMenuInterface::initImageButtons() {
    MyArray<ImageButton, FLAG_SIZE> flagButtons = {
        ImageButton(350, 250, 140, 50, 0, "./media/RO.jpg"),
        ImageButton(500, 250, 140, 50, 0, "./media/EN.jpg")
    };
    return flagButtons;
}

DropdownButton<DROPDOWN_SIZE> SettingsMenuInterface::buildDropdownButton() {
    DropdownButton<DROPDOWN_SIZE> ddButton(300, 100, 100, 50, LIGHTGRAY, "1280x720", 200);
    ddButton.addOption("1920x1080", WHITE);
    ddButton.addOption("1600x900", WHITE);
    ddButton.addOption("1280x720", WHITE);
    ddButton.addOption("1000x750", WHITE);
    ddButton.addOption("800x600", WHITE);
    return ddButton;
}

void SettingsMenuInterface::drawLabels(MyArray<TextLabel, LABEL_SIZE>& labels) {
    for (size_t i = 0; i < labels.size(); ++i) {
        labels[i].drawTextLabel(FONT, FONT_SIZE);
    }
}

void SettingsMenuInterface::drawThemeButtons(MyArray<TextButton, TEXTBUTTON_SIZE>& themeButtons) {
    for(size_t i = 0; i < themeButtons.size(); ++i){
        themeButtons[i].drawTextButton(FONT,FONT_SIZE);
    }
}

void SettingsMenuInterface::drawFlagButtons(MyArray<ImageButton, FLAG_SIZE>& flagButtons) {
    for (size_t i = 0; i < flagButtons.size(); ++i) {
        flagButtons[i].drawImageButton();
    }
}

void SettingsMenuInterface::run() {
    int height = getmaxheight(), width = getmaxwidth();
    initwindow(SETTINGS_WIDTH, SETTINGS_HEIGHT, "Settings", width / 2 - SETTINGS_WIDTH / 2, height / 2 - SETTINGS_HEIGHT / 2);
    drawScreen();
    setTextSettings();
    MyArray<TextLabel, LABEL_SIZE> labels(initLabels());
    MyArray<TextButton, TEXTBUTTON_SIZE> themeButtons(initThemeButtons());
    MyArray<ImageButton, FLAG_SIZE> flagButtons(initImageButtons());
    TextButton startButton(300, 350, 140, 50, RGB(180, 180, 180), "Start");
    DropdownButton<DROPDOWN_SIZE> ddButton(buildDropdownButton());
    drawLabels(labels);
    settingsMenu(themeButtons, flagButtons, startButton, ddButton);
}

bool SettingsMenuInterface::settingsMenu(MyArray<TextButton, TEXTBUTTON_SIZE>& themeButtons, MyArray<ImageButton, FLAG_SIZE>& flagButtons,
                                         TextButton& startButton,  DropdownButton<DROPDOWN_SIZE>& ddButton) {
    drawThemeButtons(themeButtons);
    drawFlagButtons(flagButtons);
    startButton.drawTextButton(FONT, FONT_SIZE);
    ddButton.drawTextButton(FONT, FONT_SIZE);
    while (true) {
        while (!ismouseclick(WM_LBUTTONDOWN));
        int x, y;
        getmouseclick(WM_LBUTTONDOWN, x, y);
        if (startButton.hitCollision(x, y)) {
            closegraph();
            return 1;
            // run app instance with parameters resX, resY, theme,
        }
        for (size_t i = 0; i < themeButtons.size(); ++i) {
            if (themeButtons[i].hitCollision(x, y)) {
                themeButtons[i].border(RED);
                theme = i;
                for (size_t j = 0; j < themeButtons.size(); ++j) {
                    if (i == j) {
                        continue;
                    }
                    themeButtons[j].border(BLACK);
                }
            }
        }
        for (size_t i = 0; i < flagButtons.size(); ++i) {
            if (flagButtons[i].hitCollision(x, y)) {
                flagButtons[i].border(RED);
                language = i;
                for (size_t j = 0; j < flagButtons.size(); ++j) {
                    if (i == j) {
                        continue;
                    }
                    flagButtons[j].border(BLACK);
                }
            }
        }
        if (ddButton.hitCollision(x, y)) {
            ddButton.toggleVisibillity(BACKGROUND_COLOR, FONT, FONT_SIZE);
        }
        if (ddButton.isListVisible()) {
            for (size_t i = 0; i < DROPDOWN_SIZE; ++i) {
                // handle resolution picked
                // change button display text
            }
        }
    }
}
