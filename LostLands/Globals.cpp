#include "pch.h"
#include "Globals.h"

#include "utils.h"

Point2f g_StartPosition{ 200, 300 };

Rectf g_Window{ 0.f, 0.f, 846.f, 500.f };
float g_WindowOffset{ 30.f };
Rectf g_SmallWindow
{
   g_Window.left + g_WindowOffset,
   g_Window.bottom + g_WindowOffset,
   g_Window.width - 2 * g_WindowOffset,
   g_Window.height - 2 * g_WindowOffset
};

Color4f g_HitBoxesColor{ 0, 1, 0, 1 };
bool g_bShowHitboxes{ false };


