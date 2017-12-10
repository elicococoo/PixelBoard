// myPaintBoard.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "myPaintBoard.h"
#include "PaintRect.h"
#include "ColorBoard.h"

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MYPAINTBOARD, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYPAINTBOARD));

	// 主消息循环: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  函数:  MyRegisterClass()
//
//  目的:  注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYPAINTBOARD));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MYPAINTBOARD);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数:  InitInstance(HINSTANCE, int)
//
//   目的:  保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	static int cxClient, cyClient;
	static PaintRect paintrect;
	static ColorBoard colorboard;

	switch (message)
	{
	case WM_CREATE:
		cxClient = GetSystemMetrics(SM_CXSCREEN);
		cyClient = GetSystemMetrics(SM_CYSCREEN);
		MoveWindow(hWnd, 0, 0, cxClient, cyClient, FALSE);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_NETLINES:{
			if (paintrect.netlines) CheckMenuItem(GetMenu(hWnd), ID_NETLINES, MF_UNCHECKED);
			else CheckMenuItem(GetMenu(hWnd), ID_NETLINES, MF_CHECKED);
			paintrect.netlines = !paintrect.netlines;
			InvalidateRect(hWnd, NULL, FALSE);
		//	UpdateWindow(hWnd);
			break;
		}
		case ID_CLEAR:
			for (int i = 0; i < paintrect.xsize; i++)
				for (int j = 0; j < paintrect.ysize; j++)
					paintrect.paintArea[i][j] = 0xffffff;
			InvalidateRect(hWnd, NULL, FALSE);
		break;
		case ID_SAVE:
			paintrect.save(hWnd);
			break;
		case ID_LOAD:
			paintrect.load(hWnd);
			break;
		case ID_SAVE_JPG:
			paintrect.savejpg(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO:  在此添加任意绘图代码...
		{
			SelectObject(hdc, GetStockObject(WHITE_BRUSH));
			Rectangle(hdc, 0, 0, cxClient, cyClient);

			HBRUSH brush = CreateSolidBrush(RGB(200, 200, 200));
			SelectObject(hdc, brush);
			SelectObject(hdc, GetStockObject(NULL_PEN));
			Rectangle(hdc, 0, 0, cxClient, 100);

			brush = CreateSolidBrush(RGB(100, 200, 300));
			SelectObject(hdc, brush);
			Rectangle(hdc, colorboard.refresh.left, colorboard.refresh.top, colorboard.refresh.right, colorboard.refresh.bottom);
			SetBkMode(hdc, TRANSPARENT);
			TextOut(hdc, colorboard.refresh.left + 30, colorboard.refresh.top + 20, L"刷 新", 3);


			brush = CreateSolidBrush(RGB(255, 0, 0));
			SelectObject(hdc, brush);
			Rectangle(hdc, colorboard.R.left, colorboard.R.top, colorboard.R.right, colorboard.R.bottom);
			brush = CreateSolidBrush(RGB(0, 255, 0));
			SelectObject(hdc, brush);
			Rectangle(hdc, colorboard.G.left, colorboard.G.top, colorboard.G.right, colorboard.G.bottom);
			brush = CreateSolidBrush(RGB(0, 0, 255));
			SelectObject(hdc, brush);
			Rectangle(hdc, colorboard.B.left, colorboard.B.top, colorboard.B.right, colorboard.B.bottom);
			brush = CreateSolidBrush(RGB(255, 255, 255));
			SelectObject(hdc, brush);
			Rectangle(hdc, colorboard.R.right, colorboard.R.top, colorboard.R.left + 255, colorboard.R.bottom);
			Rectangle(hdc, colorboard.G.right, colorboard.G.top, colorboard.G.left + 255, colorboard.G.bottom);
			Rectangle(hdc, colorboard.B.right, colorboard.B.top, colorboard.B.left + 255, colorboard.B.bottom);

			if (paintrect.filling == false) brush = CreateSolidBrush(RGB(255, 200, 200));
			else brush = brush = CreateSolidBrush(RGB(200, 100, 200));
			SelectObject(hdc, brush);
			Rectangle(hdc, 1720, 30, 1880, 90);
			SetBkMode(hdc, TRANSPARENT);
			TextOut(hdc, 1750, 40, L"填充模式", 4);
			if (paintrect.filling == false) TextOut(hdc, 1760, 60, L"关闭", 2);
			else TextOut(hdc, 1760, 60, L"启动中", 3);
			DeleteObject(brush);

			colorboard.drawColors(hdc);
			colorboard.showCurBrush(hdc);
			paintrect.drawArea(hdc);
			colorboard.showBuildColor(hdc);
			colorboard.getColor(hdc);

			if (paintrect.netlines){
				paintrect.drawNetLines(hdc, 0, 100, cxClient, cyClient);
			}
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:{
		hdc = GetDC(hWnd);
		int x = LOWORD(lParam), y = HIWORD(lParam);
		if (x >= colorboard.x && x <= colorboard.x + 36 * colorboard.rectwidth && y >= colorboard.y && y <= colorboard.y + 6 * colorboard.rectheight){
			colorboard.CurBrush = colorboard.colors[(x - colorboard.x) / colorboard.rectwidth + (y - colorboard.y) / colorboard.rectheight * 36];
			colorboard.showCurBrush(hdc);
		}
		else if (x >= 0 && x <= cxClient && y >= 100 && y <= cyClient){
			if (colorboard.gettingcolor) {
				colorboard.gettingcolor = false;
				colorboard.CurBrush = paintrect.paintArea[(y - 100) / paintrect.rectheight][x / paintrect.rectwidth];
				colorboard.getColor(hdc);
				colorboard.showCurBrush(hdc);
			}
			else if (paintrect.filling){
				int tmpx = (y - 100) / paintrect.rectheight, tmpy = x / paintrect.rectwidth;
				paintrect.fill(hWnd, tmpx, tmpy, paintrect.paintArea[tmpx][tmpy], colorboard.CurBrush);
		//		InvalidateRect(hWnd, NULL, FALSE);
			}
			else {
				paintrect.drawing = true;
				paintrect.paintArea[(y - 100) / paintrect.rectheight][x / paintrect.rectwidth] = colorboard.CurBrush;
				paintrect.updateRect(hdc, (y - 100) / paintrect.rectheight, x / paintrect.rectwidth);
			}
		}
		else if (x >= colorboard.refresh.left && x <= colorboard.refresh.right && y >= colorboard.refresh.top && y <= colorboard.refresh.bottom) InvalidateRect(hWnd, NULL, FALSE);
		else if (x >= colorboard.R.left && x <= colorboard.R.left + 255 && y >= colorboard.R.top && y <= colorboard.R.bottom) {
			RECT rect = colorboard.R;
			colorboard.R.right = x;
			rect.right = rect.left + 255;
			InvalidateRect(hWnd, &rect, FALSE);
			colorboard.showBuildColor(hdc);
			colorboard.isMouseDown = true;
		}
		else if (x >= colorboard.G.left && x <= colorboard.G.left + 255 && y >= colorboard.G.top && y <= colorboard.G.bottom) {
			RECT rect = colorboard.G;
			colorboard.G.right = x;
			rect.right = rect.left + 255;
			InvalidateRect(hWnd, &rect, FALSE);
			colorboard.showBuildColor(hdc);
			colorboard.isMouseDown = true;
		}
		else if (x >= colorboard.B.left && x <= colorboard.B.left + 255 && y >= colorboard.B.top && y <= colorboard.B.bottom) {
			RECT rect = colorboard.B;
			colorboard.B.right = x;
			rect.right = rect.left + 255;
			InvalidateRect(hWnd, &rect, FALSE);
			colorboard.showBuildColor(hdc);
			colorboard.isMouseDown = true;
		}
		else if (x >= 1400 && x <= 1490 && y >= 5 && y <= 95) {
			colorboard.CurBrush = ((colorboard.R.right - colorboard.R.left) << 16) + 
				((colorboard.G.right - colorboard.G.left) << 8) + (colorboard.B.right - colorboard.B.left);
			colorboard.showCurBrush(hdc);
		}
		else if (x >= 1550 && x <= 1700 && y >= 30 && y <= 90 && colorboard.gettingcolor == false){
			colorboard.gettingcolor = true;
			colorboard.getColor(hdc);
		}
		else if (x >= 1720 && x <= 1880 && y >= 30 && y <= 90){
			paintrect.filling = !paintrect.filling;
			RECT rect;
			rect.left = 1720, rect.right = 1880, rect.top = 30, rect.bottom = 90;
			InvalidateRect(hWnd, &rect, FALSE);
		}
		ReleaseDC(hWnd, hdc);
		break;
	}
	case WM_LBUTTONUP:
		paintrect.drawing = false;
		paintrect.prex = paintrect.prey = -1;
		colorboard.isMouseDown = false;
		break;
	case WM_MOUSEMOVE:
	{
		hdc = GetDC(hWnd);
		int x = LOWORD(lParam), y = HIWORD(lParam);
		if (x >= 0 && x <= cxClient && y >= 100 && y <= cyClient && paintrect.drawing){
			if (paintrect.isStateChanged(x, y)){
				paintrect.paintArea[(y - 100) / paintrect.rectheight][x / paintrect.rectwidth] = colorboard.CurBrush;
				paintrect.updateRect(hdc, (y - 100) / paintrect.rectheight, x / paintrect.rectwidth);
			}
		}
		else if (x >= colorboard.R.left && x <= colorboard.R.left + 255 && y >= colorboard.R.top && y <= colorboard.R.bottom && colorboard.isMouseDown) {
			RECT rect = colorboard.R;
			colorboard.R.right = x;
			rect.right = rect.left + 255;
			InvalidateRect(hWnd, &rect, FALSE);
			colorboard.showBuildColor(hdc);
		}
		else if (x >= colorboard.G.left && x <= colorboard.G.left + 255 && y >= colorboard.G.top && y <= colorboard.G.bottom && colorboard.isMouseDown) {
			RECT rect = colorboard.G;
			colorboard.G.right = x;
			rect.right = rect.left + 255;
			InvalidateRect(hWnd, &rect, FALSE);
			colorboard.showBuildColor(hdc);
		}
		else if (x >= colorboard.B.left && x <= colorboard.B.left + 255 && y >= colorboard.B.top && y <= colorboard.B.bottom && colorboard.isMouseDown) {
			RECT rect = colorboard.B;
			colorboard.B.right = x;
			rect.right = rect.left + 255;
			InvalidateRect(hWnd, &rect, FALSE);
			colorboard.showBuildColor(hdc);
		}
		ReleaseDC(hWnd, hdc);
	}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
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
