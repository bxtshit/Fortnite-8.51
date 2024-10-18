#include "direct2d.h"

namespace d2d
{
	void FilledRect(float x, float y, float width, float height, float thickness, float r, float g, float b, float a)
	{
		DrawBox(x, y, width, height, thickness, r / 255, g / 255, b / 255, a, true);
	}
	void EmptyRect(float x, float y, float width, float height, float thickness, float r, float g, float b, float a)
	{
		DrawBox(x, y, width, height, thickness, r / 255, g / 255, b / 255, a, false);
	}
	void Line(float x1, float y1, float x2, float y2, float thickness, float r, float g, float b, float a)
	{
		DrawLine(x1, y1, x2, y2, thickness, r / 255, g / 255, b / 255, a);
	}
	void Text(std::string str, float fontSize, float x, float y, float r, float g, float b, float a)
	{
		DrawString(str, fontSize, x, y, r / 255, g / 255, b / 255, a);
	}
	void FilledCircle(float x, float y, float radius, float thickness, float r, float g, float b, float a)
	{
		DrawCircle(x, y, radius, thickness, r / 255, g / 255, b / 255, a, true);
	}
	void EmptyCircle(float x, float y, float radius, float thickness, float r, float g, float b, float a)
	{
		DrawCircle(x, y, radius, thickness, r / 255, g / 255, b / 255, a, false);
	}
}