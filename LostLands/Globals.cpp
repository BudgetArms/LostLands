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

std::string WStringToString(const std::wstring& wstr)
{
    std::string utf8;
    for (wchar_t wc : wstr) {
        uint32_t c = static_cast<uint32_t>(wc);

        if (c <= 0x7F) {
            utf8.push_back(static_cast<char>(c));
        }
        else if (c <= 0x7FF) {
            utf8.push_back(static_cast<char>(0xC0 | ((c >> 6) & 0x1F)));
            utf8.push_back(static_cast<char>(0x80 | (c & 0x3F)));
        }
        else if (c <= 0xFFFF) {
            utf8.push_back(static_cast<char>(0xE0 | ((c >> 12) & 0x0F)));
            utf8.push_back(static_cast<char>(0x80 | ((c >> 6) & 0x3F)));
            utf8.push_back(static_cast<char>(0x80 | (c & 0x3F)));
        }
        else {
            utf8.push_back(static_cast<char>(0xF0 | ((c >> 18) & 0x07)));
            utf8.push_back(static_cast<char>(0x80 | ((c >> 12) & 0x3F)));
            utf8.push_back(static_cast<char>(0x80 | ((c >> 6) & 0x3F)));
            utf8.push_back(static_cast<char>(0x80 | (c & 0x3F)));
        }
    }
    return utf8;
}


