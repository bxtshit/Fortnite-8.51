#include <Windows.h>
#include <string>

struct
{
#define Font_CALIBRI			(1 << 3)

#define Font_ARIAL			(1 << 4)

#define Font_COURIER			(1 << 5)

#define Font_GABRIOLA			(1 << 6)

#define Font_IMPACT			(1 << 7)
}fonts;

#define FOREGROUND	(1 << 0)

void DirectOverlaySetOption(DWORD option);

typedef void(*DirectOverlayCallback)(int width, int height);

void Setup(DirectOverlayCallback callbackFunction, HWND targetWindow);

void DrawLine(float x1, float y1, float x2, float y2, float thickness, float r, float g, float b, float a = 1);

void DrawBox(float x, float y, float width, float height, float thickness, float r, float g, float b, float a, bool filled);

void DrawCircle(float x, float y, float radius, float thickness, float r, float g, float b, float a, bool filled);

void DrawEllipse(float x, float y, float width, float height, float thickness, float r, float g, float b, float a, bool filled);

void DrawString(std::string str, float fontSize, float x, float y, float r, float g, float b, float a = 1);