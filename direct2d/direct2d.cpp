#include "direct2d.h"
#include <d2d1.h>
#include <dwrite.h>
#include <dwmapi.h>
#include <fstream>
#include <comdef.h>
#include <iostream>
#include <ctime>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "dwrite.lib")

ID2D1Factory* factory;
ID2D1HwndRenderTarget* target;
ID2D1SolidColorBrush* solid_brush;
IDWriteFactory* w_factory;
IDWriteTextFormat* w_format;
IDWriteTextLayout* w_layout;
HWND overlayWindow;
HINSTANCE appInstance;
HWND targetWindow;
HWND enumWindow = NULL;
time_t preTime = clock();
time_t showTime = clock();

bool o_Foreground = false;
std::wstring fontname = L"Courier";

DirectOverlayCallback drawLoopCallback = NULL;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void DrawString(std::string str, float fontSize, float x, float y, float r, float g, float b, float a)
{
	RECT re;
	GetClientRect(overlayWindow, &re);
	FLOAT dpix, dpiy;
	dpix = static_cast<float>(re.right - re.left);
	dpiy = static_cast<float>(re.bottom - re.top);
	HRESULT res = w_factory->CreateTextLayout(std::wstring(str.begin(), str.end()).c_str(), str.length() + 1, w_format, dpix, dpiy, &w_layout);
	if (SUCCEEDED(res))
	{
		DWRITE_TEXT_RANGE range = { 0, str.length() };
		w_layout->SetFontSize(fontSize, range);
		solid_brush->SetColor(D2D1::ColorF(r, g, b, a));
		target->DrawTextLayout(D2D1::Point2F(x, y), w_layout, solid_brush);
		w_layout->Release();
		w_layout = NULL;
	}
}

void DrawBox(float x, float y, float width, float height, float thickness, float r, float g, float b, float a, bool filled)
{
	solid_brush->SetColor(D2D1::ColorF(r, g, b, a));
	if (filled)  target->FillRectangle(D2D1::RectF(x, y, x + width, y + height), solid_brush);
	else target->DrawRectangle(D2D1::RectF(x, y, x + width, y + height), solid_brush, thickness);
}

void DrawLine(float x1, float y1, float x2, float y2, float thickness, float r, float g, float b, float a) {
	solid_brush->SetColor(D2D1::ColorF(r, g, b, a));
	target->DrawLine(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), solid_brush, thickness);
}

void DrawCircle(float x, float y, float radius, float thickness, float r, float g, float b, float a, bool filled)
{
	solid_brush->SetColor(D2D1::ColorF(r, g, b, a));
	if (filled) target->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), solid_brush);
	else target->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), solid_brush, thickness);
}

void DrawEllipse(float x, float y, float width, float height, float thickness, float r, float g, float b, float a, bool filled)
{
	solid_brush->SetColor(D2D1::ColorF(r, g, b, a));
	if (filled) target->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), width, height), solid_brush);
	else target->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), width, height), solid_brush, thickness);
}

void d2oSetup(HWND tWindow) {
	targetWindow = tWindow;
	WNDCLASS wc = { };
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(0);
	wc.lpszClassName = "direct2d_overlay";
	RegisterClass(&wc);
	overlayWindow = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST,
		wc.lpszClassName, "direct2d", WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, wc.hInstance, NULL);

	MARGINS mar = { -1 };
	DwmExtendFrameIntoClientArea(overlayWindow, &mar);
	D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &factory);
	factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)),
		D2D1::HwndRenderTargetProperties(overlayWindow, D2D1::SizeU(200, 200),
			D2D1_PRESENT_OPTIONS_IMMEDIATELY), &target);
	target->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f), &solid_brush);
	target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&w_factory));
	w_factory->CreateTextFormat(fontname.c_str(), NULL, DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 10.0f, L"en-us", &w_format);
}

void mainLoop() {
	MSG message;
	message.message = WM_NULL;
	SetWindowLong(overlayWindow, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_LAYERED);
	ShowWindow(overlayWindow, SW_SHOW);
	UpdateWindow(overlayWindow);
	SetLayeredWindowAttributes(overlayWindow, RGB(0, 0, 0), 255, LWA_ALPHA);
	if (message.message != WM_QUIT)
	{
		if (PeekMessage(&message, overlayWindow, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		UpdateWindow(overlayWindow);
		WINDOWINFO info;
		ZeroMemory(&info, sizeof(info));
		info.cbSize = sizeof(info);
		GetWindowInfo(targetWindow, &info);
		D2D1_SIZE_U siz;
		siz.height = ((info.rcClient.bottom) - (info.rcClient.top));
		siz.width = ((info.rcClient.right) - (info.rcClient.left));
		if (!IsIconic(overlayWindow)) {
			SetWindowPos(overlayWindow, NULL, info.rcClient.left, info.rcClient.top, siz.width, siz.height, SWP_SHOWWINDOW);
			target->Resize(&siz);
		}
		target->BeginDraw();
		target->Clear(D2D1::ColorF(0, 0, 0, 0));
		if (drawLoopCallback != NULL) {
			if (o_Foreground) {
				if (GetForegroundWindow() == targetWindow)
					goto toDraw;
				else goto noDraw;
			}

		toDraw:
			time_t postTime = clock();
			time_t frameTime = postTime - preTime;
			preTime = postTime;

			drawLoopCallback(siz.width, siz.height);
		}
	noDraw:
		target->EndDraw();
		Sleep(1);
	}
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uiMessage)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uiMessage, wParam, lParam);
	}
	return 0;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	DWORD lpdwProcessId;
	GetWindowThreadProcessId(hwnd, &lpdwProcessId);
	if (lpdwProcessId == GetCurrentProcessId())
	{
		enumWindow = hwnd;
		return FALSE;
	}
	return TRUE;
}

DWORD WINAPI OverlayThread(LPVOID lpParam)
{
	if (lpParam == NULL) {
		EnumWindows(EnumWindowsProc, NULL);
	}
	else {
		enumWindow = (HWND)lpParam;
	}
	d2oSetup(enumWindow);
	for (;;) {
		mainLoop();
	}
}

void Setup(DirectOverlayCallback callback, HWND _targetWindow) {
	drawLoopCallback = callback;
	CreateThread(0, 0, OverlayThread, _targetWindow, 0, NULL);
}

void DirectOverlaySetOption(DWORD option) {
	if (option & FOREGROUND) o_Foreground = true;
	if (option & Font_ARIAL) fontname = L"arial";
	if (option & Font_COURIER) fontname = L"Courier";
	if (option & Font_CALIBRI) fontname = L"Calibri";
	if (option & Font_GABRIOLA) fontname = L"Gabriola";
	if (option & Font_IMPACT) fontname = L"Impact";
}