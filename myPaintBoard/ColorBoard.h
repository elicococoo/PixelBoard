#pragma once

class ColorBoard{
private:
	void getBrush(int a);
	void getPen(int a);
public:
	int rectwidth, rectheight, x, y;
	const int INTERNAL = 51;
	int CurBrush, colors[220];
	bool gettingcolor;
	RECT refresh, buildcolor, R, G, B;
	ColorBoard();
	~ColorBoard();
	void drawColors(HDC hdc);
	void showCurBrush(HDC hdc);
	void showBuildColor(HDC hdc);
	void getColor(HDC hdc);
	bool isMouseDown;
	HBRUSH brush;
	HPEN pen;
};