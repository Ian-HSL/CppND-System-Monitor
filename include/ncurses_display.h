#ifndef NCURSES_DISPLAY_H
#define NCURSES_DISPLAY_H

#include <curses.h>

#include "process.h"
#include "system.h"

namespace NCursesDisplay {
void Display(System& system, int n = 10);
void DisplaySystem(System& system, WINDOW* window);
void DisplayProcesses(std::vector<Process>& processes, WINDOW* window, int n);
void SortProcesses();
std::string ProgressBar(double percent);
};  // namespace NCursesDisplay

#endif