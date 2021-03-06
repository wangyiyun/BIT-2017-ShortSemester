#pragma once
#include "head.h"

#define COLOR_RED (FOREGROUND_INTENSITY | FOREGROUND_RED)
#define COLOR_GREEN (FOREGROUND_INTENSITY | FOREGROUND_GREEN)
#define COLOR_BLUE (FOREGROUND_INTENSITY | FOREGROUND_BLUE)
#define COLOR_YELLOW (FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN)
#define COLOR_PURPLE (FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE)
#define COLOR_CYAN (FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define COLOR_GREY (FOREGROUND_INTENSITY)
#define COLOR_WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define COLOR_LIGHT (FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)

void setColor(int color);