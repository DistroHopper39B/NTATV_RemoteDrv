/* Apple TV Remote Driver for Windows XP
 * Copyright (C) Sylas Hollander. All Rights Reserved.
 * Based on atvclient by Christoph Cantillon and Peter Korsgaard
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include <windows.h>
#include <appleirapi.h>
#include <commctrl.h>

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 480

static const wchar_t WINDOW_NAME[] = L"Apple TV Utility";


LRESULT CALLBACK WindowProc(HWND hwnd,
							UINT uMsg,
							WPARAM wParam,
							LPARAM lParam);

static void UseDefaultFont(HWND hwnd)
{
	SendMessage(hwnd, WM_SETFONT, (WPARAM) GetStockObject(DEFAULT_GUI_FONT), (LPARAM) TRUE);
}

HWND CreateTabControl(HINSTANCE hInstance, HWND parentWindow)
{
	HWND hTabView = NULL;
	TCITEM tie = {};

	hTabView = CreateWindowEx(0,
							  WC_TABCONTROL,
							  L"",
							  WS_CHILD | WS_VISIBLE | TCS_FIXEDWIDTH,
							  5,
							  5,
							  WINDOW_WIDTH - 20,
							  WINDOW_HEIGHT - 60,
							  parentWindow,
							  NULL,
							  hInstance,
							  NULL);

	if (!hTabView)
	{
		MessageBox(NULL, L"UI ERROR: Could not create tabs", WINDOW_NAME, MB_OK | MB_ICONERROR);
		return NULL;
	}

	UseDefaultFont(hTabView);

	TabCtrl_SetItemSize(hTabView, 45, 20);

	tie.mask	= TCIF_TEXT;
	tie.pszText	= L"LED";

	if (TabCtrl_InsertItem(hTabView, 0, &tie) == -1)
	{
		DestroyWindow(hTabView);
		MessageBox(NULL, L"Couldn't add tab LED", WINDOW_NAME, MB_OK | MB_ICONERROR);
		return NULL;
	}

	tie.pszText = L"Remote";

	if (TabCtrl_InsertItem(hTabView, 0, &tie) == -1)
	{
		DestroyWindow(hTabView);
		MessageBox(NULL, L"Couldn't add tab Remote", WINDOW_NAME, MB_OK | MB_ICONERROR);
		return NULL;
	}

	tie.pszText = L"About";

	if (TabCtrl_InsertItem(hTabView, 0, &tie) == -1)
	{
		DestroyWindow(hTabView);
		MessageBox(NULL, L"Couldn't add tab About", WINDOW_NAME, MB_OK | MB_ICONERROR);
		return NULL;
	}

	return hTabView;
}

int WINAPI wWinMain(HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					PWSTR pCmdLine,
					int nCmdShow)
{
	HWND hMainWindow 	= NULL;
	HWND hTabView 		= NULL;
	HWND hTab_LED 		= NULL;
	HWND hTab_Remote 	= NULL;
	HWND hTab_About 	= NULL;

	const wchar_t CLASS_NAME[] = L"Apple TV Utility";

	MSG 					msg = {};
	WNDCLASSEX 				wc = {};
	INITCOMMONCONTROLSEX 	icce = {};

	appleir_device_handle	device = NULL;

	// Initialize IR controller
	device = appleir_open();
	if (!device)
	{
		MessageBox(NULL,
				   L"Fatal error: IR receiver not found or in use by another program",
				   WINDOW_NAME,
				   MB_OK | MB_ICONERROR);
		return ERROR_DEVICE_IN_USE;
	}

	// Initialize common controls
	icce.dwSize	= sizeof(INITCOMMONCONTROLSEX);
	icce.dwICC	= ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icce);

	// Create window
	wc.hInstance 		= hInstance;
	wc.lpszClassName 	= CLASS_NAME;
	wc.lpfnWndProc 		= WindowProc;
	wc.style			= CS_DBLCLKS;
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm			= LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor			= LoadIcon(NULL, IDC_ARROW);
	wc.lpszMenuName		= NULL;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra 		= 0;
	wc.hbrBackground	= GetSysColorBrush(COLOR_BTNFACE);


	if (!RegisterClassEx(&wc))
	{
		return (int) GetLastError();
	}

	hMainWindow = CreateWindowEx(0,
							   	WC_TABCONTROL,
							   	WINDOW_NAME,
							   	WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
							   	CW_USEDEFAULT,
							   	CW_USEDEFAULT,
							   	WINDOW_WIDTH,
							   	WINDOW_HEIGHT,
							   	HWND_DESKTOP,
							   	NULL,
							   	hInstance,
							   	NULL);



	if (!hMainWindow)
		return 0;


	hTabView = CreateTabControl(hInstance, hMainWindow);
	if (!hTabView)
		return 0;


	ShowWindow(hMainWindow, nCmdShow);

	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd,
							UINT uMsg,
							WPARAM wParam,
							LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW + 1));

			EndPaint(hwnd, &ps);
		}
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}