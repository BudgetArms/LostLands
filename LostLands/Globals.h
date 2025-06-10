#pragma once
#include <string>
#include <iostream>

//#include "utils.h"


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

