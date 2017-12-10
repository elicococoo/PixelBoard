#include "stdAfx.h"
#include "ColorBoard.h"

ColorBoard::ColorBoard(){
	rectwidth =20, rectheight = 15;
	x = 20, y = 5;
	CurBrush = 0xffffff;
	int r = 0, g = 0, b = 0;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++){
			for (int k = 0; k < 6; k++){
				r = i * INTERNAL, g = j * INTERNAL, b = k * INTERNAL;
				colors[i * 36 + j * 6 + k] = (r << 16) + (g << 8) + b;
			}
		}
	}
	refresh.left = 950, refresh.right = 1050, refresh.top = 30, refresh.bottom = 90;
	R.left = G.left = B.left = 1100;
	R.right = G.right = B.right = 1100;
	R.top = 10, G.top = 40, B.top = 70;
	R.bottom = 30, G.bottom = 60, B.bottom = 90;
	isMouseDown = false;
	gettingcolor = false;
}

ColorBoard::~ColorBoard(){}

void ColorBoard::getBrush(int a) {
	brush = CreateSolidBrush(RGB(a >> 16, (a & 0xff00) >> 8, a & 0xff));
}

void ColorBoard::getPen(int a) {
	pen = CreatePen(PS_SOLID, 1, RGB(a >> 16, (a & 0xff00) >> 8, a & 0xff));
}

void ColorBoard::drawColors(HDC hdc){
	SelectObject(hdc, GetStockObject(NULL_PEN));
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 36; j++) {
			getBrush(colors[i * 36 + j]);
			SelectObject(hdc, brush);
			Rectangle(hdc, x + j * rectwidth, y + i * rectheight, x + j * rectwidth + rectwidth, y + i * rectheight + rectheight);
			DeleteObject(brush);
		}
	}
}

void ColorBoard::showCurBrush(HDC hdc) {
	getBrush(CurBrush);
	SelectObject(hdc, brush);
	SelectObject(hdc, GetStockObject(BLACK_PEN));
	Rectangle(hdc, 800, 5, 890, 95);
	DeleteObject(brush);
}

void ColorBoard::showBuildColor(HDC hdc){
	brush = CreateSolidBrush(RGB(R.right - R.left, G.right - G.left, B.right - B.left));
	SelectObject(hdc, brush);
	SelectObject(hdc, GetStockObject(BLACK_PEN));
	Rectangle(hdc, 1400, 5, 1490, 95);
	DeleteObject(brush);
}

void ColorBoard::getColor(HDC hdc) {
	if (gettingcolor) {
		brush = CreateSolidBrush(RGB(200, 200, 255));
		SelectObject(hdc, brush);
		Rectangle(hdc, 1550, 30, 1700, 90);
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, 1570, 50, L"获取颜色中...", 8);
	}
	else {
		brush = CreateSolidBrush(RGB(200, 255, 200));
		SelectObject(hdc, brush);
		Rectangle(hdc, 1550, 30, 1700, 90);
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, 1590, 50, L"获取颜色", 4);
	}
	DeleteObject(brush);
}