//#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H

//#include "utils.h"

#define DEBUG_MESSAGE 1

#include <string.h>
#include <string>

#include <iostream>

enum class Shape
{
    Triangle,
    Square,
    Pentagon,
    Hexagon,
    Heptagon,
    Circle
};



extern Point2f g_StartPosition;

extern Rectf g_Window;
extern float g_WindowOffset;
extern Rectf g_SmallWindow;

extern Color4f g_HitBoxesColor;
extern bool g_bShowHitboxes;


#endif // GLOBALS_H