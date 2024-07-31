#include "stdafx.h"
#include "draw2.h"
#include <vector>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include<cmath>
#include<windows.h>

#define MAX_LOADSTRING 100


// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// buttonsg
HWND hwndButton;

// sent data

char figure = '1', current_figure = '0';
int moveX = 100, moveY = 200, moveLineY = 0, moveLineX = 0, objectCount = 0, high = 0;
bool catch_figure = 0;
int moveXtab[10], moveYtab[10];
char figures[10];

RECT drawArea1 = { 0, 41, 2000, 260 };

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Buttons(HWND, UINT, WPARAM, LPARAM);


void background(HDC hdc) {
	Graphics graphics(hdc);
	Pen pen1(Color(255, 249, 29, 255));
	Pen pen5(Color(255, 1, 1, 160));

	graphics.DrawLine(&pen5, 0, 240, 8000, 240);
	graphics.DrawLine(&pen5, 42, 240, 42, 42);	//pionowe
	graphics.DrawLine(&pen5, 70, 240, 70, 42);
	graphics.DrawLine(&pen5, 300, 42, 300, 60);	//pozioma

	graphics.DrawLine(&pen5, 42, 42, 300, 42);	//pozioma
	graphics.DrawLine(&pen5, 42, 60, 300, 60);	//pozioma

	graphics.DrawLine(&pen5, moveX + 20 + moveLineX, 60, moveX + 20 + moveLineX, moveY - 20 + moveLineY);


	for (int i = 1; i <= objectCount; i++) {
		if (figures[i] == '1') {
			graphics.DrawRectangle(&pen1, moveXtab[i], moveYtab[i], 40, 40);
		}
		else if (figures[i] == '2') {
			graphics.DrawLine(&pen1, moveXtab[i] + 20, moveYtab[i], moveXtab[i], moveYtab[i] + 40);
			graphics.DrawLine(&pen1, moveXtab[i] + 20, moveYtab[i], moveXtab[i] + 40, moveYtab[i] + 40);
			graphics.DrawLine(&pen1, moveXtab[i], moveYtab[i] + 40, moveXtab[i] + 40, moveYtab[i] + 40);

		}
		else if (figures[i] == '3') {
			graphics.DrawEllipse(&pen1, moveXtab[i], moveYtab[i], 40, 40);
		}

	}

}
void MyOnPaint(HDC hdc, char figure, char current_figure)
{
	Graphics graphics(hdc);
	Pen pen1(Color(255, 249, 29, 255));
	Pen pen2(Color(255, 1, 1, 160));

	background(hdc);

	if (figure == 'o') {
		background(hdc);
	}
	else if (figure == '1') {
		graphics.DrawRectangle(&pen1, moveX, moveY, 40, 40);
	}
	else if (figure == '2') {
		graphics.DrawLine(&pen1, moveX + 20, moveY, moveX, moveY + 40);
		graphics.DrawLine(&pen1, moveX + 20, moveY, moveX + 40, moveY + 40);
		graphics.DrawLine(&pen1, moveX, moveY + 40, moveX + 40, moveY + 40);
	}
	else if (figure == '3') {
		graphics.DrawEllipse(&pen1, moveX, moveY, 40, 40);
	}

}

void repaintWindow(HWND hWnd, HDC& hdc, PAINTSTRUCT& ps, RECT* drawArea, char figure, char current_figure)
{
	if (drawArea == NULL)
		InvalidateRect(hWnd, NULL, TRUE); // repaint all
	else
		InvalidateRect(hWnd, drawArea, TRUE); //repaint drawArea
	hdc = BeginPaint(hWnd, &ps);

	MyOnPaint(hdc, figure, current_figure);
	EndPaint(hWnd, &ps);
}

// main function (exe hInstance)
int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	MSG msg;
	HACCEL hAccelTable;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DRAW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAW));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);

	return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAW));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_DRAW);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;


	hInst = hInstance; // Store instance handle (of exe) in our global variable

	// main window
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	// create button and store the handle                                                       
	hwndButton = CreateWindow(TEXT("button"),
		TEXT("+ kwadrat"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 300,
		80, 50,
		hWnd,
		(HMENU)ID_BUTTON1,
		hInstance,
		NULL);
	hwndButton = CreateWindow(TEXT("button"),
		TEXT("+ trojkat"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 350,
		80, 50,
		hWnd,
		(HMENU)ID_BUTTON2,
		hInstance,
		NULL);
	hwndButton = CreateWindow(TEXT("button"),
		TEXT("+ kolo"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 400,
		80, 50,
		hWnd,
		(HMENU)ID_BUTTON3,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("U"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		275, 300,
		25, 25,
		hWnd,
		(HMENU)ID_BUTTON_U,
		hInstance,
		NULL);
	hwndButton = CreateWindow(TEXT("button"),
		TEXT("D"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		275, 350,
		25, 25,
		hWnd,
		(HMENU)ID_BUTTON_D,
		hInstance,
		NULL);
	hwndButton = CreateWindow(TEXT("button"),
		TEXT("R"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		300, 325,
		25, 25,
		hWnd,
		(HMENU)ID_BUTTON_R,
		hInstance,
		NULL);
	hwndButton = CreateWindow(TEXT("button"),
		TEXT("L"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		250, 325,
		25, 25,
		hWnd,
		(HMENU)ID_BUTTON_L,
		hInstance,
		NULL);
	hwndButton = CreateWindow(TEXT("button"),
		TEXT("o"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		275, 325,
		25, 25,
		hWnd,
		(HMENU)ID_BUTTON_0,
		hInstance,
		NULL);


	hwndButton = CreateWindow(TEXT("button"),
		TEXT("kwadraty"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		350, 300,
		80, 50,
		hWnd,
		(HMENU)ID_BUTTON10,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("trojkaty"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		350, 350,
		80, 50,
		hWnd,
		(HMENU)ID_BUTTON11,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("kola"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		350, 400,
		80, 50,
		hWnd,
		(HMENU)ID_BUTTON12,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("ON"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		700, 300, 50, 50,
		hWnd,
		(HMENU)ID_BUTTON_ON,
		hInstance,
		NULL);


	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	int current_weight = 10;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// MENU & BUTTON messages
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_BUTTON1:
			objectCount++;

			figure = '1';
			if (figure == current_figure) high += 40;
			figures[objectCount] = figure;
			moveX = 120, moveY = 200, moveLineY = 0, moveLineX = 0;

			repaintWindow(hWnd, hdc, ps, &drawArea1, figure, current_figure);

			break;
		case ID_BUTTON2:
			objectCount++;

			figure = '2';
			if (figure == current_figure ) high += 40;
			figures[objectCount] = figure;

			moveX = 120, moveY = 200, moveLineY = 0, moveLineX = 0;
			repaintWindow(hWnd, hdc, ps, &drawArea1, figure, current_figure);
			break;
		case ID_BUTTON3:
			objectCount++;

			figure = '3';
			if (figure == current_figure ) high += 40;
			figures[objectCount] = figure;

			moveX = 120, moveY = 200, moveLineY = 0, moveLineX = 0;
			repaintWindow(hWnd, hdc, ps, &drawArea1, figure, current_figure);
			break;

		case ID_BUTTON10:
			current_figure = '1';
			high = 0;
			break;
		case ID_BUTTON11:
			current_figure = '2';
			high = 0;
			break;
		case ID_BUTTON12:
			current_figure = '3';
			high = 0;
			break;

		case ID_BUTTON_U:
			if (moveY > 70 && catch_figure == 1) moveY -= 10;
			if (catch_figure == 0 && moveLineY > -120) moveLineY -= 10;
			repaintWindow(hWnd, hdc, ps, &drawArea1, figure, current_figure);
			break;
		case ID_BUTTON_D:
			if (moveY < 200 && catch_figure == 1)	moveY += 10;
			if (moveY >= 200 && moveY < 220 && moveLineY < 20) moveLineY += 10;
			repaintWindow(hWnd, hdc, ps, &drawArea1, figure, current_figure);
			break;
		case ID_BUTTON_R:
			if (moveX < 270 && catch_figure == 1) moveX += 10;
			if (catch_figure == 0) moveLineX += 10;

			repaintWindow(hWnd, hdc, ps, &drawArea1, figure, current_figure);
			break;
		case ID_BUTTON_L:
			if (moveX > 80 && catch_figure == 1) moveX -= 10;
			if (catch_figure == 0) moveLineX -= 10;

			repaintWindow(hWnd, hdc, ps, &drawArea1, figure, current_figure);
			break;
		case ID_BUTTON_0:
			if (moveY == 200 && moveLineX == 0 && catch_figure == 0 && current_figure == figure) catch_figure = 1;
			else if (catch_figure == 1 && (moveY == 200 - high && moveLineX == 0) && high <= 80) {
				catch_figure = 0;
				moveXtab[objectCount] = moveX;
				moveYtab[objectCount] = moveY;
			}
			repaintWindow(hWnd, hdc, ps, &drawArea1, figure, current_figure);
			break;
		case ID_BUTTON_ON:
			figure = 'o';
			for (int i = 0; i < 10; i++) {
				moveXtab[i] = 0, moveYtab[i] = 0, figures[i] = '0';
			}
			repaintWindow(hWnd, hdc, ps, &drawArea1, figure, current_figure);
			break;


		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}