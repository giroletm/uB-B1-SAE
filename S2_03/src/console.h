#pragma once

#include <stdio.h>
#include <string>

#if defined(WIN32) || defined(_WIN32) /* si vous êtes sous Windows */

#include <windows.h>
#include <conio.h>

#elif defined (linux) /* si vous êtes sous Linux */

#include <sys/ioctl.h>
#include <sys/select.h>
#include <termios.h>
#include <stdarg.h>
#include <unistd.h>

inline void changemode(int dir)
{
  static struct termios oldt, newt;
 
  if ( dir == 1 )
  {
    tcgetattr( STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);
  }
  else
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}

static int peek = -1;
inline int _kbhit() {
    // variables de lecture
    char ch;
    int nread;
    struct termios origTerm, newTerm;

    // vérification d'usage
    if(peek != -1)
        return 1;

    // préparation du terminal
    tcgetattr(0, &origTerm);
    newTerm = origTerm;
    newTerm.c_lflag &= ~ICANON;
    newTerm.c_lflag &= ~ECHO;
    newTerm.c_lflag &= ~ISIG;
    newTerm.c_cc[VMIN] = 1;
    newTerm.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &newTerm);

    // écoute clavier
    newTerm.c_cc[VMIN]=0;
    tcsetattr(0, TCSANOW, &newTerm);
    nread = read(0,&ch,1);
    newTerm.c_cc[VMIN]=1;
    tcsetattr(0, TCSANOW, &newTerm);

    // Rétablissement du terminal
    tcsetattr(0,TCSANOW, &origTerm);

    // test de lecture
    if (nread == 1) {
        peek = ch;
        return 1;
    }
    return 0;
}

inline char _getch() {
    char ch = peek;
    peek = -1;
    return ch;
}

#else /* sinon vous êtes sur une plateforme non supportée */

#error not defined for this platform

#endif


inline void PositionCursor(int line, int col) {
    if(line >= 0 && col >= 0)
        printf("\033[%d;%dH", line, col);
    else if(line >= 0)
        printf("\033[%d;H", line);
    else if(col >= 0)
        printf("\033[;%dG", col);
}

inline void MoveCursorLine(int linesToAdd) {
    if(linesToAdd >= 0)
        printf("\033[%dA", linesToAdd);
    else
        printf("\033[%dB", -linesToAdd);
}

inline void MoveCursorCol(int colsToAdd) {
    if(colsToAdd >= 0)
        printf("\033[%dC", colsToAdd);
    else
        printf("\033[%dD", -colsToAdd);
}

inline void ClearScreen() {
    printf("\033[2J");
}

inline void ClearEndOfLine() {
    printf("\033[K");
}

inline void ClearLine() {
    printf("\033[2K");
}

void fakeCin(std::string &str, bool* interrupt, bool printNewLine=true);