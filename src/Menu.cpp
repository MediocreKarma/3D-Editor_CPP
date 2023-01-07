#include "Menu.h"
#include "SettingsMenuInterface.h"

Menu::Menu(const int& theme, int language, const int& appWidth, const int& appHeight) :
    x1(), y1(), x2(), y2(), m_appWidth(appWidth), m_appHeight(appHeight), m_theme(theme), m_language(language), m_fileButton(40, 13, 80, 26, Language::Text[(int)Lang::File][m_language].data(), 120, 80),
    m_settingsButton(130, 13, 100, 26, Language::Text[(int)Lang::Settings][m_language].data()), m_helpButton(210, 13, 60, 26, Language::Text[(int)Lang::Help][m_language].data(), 80, 80),
    m_space(-2500, m_theme, m_language, this), m_fileGetter(language), m_settingsMenuFlag(false) {
    m_fileButton.addOption(Language::Text[(int)Lang::New][m_language].data());
    m_fileButton.addOption(Language::Text[(int)Lang::Save][m_language].data());
    m_fileButton.addOption(Language::Text[(int)Lang::Save_As][m_language].data());
    m_fileButton.addOption(Language::Text[(int)Lang::Open][m_language].data());
    initSpace();
    Mesh cube;
    cube.addPoint(-100, -100, -100);
    cube.addPoint(-100, -100, 100);
    cube.addPoint(-100, 100, -100);
    cube.addPoint(100, -100, -100);
    cube.addPoint(-100, 100, 100);
    cube.addPoint(100, -100, 100);
    cube.addPoint(100, 100, -100);
    cube.addPoint(100, 100, 100);
    cube.addIndexConnections(0, {1, 2, 3});
    cube.addIndexConnections(1, {0, 4, 5});
    cube.addIndexConnections(2, {0, 4, 6});
    cube.addIndexConnections(3, {0, 5, 6});
    cube.addIndexConnections(4, {1, 2, 7});
    cube.addIndexConnections(5, {1, 3, 7});
    cube.addIndexConnections(6, {2, 3, 7});
    cube.updateCenterPoint();
    cube.translate(500, 0, 0);
    m_space.addMesh(cube);
    cube.translate(0, 600, 0);
    m_space.addMesh(cube);
    cube.translate(0, 600, 0);
    m_space.addMesh(cube);
    cube.translate(0, 600, 0);
    m_space.addMesh(cube);
    cube.translate(0, 600, 0);
    m_space.addMesh(cube);
}

void Menu::setSettings(const int& theme, int language, const int& appWidth, const int& appHeight) {
    m_theme = theme;
    m_language = language;
    m_fileGetter.setLanguage(language);
    m_fileButton = DropdownButton<4>(40, 13, 80, 26, Language::Text[(int)Lang::File][m_language].data(), 120, 80);
    m_settingsButton = TextButton(130, 13, 100, 26, Language::Text[(int)Lang::Settings][m_language].data());
    m_helpButton = DropdownButton<3>(210, 13, 60, 26, Language::Text[(int)Lang::Help][m_language].data(), 80, 80);
    m_fileButton.addOption(Language::Text[(int)Lang::New][m_language].data());
    m_fileButton.addOption(Language::Text[(int)Lang::Save][m_language].data());
    m_fileButton.addOption(Language::Text[(int)Lang::Save_As][m_language].data());
    m_fileButton.addOption(Language::Text[(int)Lang::Open][m_language].data());
    m_appWidth = appWidth;
    m_appHeight = appHeight;
    m_space.setCorners(0, 27, m_appWidth, m_appHeight);
    m_space.setTheme(theme);
    m_space.setLanguage(language);
    m_space.update();
    m_space.render();
}

void Menu::setBorder(const int& x1_, const int& y1_, const int& x2_, const int& y2_) {
    x1 = x1_;
    y1 = y1_;
    x2 = x2_;
    y2 = y2_;
}

void Menu::saveSpace3D(Space3D& space, const char& saveType) {
    if (space.isLinkedWithFile() && saveType != 2) {
        FILE* fp = fopen(space.getLinkedFileName().data(), "w");
        space.fprint(fp);
        fclose(fp);
        return;
    }
    int getCurrentWindowNumber = getcurrentwindow();
    MyArray<char, 512> savepath = m_fileGetter.userSavePath();
    setcurrentwindow(getCurrentWindowNumber);
    if (!savepath[0]) {
        return;
    }
    FILE* fp = fopen(savepath.data(), "w");
    if (!fp) {
        showerrorbox(m_language ? "Bad file path!" : "Calea fisierului nu este buna!");
    }
    else {
        space.fprint(fp);
        space.setLinkedFileName(savepath);
    }
    fclose(fp);
}

void Menu::openSpace3D(Space3D& space) {
    int getCurrentWindowNumber = getcurrentwindow();
    MyArray<char, 512> openpath = m_fileGetter.userOpenPath();
    setcurrentwindow(getCurrentWindowNumber);
    if (!openpath[0]) {
        return;
    }
    FILE* fp = fopen(openpath.data(), "r");
    Space3D tempSpace(-2500, m_theme, m_language, this);
    tempSpace.setCorners(0, 27, m_appWidth, m_appHeight);
    if (!fp) {
        showerrorbox(m_language ? "File not found!" : "Fisierul nu a fost gasit!");
    }
    else if (!tempSpace.fscan(fp)){
        showerrorbox(m_language ? "Bad file!" : "Fisierul nu este bun!");
        fclose(fp);
    }
    else {
        space = tempSpace;
        space.setLinkedFileName(openpath);
    }
    fclose(fp);
}

void Menu::drawMenu() {
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(0, 0, x1, y1);
    bar(0, 0, x2, y2);
    setlinestyle(SOLID_LINE, 0, 1);
    m_fileButton.drawTextButton(3, 1, ColorSchemes::mixColors(LIGHTGRAY, ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR], 25));
    m_fileButton.border(BLACK);
    m_settingsButton.drawTextButton(3, 1, ColorSchemes::mixColors(RGB(140, 140, 140), ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR], 25));
    m_settingsButton.border(BLACK);
    m_helpButton.drawTextButton(3, 1, ColorSchemes::mixColors(LIGHTGRAY, ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR], 25));
    m_helpButton.border(BLACK);
    if (m_fileButton.isListVisible()) {
        m_fileButton.showList(3, 1, ColorSchemes::mixColors(RGB(210, 210, 210), ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR], 25));
    }
}

void Menu::initSpace() {
    m_space = Space3D(-2500, m_theme, m_language, this);
    m_space.setCorners(0, 27, m_appWidth, m_appHeight);
}

void Menu::draw() {
    m_space.run();
    drawMenu();
    swapbuffers();
}
bool Menu::getCommand(const int& x, const int& y) {
    if (x == -1) {
        return m_space.getKeyCommand();
    }
    if (m_fileButton.isListVisible()) {
        int pos = m_fileButton.listHitCollision(x, y);
        if (pos >= 0) {
            if (pos == 0) {
                initSpace();
            }
            else if (pos == 1 || pos == 2) {
                saveSpace3D(m_space, pos);
            }
            else if (pos == 3) {
                openSpace3D(m_space);
            }
            m_fileButton.setVisibility(false);
            return true;
        }
    }
    if (m_fileButton.hitCollision(x, y)) {
        m_fileButton.setVisibility(!m_fileButton.isListVisible());
        return 1;
    }
    if (m_settingsButton.hitCollision(x, y)) {
        closegraph();
        m_settingsMenuFlag = true;
        return false;
    }
    if (m_helpButton.hitCollision(x, y)) {
        int getCurrentWindowNumber = getcurrentwindow();
        int helpWindow = initwindow(800, 570, Language::Text[(int)Lang::Help][m_language].data(), getmaxwidth() / 2 - 400, getmaxheight() / 2 - 300, false, false);
        setcurrentwindow(helpWindow);
        MyArray<MyArray<char, 32>, 2> helpFilename = {"help/ro.txt", "help/en.txt"};
        FILE *helpf = fopen(helpFilename[m_language].data(), "r");
        MyVector<MyArray<char, 256>> helpText{};
        if (!helpf) {
            closegraph(helpWindow);
            setcurrentwindow(getCurrentWindowNumber);
            return false;
        }
        while(!feof(helpf)) {
            //temporar pt ca mi da warning si nu mi ia empty lines, dar functioneaza
            char buf[512];
            fscanf(helpf, "%[^\n]\n", &buf);
            helpText.push_back(MyArray<char, 256>(buf));
        }
        int lineIndex = 0;
        static const int LINE_WIDTH = 18;
        static const int LINE_MARGIN = 20;
        setfillstyle(SOLID_FILL, ColorSchemes::mixColors(LIGHTGRAY, ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR], 10));
        setbkcolor(ColorSchemes::mixColors(LIGHTGRAY, ColorSchemes::themeColors[m_theme][ColorSchemes::ACCENTCOLOR], 10));
        bar(0, 0, 800, 600);
        setcolor(BLACK);
        for (size_t i = 0; i < helpText.size(); ++i) {
            bool backslash = 0;
            if (helpText[i].data()[0] == '\\') {
                lineIndex++;
                backslash = true;
            }
            outtextxy(LINE_MARGIN, LINE_MARGIN + lineIndex++ * LINE_WIDTH, helpText[i].data() + backslash);
        }
        MyArray<char, 32> backText(m_language ? "Back" : "Inapoi");
        TextButton backButton(45, getmaxy() - 30, 60, 30, backText.data());
        backButton.drawTextButton(0, 0, WHITE);
        while (true) {
            if (kbhit()) {
                char c = getch();
                if (c == 27) {
                    break;
                }
            }
            else if(ismouseclick(WM_LBUTTONDOWN)) {
                int x_, y_;
                getmouseclick(WM_LBUTTONDOWN, x_, y_);
                if (backButton.hitCollision(x_, y_)) {
                    break;
                }
            }
        }
        closegraph(helpWindow);
        setcurrentwindow(getCurrentWindowNumber);
        return false;
    }
    if (m_space.getCommand(x, y)) {
        return true;
    }
    return false;
}

bool Menu::getRightClickCommand(const int& x, const int& y) {
    if (x == -1) {
        return false;
    }
    if (m_space.getRightClickCommand(x, y)) {
        return true;
    }
    return false;
}

bool Menu::returnToSettingsFlag() {
    if (m_settingsMenuFlag) {
        m_settingsMenuFlag = false;
        return true;
    }
    return false;
}
