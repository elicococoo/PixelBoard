#include "stdafx.h"
#include "PaintRect.h"
#include <cstring>
#include <set>
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <atlimage.h>
using namespace std;

PaintRect::PaintRect(){
	rectwidth = 10, rectheight = 10;
	netlines = false;
	xsize = 108, ysize = 192;
	for (int i = 0; i < xsize; i++)
		for (int j = 0; j < ysize; j++)
			paintArea[i][j] = 0xffffff;
	drawing = false;
	prex = prey = -1;
	filling = false;
}

PaintRect::~PaintRect(){

}

void PaintRect::getBrush(int a) {
	brush = CreateSolidBrush(RGB(a >> 16, (a & 0xff00) >> 8, a & 0xff));
}

void PaintRect::getPen(int a) {
	pen = CreatePen(PS_SOLID, 1, RGB(a >> 16, (a & 0xff00) >> 8, a & 0xff));
}

void PaintRect::drawNetLines(HDC hdc, int left, int top, int right, int bottom){
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
	SelectObject(hdc, pen);
	for (int i = 0; i < ysize; i++) {
		MoveToEx(hdc, left + i * rectwidth, top, NULL);
		LineTo(hdc, left + i * rectwidth, bottom);
	}
	for (int i = 0; i < xsize; i++) {
		MoveToEx(hdc, left, top + i * rectheight, NULL);
		LineTo(hdc, right, top + i * rectheight);
	}
	DeleteObject(pen);
}

void PaintRect::drawArea(HDC hdc){
	for (int i = 0; i < xsize; i++){
		for (int j = 0; j < ysize; j++){
			getBrush(paintArea[i][j]);
			getPen(paintArea[i][j]);
			SelectObject(hdc,brush);
			SelectObject(hdc, pen);
			Rectangle(hdc, j * rectwidth, 100 + i * rectheight, j * rectwidth + rectwidth, 100 + i * rectheight + rectheight);
			DeleteObject(brush);
			DeleteObject(pen);
		}
	}
}

void PaintRect::updateRect(HDC hdc, int x, int y) {
	getBrush(paintArea[x][y]);
	getPen(paintArea[x][y]);
	SelectObject(hdc, brush);
	SelectObject(hdc, pen);
	Rectangle(hdc, y * rectwidth , 100 + x * rectheight, y * rectwidth + rectwidth, 100 + x * rectheight + rectheight);
	DeleteObject(brush);
	DeleteObject(pen);
}

bool PaintRect::isStateChanged(int x, int y) {
	int nowx = x / rectwidth, nowy = (y - 100) / rectheight;
	if (nowx != prex || nowy != prey) {
		prex = nowx, prey = nowy;
		return true;
	}
	return false;
}

void PaintRect::save(HWND hWnd){
	BROWSEINFO  bi;
	TCHAR chPath[255] = { 0 }; //用来存储路径的字符串
	TCHAR buffer[255] = { 0 };
	ZeroMemory(&bi, sizeof(bi));
	bi.hwndOwner = hWnd;
	bi.pidlRoot = NULL;//初始化制定的root目录
	bi.lpszTitle = _T("请选择路径: ");
	bi.pszDisplayName = buffer;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_EDITBOX;

	LPITEMIDLIST lpDlist; //用来保存返回信息的IDList
	lpDlist = SHBrowseForFolder(&bi); //显示选择对话框
	if (lpDlist != NULL){  //用户按了确定按钮
		SHGetPathFromIDList(lpDlist, chPath);//把项目标识列表转化成字符串
		wstring tmp = chPath;
		int nLen = WideCharToMultiByte(CP_ACP, 0, chPath, -1, NULL, 0, NULL, NULL);
		char* pszDst = new char[nLen];
		WideCharToMultiByte(CP_ACP, 0, chPath, -1, pszDst, nLen, NULL, NULL);
		pszDst[nLen - 1] = 0;
		string s(pszDst);
		delete[] pszDst;
		FILE* file;
		fopen_s(&file, (s + "/myPaintBoard.pxl").c_str(), "w");
		for (int i = 0; i < xsize; i++) {
			for (int j = 0; j < ysize; j++)
				fprintf(file, "%d\n", paintArea[i][j]);
		}
		fclose(file);
	}
}

void PaintRect::load(HWND hWnd) {
	BROWSEINFO  bi;
	TCHAR chPath[255] = { 0 }; //用来存储路径的字符串
	TCHAR buffer[255] = { 0 };
	ZeroMemory(&bi, sizeof(bi));
	bi.hwndOwner = hWnd;
	bi.pidlRoot = NULL;//初始化制定的root目录
	bi.lpszTitle = _T("请选择路径: ");
	bi.pszDisplayName = buffer;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.ulFlags = BIF_BROWSEINCLUDEFILES | BIF_EDITBOX;

	LPITEMIDLIST lpDlist; //用来保存返回信息的IDList
	lpDlist = SHBrowseForFolder(&bi); //显//显示选择对话框
	if (lpDlist != NULL){  //用户按了确定按钮
		SHGetPathFromIDList(lpDlist, chPath);//把项目标识列表转化成字符串
		wstring tmp = chPath;
		if (tmp[tmp.size() - 1] != 't' && tmp[tmp.size() - 1] != 'T') return;
		if (tmp[tmp.size() - 2] != 'x' && tmp[tmp.size() - 1] != 'X') return;
		if (tmp[tmp.size() - 3] != 't' && tmp[tmp.size() - 1] != 'T') return;
		if (tmp[tmp.size() - 4] != '.') return;
		int nLen = WideCharToMultiByte(CP_ACP, 0, chPath, -1, NULL, 0, NULL, NULL);
		char* pszDst = new char[nLen];
		WideCharToMultiByte(CP_ACP, 0, chPath, -1, pszDst, nLen, NULL, NULL);
		pszDst[nLen - 1] = 0;
		string s(pszDst);
		delete[] pszDst;
		FILE* file;
		fopen_s(&file, s.c_str(), "r");
		int now = 0, t = 0, cnt = 0;
		for (int i = 0; i < xsize; i++) 
			for (int j = 0; j < ysize; j++){
				fscanf_s(file, "%d", &paintArea[i][j]);
			}
		fclose(file);
		InvalidateRect(hWnd, NULL, FALSE);
	}
}

void PaintRect::fill(HWND hWnd, int nowx, int nowy, int initcolor, int destcolor) {
	if (initcolor == destcolor) return;
	HDC hdc = GetDC(hWnd);
	getBrush(destcolor);
	getPen(destcolor);
	SelectObject(hdc, brush);
	SelectObject(hdc, pen);
	queue<pair<int, int> > q;
	q.push(make_pair(nowx, nowy));
	paintArea[nowx][nowy] = destcolor;
	Rectangle(hdc, nowy * rectwidth, nowx * rectheight + 100, nowy * rectwidth + rectwidth, nowx * rectheight + rectheight + 100);
	while (!q.empty()) {
		pair<int, int> pp, p = q.front();
		q.pop();
		pp.first = p.first + 1, pp.second = p.second;
		if (pp.first < xsize && paintArea[pp.first][pp.second] == initcolor){
			paintArea[pp.first][pp.second] = destcolor;
			Rectangle(hdc, pp.second * rectwidth, pp.first * rectheight + 100, pp.second * rectwidth + rectwidth, pp.first * rectheight + rectheight + 100);
			q.push(pp);
		}
		pp.first = p.first - 1, pp.second = p.second;
		if (pp.first >= 0 && paintArea[pp.first][pp.second] == initcolor) {
			paintArea[pp.first][pp.second] = destcolor;
			Rectangle(hdc, pp.second * rectwidth, pp.first * rectheight + 100, pp.second * rectwidth + rectwidth, pp.first * rectheight + rectheight + 100);
			q.push(pp);
		}
		pp.first = p.first, pp.second = p.second + 1;
		if (pp.second < ysize && paintArea[pp.first][pp.second] == initcolor) {
			paintArea[pp.first][pp.second] = destcolor;
			Rectangle(hdc, pp.second * rectwidth, pp.first * rectheight + 100, pp.second * rectwidth + rectwidth, pp.first * rectheight + rectheight + 100);
			q.push(pp);
		}
		pp.first = p.first, pp.second = p.second - 1;
		if (pp.second >= 0 && paintArea[pp.first][pp.second] == initcolor) {
			paintArea[pp.first][pp.second] = destcolor;
			Rectangle(hdc, pp.second * rectwidth, pp.first * rectheight + 100, pp.second * rectwidth + rectwidth, pp.first * rectheight + rectheight + 100);
			q.push(pp);
		}
	}
	DeleteObject(brush);
	DeleteObject(pen);
	ReleaseDC(hWnd, hdc);
}

void PaintRect::savejpg(HWND hWnd){
	BROWSEINFO  bi;
	TCHAR chPath[255] = { 0 }; //用来存储路径的字符串
	TCHAR buffer[255] = { 0 };
	ZeroMemory(&bi, sizeof(bi));
	bi.hwndOwner = hWnd;
	bi.pidlRoot = NULL;//初始化制定的root目录
	bi.lpszTitle = _T("请选择路径: ");
	bi.pszDisplayName = buffer;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_EDITBOX;

	LPITEMIDLIST lpDlist; //用来保存返回信息的IDList
	lpDlist = SHBrowseForFolder(&bi); //显//显示选择对话框
	if (lpDlist != NULL){  //用户按了确定按钮
		SHGetPathFromIDList(lpDlist, chPath);//把项目标识列表转化成字符串
		wstring tmp = chPath;
		tmp += L"\\myPaintBoard.jpg";
		CRect rcBk;
		GetClientRect(hWnd, &rcBk);
		HDC hdc = GetDC(hWnd);
		CImage Image;
		Image.Create(rcBk.Width(), rcBk.Height() - 100, 24);
		::BitBlt(Image.GetDC(), 0, 0, rcBk.Width(), rcBk.Height(), hdc, 0, 100, SRCCOPY);
		CString path = tmp.c_str();
		Image.Save(path);
		Image.ReleaseDC();
		Image.Destroy();
	}
}