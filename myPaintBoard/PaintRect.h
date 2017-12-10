#pragma once

class PaintRect{
private:
	void getBrush(int a);
	void getPen(int a);
public:
	int paintArea[200][200];
	int rectwidth, rectheight, xsize, ysize;
	int prex, prey;
	bool netlines;
	bool drawing;
	bool isStateChanged(int x, int y);
	PaintRect();
	~PaintRect();
	void drawNetLines(HDC hdc, int left, int top, int right, int bottom);
	void drawArea(HDC hdc);
	void updateRect(HDC hdc, int x, int y);
	HBRUSH brush;
	HPEN pen;
	void save(HWND hWnd);
	void load(HWND hWnd);
	bool filling;
	void fill(HWND hWnd, int nowx, int nowy, int initcolor, int destcolor);
	void savejpg(HWND hWnd);
};