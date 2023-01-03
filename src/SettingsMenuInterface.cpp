#include "SettingsMenuInterface.h"

static int resolution = 2;
static int theme = 0;
static int language = 0;
static int accentColor = 0;

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
        TextLabel(100, 100, 160, 30, "Rezolutie / Resolution"),
        TextLabel(430, 100, 300, 30, "Culoare de accent / Accent color"),
        TextLabel(100, 290, 160, 50, "Limba / Language")
    };
    return labels;
}

MyArray<TextButton, SettingsMenuInterface::TEXTBUTTON_SIZE> SettingsMenuInterface::initThemeButtons() {
    MyArray<TextButton, SettingsMenuInterface::TEXTBUTTON_SIZE> themeButtons = {
        TextButton(280 + 140 / 2, 50, 140, 50, "Light"),
        TextButton(280  + 20 + 3 * 140 / 2, 50, 140, 50, "Dark")
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
    DropdownButton<SettingsMenuInterface::DROPDOWN_SIZE> ddButton(100, 130, 100, 24, "", 100, 110);
    ddButton.addOption("1920x1080");
    ddButton.addOption("1600x900");
    ddButton.addOption("1280x720");
    ddButton.addOption("1000x750");
    ddButton.addOption("800x600");
    return ddButton;
}

DropdownButton<SettingsMenuInterface::DROPDOWN_SIZE> SettingsMenuInterface::buildAccentColorDropdownButton() {
    DropdownButton<SettingsMenuInterface::DROPDOWN_SIZE> accentButton(430, 130, 160, 24, "", 160, 110);
    accentButton.addOption("Rosu / Red");
    accentButton.addOption("Albastru / Blue");
    accentButton.addOption("Roz / Pink");
    accentButton.addOption("Galben / Yellow");
    accentButton.addOption("Verde / Green");
    return accentButton;
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

void SettingsMenuInterface::checkSavedSettings() {
    FILE* fp = fopen("Settings.ini", "r");
    int resRead = 2, themeRead = 0, langRead = 0;
    if (fscanf(fp, "Resolution: %i\nTheme: %i\nLanguage: %i", &resRead, &themeRead, &langRead) == 3) {
        resolution = resRead;
        theme = themeRead / ACCENT_COLOR_SIZE;
        accentColor = themeRead % ACCENT_COLOR_SIZE;
        language = langRead;
    }
    fclose(fp);
}

void SettingsMenuInterface::saveSelection() {
    FILE* fp = fopen("Settings.ini", "w");
    fprintf(fp, "Resolution: %i\nTheme: %i\nLanguage: %i", resolution, ACCENT_COLOR_SIZE * theme + accentColor, language);
    fclose(fp);
}

void SettingsMenuInterface::run() {
    MyArray<TextLabel, LABEL_SIZE> labels(initLabels());
    MyArray<TextButton, TEXTBUTTON_SIZE> themeButtons(initThemeButtons());
    MyArray<ImageButton, FLAG_SIZE> flagButtons(initImageButtons());
    TextButton startButton(300, 350, 100, 50, "Start");
    DropdownButton<DROPDOWN_SIZE> ddButton(buildDropdownButton());
    DropdownButton<DROPDOWN_SIZE> accentButton(buildAccentColorDropdownButton());
    settingsMenu(themeButtons, flagButtons, startButton, ddButton, accentButton, labels);
}

void SettingsMenuInterface::initScreen() {
    int height = getmaxheight(), width = getmaxwidth();
    initwindow(SETTINGS_WIDTH, SETTINGS_HEIGHT, "Settings", width / 2 - SETTINGS_WIDTH / 2, height / 2 - SETTINGS_HEIGHT / 2);
    drawScreen();
    setTextSettings();
    checkSavedSettings();
}

void SettingsMenuInterface::draw(MyArray<TextButton, TEXTBUTTON_SIZE>& themeButtons, MyArray<ImageButton, FLAG_SIZE>& flagButtons,
                                    TextButton& startButton,  DropdownButton<DROPDOWN_SIZE>& ddButton,
                                    DropdownButton<DROPDOWN_SIZE>& accentButton, MyArray<TextLabel, LABEL_SIZE>& labels) {
    drawThemeButtons(themeButtons);
    drawFlagButtons(flagButtons);
    drawLabels(labels);
    startButton.drawTextButton(FONT, FONT_SIZE, BUTTON_COLOR);
    ddButton.drawTextButton(FONT, FONT_SIZE, BUTTON_COLOR);
    accentButton.drawTextButton(FONT, FONT_SIZE, BUTTON_COLOR);
    themeButtons[theme].border(HIGHLIGHT_COLOR);
    flagButtons[language].border(HIGHLIGHT_COLOR);
    ddButton.changeMain(resolution, FONT, FONT_SIZE, BUTTON_COLOR);
    accentButton.changeMain(accentColor, FONT, FONT_SIZE, BUTTON_COLOR);
}

void SettingsMenuInterface::settingsMenu(MyArray<TextButton, TEXTBUTTON_SIZE>& themeButtons, MyArray<ImageButton, FLAG_SIZE>& flagButtons,
                                         TextButton& startButton, DropdownButton<DROPDOWN_SIZE>& ddButton, DropdownButton<DROPDOWN_SIZE>& accentButton, MyArray<TextLabel, LABEL_SIZE>& labels) {
    initScreen();
    draw(themeButtons, flagButtons, startButton, ddButton, accentButton, labels);
    AppInterface appHandler;
    while (true) {
        while (!ismouseclick(WM_LBUTTONDOWN));
        int x, y;
        getmouseclick(WM_LBUTTONDOWN, x, y);
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
            }
        }
        if (accentButton.hitCollision(x, y)) {
            accentButton.toggleVisibillity(BACKGROUND_COLOR, FONT, FONT_SIZE, DROPLIST_COLOR);
        }
        if (accentButton.isListVisible()) {
            const int accentCollide = accentButton.listHitCollision(x,y);
            if(accentCollide >= 0) {
                accentButton.changeMain(accentCollide, FONT, FONT_SIZE, BUTTON_COLOR);
                accentButton.toggleVisibillity(BACKGROUND_COLOR, FONT, FONT_SIZE, DROPLIST_COLOR);
                accentColor = accentCollide;
            }
        }
        if (startButton.hitCollision(x, y)) {
            closegraph();
            saveSelection();
            appHandler.setSettings(resOptions[resolution][0], resOptions[resolution][1], theme * ACCENT_COLOR_SIZE + accentColor, language);
            appHandler.run();
            initScreen();
            draw(themeButtons, flagButtons, startButton, ddButton, accentButton, labels);
        }
    }
}
