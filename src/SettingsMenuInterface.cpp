#include "SettingsMenuInterface.h"

static int resolution = 2;
static int theme = 0;
static int language = 0;

void SettingsMenuInterface::drawScreen() {
    setfillstyle(SOLID_FILL, BACKGROUND_COLOR);
    bar(0, 0, getmaxx(), getmaxy());
}

void SettingsMenuInterface::setTextSettings() {
    settextstyle(FONT, 0, FONT_SIZE);
}

MyArray<TextLabel, SettingsMenuInterface::LABEL_SIZE> SettingsMenuInterface::initLabels() {
    MyArray<TextLabel, SettingsMenuInterface::LABEL_SIZE> labels = {
        TextLabel(100, 50, 160, 50, "Tema / Theme"),
        TextLabel(100, 110, 160, 50, "Rezolutie / Resolution"),
        TextLabel(100, 290, 160, 50, "Limba / Language")
    };
    return labels;
}

MyArray<TextButton, SettingsMenuInterface::TEXTBUTTON_SIZE> SettingsMenuInterface::initThemeButtons() {
    MyArray<TextButton, SettingsMenuInterface::TEXTBUTTON_SIZE> themeButtons = {
        TextButton(350, 50, 140, 50, "Light"),
        TextButton(500, 50, 140, 50, "Dark")
    };
    return themeButtons;
}

MyArray<ImageButton, SettingsMenuInterface::FLAG_SIZE> SettingsMenuInterface::initImageButtons() {
    MyArray<ImageButton, SettingsMenuInterface::FLAG_SIZE> flagButtons = {
        ImageButton(350, 290, 140, 50, "./media/RO.jpg"),
        ImageButton(500, 290, 140, 50, "./media/EN.jpg")
    };
    return flagButtons;
}

DropdownButton<SettingsMenuInterface::DROPDOWN_SIZE> SettingsMenuInterface::buildDropdownButton() {
    DropdownButton<SettingsMenuInterface::DROPDOWN_SIZE> ddButton(425, 110, 140, 50, "1280x720", 120);
    ddButton.addOption("1920x1080");
    ddButton.addOption("1600x900");
    ddButton.addOption("1280x720");
    ddButton.addOption("1000x750");
    ddButton.addOption("800x600");
    return ddButton;
}

void SettingsMenuInterface::drawLabels(MyArray<TextLabel, LABEL_SIZE>& labels) {
    for (size_t i = 0; i < labels.size(); ++i) {
        labels[i].drawTextLabel(FONT, FONT_SIZE, LABEL_COLOR);
    }
}

void SettingsMenuInterface::drawThemeButtons(MyArray<TextButton, TEXTBUTTON_SIZE>& themeButtons) {
    for(size_t i = 0; i < themeButtons.size(); ++i){
        themeButtons[i].drawTextButton(FONT, FONT_SIZE, themeFillColors[i]);
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
    TextButton startButton(300, 350, 100, 50, "Start");
    DropdownButton<DROPDOWN_SIZE> ddButton(buildDropdownButton());
    drawLabels(labels);
    settingsMenu(themeButtons, flagButtons, startButton, ddButton);
}

void SettingsMenuInterface::settingsMenu(MyArray<TextButton, TEXTBUTTON_SIZE>& themeButtons, MyArray<ImageButton, FLAG_SIZE>& flagButtons,
                                         TextButton& startButton,  DropdownButton<DROPDOWN_SIZE>& ddButton) {
    drawThemeButtons(themeButtons);
    drawFlagButtons(flagButtons);
    startButton.drawTextButton(FONT, FONT_SIZE, BUTTON_COLOR);
    ddButton.drawTextButton(FONT, FONT_SIZE, BUTTON_COLOR);
    themeButtons[theme].border(HIGHLIGHT_COLOR);
    flagButtons[language].border(HIGHLIGHT_COLOR);
    while (true) {
        while (!ismouseclick(WM_LBUTTONDOWN));
        int x, y;
        getmouseclick(WM_LBUTTONDOWN, x, y);
        if (startButton.hitCollision(x, y)) {
            closegraph();
            AppInterface appHandler(resOptions[resolution][0], resOptions[resolution][1], theme, language);
            appHandler.run();
            return;
        }
        for (size_t i = 0; i < themeButtons.size(); ++i) {
            if (themeButtons[i].hitCollision(x, y)) {
                themeButtons[i].border(HIGHLIGHT_COLOR);
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
                flagButtons[i].border(HIGHLIGHT_COLOR);
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
            ddButton.toggleVisibillity(BACKGROUND_COLOR, FONT, FONT_SIZE, DROPLIST_COLOR);
        }
        if (ddButton.isListVisible()) {
            const int ddCollide = ddButton.listHitCollision(x,y);
            if(ddCollide >= 0) {
                ddButton.changeMain(ddCollide, FONT, FONT_SIZE, BUTTON_COLOR);
                ddButton.toggleVisibillity(BACKGROUND_COLOR, FONT, FONT_SIZE, DROPLIST_COLOR);
                resolution = ddCollide;
                ddButton.border(HIGHLIGHT_COLOR);
            }
        }
    }
}
