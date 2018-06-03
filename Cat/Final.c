// 캐릭터 뒤로 돌지 못함
// 비트맵 한장이 2000이 끝인데 4050은 어떻게 하는지
// 캐릭터 속도가 빨라 비트맵보다 먼저 감
// 부드럽게 움직이는 느낌
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include<string.h>	
#include <ctype.h>
#include <time.h>
#include "resource.h"
#include<math.h>
#include <conio.h>
#include <mmsystem.h>
#pragma comment (lib, "winmm.lib")


#define speed 4
#define mobsizex1 64
#define mobsizey1 58
#define errx 24
#define erry 2

typedef struct BLOCK
{
	RECT point;
	BOOL flag;
	int item;
}BLOCK;

BLOCK list[100];
BLOCK MOB[100];
BLOCK OBJ[100];
BLOCK TEM[100];

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM IParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow
	(lpszClass, "Final Project 고양이마리오",
		WS_OVERLAPPEDWINDOW,
		0, 0, 900, 900,
		NULL, (HMENU)NULL,
		hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HDC hdc, mem1dc, mem2dc;
	PAINTSTRUCT ps;
	RECT rv;
	static HBITMAP hbit1, hbit2, oldbit1, oldbit2, Catbit[8], mapbit[3], mobbit1[2], mobbit2[3], mobbit3[2], mobbit4[2], mobbit5[2];
	static HBITMAP objbit[17], muscle[2];
	BITMAP bmp;
	static int Width, Height;
	static int xpos, xpos2, ypos, mapx, mapx2, mapx3, mapx4, mapx5, mapx6;
	srand((unsigned int)time(NULL));
	int i, j;
	static int jcount, savex, savey, save1, save2, save3, save4, save5, save6, save7;
	static BOOL Jump, LJUMP, DIE, fishs;
	static int count, fish, fcount;
	static int move;
	static BOOL push, moveableL, moveableR1, moveableR2, heading, OverPower, muscleman, save, win;
	static int a;

	switch (iMsg)
	{
	case WM_CREATE:
		mciSendString("open field.wav type mpegvideo alias MediaFile", NULL, 0, NULL);
		mciSendString("play MediaFile", NULL, 0, NULL);

		win = FALSE;
		savex = 100;
		savey = 620;
		save1 = 0;
		save2 = 0;
		save3 = 0;
		save4 = 0;
		save5 = 0;
		save6 = 100;
		OverPower = FALSE;
		a = 0;
		fcount = 0;
		OverPower = FALSE;
		fish = 0;
		fishs = FALSE;
		DIE = FALSE;

		moveableL = TRUE;
		moveableR1 = TRUE;
		moveableR2 = TRUE;
		muscleman = FALSE;
		push = heading = FALSE;
		move = 1;
		jcount = 0;
		mapx = 0;
		xpos = xpos2 = 100;
		ypos = 620;
		mapx2 = mapx3 = mapx4 = mapx5 = mapx6 = 0;

		for (i = 0; i < 3; i++)
			mapbit[i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1 + i));

		for (i = 0; i < 8; i++)
			Catbit[i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4 + i));

		for (i = 0; i < 2; i++)
			mobbit1[i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP12 + i));

		for (i = 0; i < 3; i++)
			mobbit2[i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP14 + i));

		for (i = 0; i < 2; i++)
			mobbit3[i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP17 + i));

		for (i = 0; i < 2; i++)
			mobbit4[i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP19 + i));

		for (i = 0; i < 2; i++)
			mobbit5[i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP22 + i));

		objbit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP21));
		for (i = 0; i < 7; i++)
			objbit[1 + i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP24 + i));
		for (i = 0; i < 3; i++)
			objbit[8 + i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP33 + i));
		objbit[11] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP36));
		objbit[12] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP37));
		objbit[13] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP38));
		objbit[14] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP39));
		objbit[15] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP41));
		objbit[16] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP40));

		for (i = 0; i < 2; i++)
			muscle[i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP31 + i));

		list[0].point.left = 384 * 2;
		list[0].point.top = 464;
		list[1].point.left = 448 * 2;
		list[1].point.top = 464;
		list[2].point.left = 512 * 2;
		list[2].point.top = 464;
		list[3].point.left = 1472 * 2;
		list[3].point.top = 464;
		list[4].point.left = 1536 * 2;
		list[4].point.top = 464;
		list[5].point.left = 1568 * 2;
		list[5].point.top = 232;
		list[6].point.left = 1600 * 2;
		list[6].point.top = 232;
		list[7].point.left = 1632 * 2;
		list[7].point.top = 232;
		list[8].point.left = 1664 * 2;
		list[8].point.top = 232;
		list[9].point.left = 1696 * 2;
		list[9].point.top = 232;
		list[10].point.left = 1824 * 2;
		list[10].point.top = 232;
		list[11].point.left = 1856 * 2;
		list[11].point.top = 232;
		list[12].point.left = 1888 * 2;
		list[12].point.top = 232;
		list[13].point.left = 1888 * 2;
		list[13].point.top = 464;
		list[14].point.left = 2112 * 2;
		list[14].point.top = 464;
		list[15].point.left = 2144 * 2;
		list[15].point.top = 464;
		list[16].point.left = 3168 * 2;
		list[16].point.top = 464;
		list[17].point.left = 3200 * 2;
		list[17].point.top = 464;
		list[18].point.left = 3232 * 2;
		list[18].point.top = 464;
		list[19].point.left = 3264 * 2;
		list[19].point.top = 464;
		list[20].point.left = 2304 * 2 - 8;
		list[20].point.top = 691;
		list[21].point.left = 646 * 2;
		list[21].point.top = 528;
		list[22].point.left = 937 * 2;
		list[22].point.top = 472;
		list[23].point.left = 3040 * 2 - 18;
		list[23].point.top = 528;
		list[24].point.left = 3391 * 2 - 15;
		list[24].point.top = 583;
		list[25].point.left = 2624 * 2 - 15;
		list[25].point.top = 635;
		list[26].point.left = 2656 * 2 - 15;
		list[26].point.top = 581;
		list[27].point.left = 2688 * 2 - 15;
		list[27].point.top = 523;
		list[28].point.left = 2816 * 2 - 15;
		list[28].point.top = 523;
		list[29].point.left = 2848 * 2 - 15;
		list[29].point.top = 581;
		list[30].point.left = 3454 * 2 - 15;
		list[30].point.top = 635;
		list[31].point.left = 3486 * 2 - 15;
		list[31].point.top = 581;
		list[32].point.left = 3518 * 2 - 15;
		list[32].point.top = 519 + 4;
		list[33].point.left = 3550 * 2 - 15;
		list[33].point.top = 455 + 16;
		list[34].point.left = 3582 * 2 - 15;
		list[34].point.top = 391 + 25;
		list[35].point.left = 3614 * 2 - 15;
		list[35].point.top = 327 + 32;
		list[36].point.left = 3646 * 2 - 15;
		list[36].point.top = 263 + 42;
		list[37].point.left = 3678 * 2 - 15;
		list[37].point.top = 190 + 60;
		for (i = 0; i < 8; i++)
		{
			list[38 + i].point.left = 3710 * 2 - 15;
			list[38 + i].point.top = 190 + 60 + 55 * i;
		}
		//물음표
		list[45].point.left = 256 * 2;
		list[45].point.top = 464;
		list[45].item = 1;
		list[46].point.left = 416 * 2;
		list[46].point.top = 464;
		list[46].item = 1;
		list[47].point.left = 480 * 2;
		list[47].point.top = 464;
		list[47].item = 1;
		list[48].point.left = 448 * 2;
		list[48].item = 2;
		list[48].point.top = 232;
		list[49].point.left = 1504 * 2;
		list[49].point.top = 464;
		list[49].item = 3;
		list[50].point.left = 2272 * 2;
		list[50].point.top = 464;
		list[50].item = 1;
		list[51].point.left = 2336 * 2;
		list[51].point.top = 464;
		list[51].item = 1;
		list[52].point.left = 2336 * 2;
		list[52].point.top = 232;
		list[52].item = 1;
		list[53].point.left = 2400 * 2;
		list[53].point.top = 464;
		list[53].item = 1;
		//투명
		list[54].point.left = 1279 * 2;
		list[54].point.top = 464;
		list[55].point.left = 2848 * 2;
		list[55].point.top = 304;
		list[56].point.left = 2880 * 2 - 18;
		list[56].point.top = 530;
		list[57].point.left = 2912 * 2 - 18;
		list[57].point.top = 530;
		list[58].point.left = 2944 * 2 - 18;
		list[58].point.top = 530;
		list[59].point.left = 2976 * 2 - 18;
		list[59].point.top = 530;
		list[60].point.left = 3008 * 2 - 18;
		list[60].point.top = 530;
		// 중간 깃발
		list[61].point.left = 2144 * 2;      
		list[61].point.top = 245;
		TEM[45].point.left = list[45].point.left;
		TEM[45].point.top = list[45].point.top - 62;
		TEM[45].point.right = TEM[45].point.left + 46;
		TEM[45].point.bottom = list[46].point.top;
		TEM[46].point.left = list[46].point.left;
		TEM[46].point.top = list[46].point.top - 62;
		TEM[46].point.right = TEM[46].point.left + 46;
		TEM[46].point.bottom = list[46].point.top;
		TEM[47].point.left = list[47].point.left;
		TEM[47].point.top = list[47].point.top - 62;
		TEM[47].point.right = TEM[47].point.left + 46;
		TEM[47].point.bottom = list[47].point.top;
		TEM[48].point.left = list[48].point.left;
		TEM[48].point.top = list[48].point.top - 58;
		TEM[48].point.right = TEM[48].point.left + 64;
		TEM[48].point.bottom = list[48].point.top;
		TEM[49].point.left = list[49].point.left;
		TEM[49].point.top = list[49].point.top - 64;
		TEM[49].point.right = TEM[49].point.left + 64;
		TEM[49].point.bottom = list[49].point.top;
		TEM[50].point.left = list[50].point.left;
		TEM[50].point.top = list[50].point.top - 62;
		TEM[50].point.right = TEM[50].point.left + 46;
		TEM[50].point.bottom = list[50].point.top;
		TEM[51].point.left = list[51].point.left;
		TEM[51].point.top = list[51].point.top - 62;
		TEM[51].point.right = TEM[51].point.left + 46;
		TEM[51].point.bottom = list[51].point.top;
		TEM[52].point.left = list[52].point.left;
		TEM[52].point.top = list[52].point.top - 62;
		TEM[52].point.right = TEM[52].point.left + 46;
		TEM[52].point.bottom = list[52].point.top;
		TEM[53].point.left = list[53].point.left;
		TEM[53].point.top = list[53].point.top - 62;
		TEM[53].point.right = TEM[53].point.left + 46;
		TEM[53].point.bottom = list[53].point.top;
		for (i = 0; i < 20; i++)
		{
			list[i].point.right = list[i].point.left + 63;
			list[i].point.bottom = list[i].point.top + 64;
		}
		list[20].point.right = list[20].point.left + 320;
		list[20].point.bottom = list[20].point.top + 120;
		list[21].point.right = list[21].point.left + 63 * 2;
		list[21].point.bottom = list[21].point.top + 388;
		list[22].point.right = list[22].point.left + 63 * 2;
		list[22].point.bottom = list[22].point.top + 420;
		list[23].point.right = list[23].point.left + 63 * 2;
		list[23].point.bottom = list[23].point.top + 388;
		list[24].point.right = list[24].point.left + 63 * 2;
		list[24].point.bottom = list[24].point.top + 111;
		for (i = 25; i < 61; i++)
		{
			list[i].point.right = list[i].point.left + 64;
			list[i].point.bottom = list[i].point.top + 55;
		}
		for (i = 45; i < 56; i++)
		{
			list[i].point.right = list[i].point.left + 64;
			list[i].point.bottom = list[i].point.top + 64;
		}
		list[61].point.right = list[24].point.left + 114;
		list[61].point.bottom = list[24].point.top + 222;
		MOB[0].point.left = 2500;
		MOB[0].point.top = 635;
		MOB[0].point.right = MOB[0].point.left + mobsizex1;
		MOB[0].point.bottom = 635 + mobsizey1;
		MOB[1].point.left = 400;
		MOB[1].point.top = 635;
		MOB[1].point.right = MOB[1].point.left + mobsizex1;
		MOB[1].point.bottom = 635 + mobsizey1;
		MOB[2].point.left = 600;
		MOB[2].point.top = 635;
		MOB[2].point.right = MOB[2].point.left + mobsizex1;
		MOB[2].point.bottom = 635 + mobsizey1;
	
		MOB[7].point.left = 646 * 2 + errx;
		MOB[7].point.top = 528;
		MOB[7].point.right = MOB[7].point.left + 32;
		MOB[7].point.bottom = 635 + mobsizey1 + 46;
		
		
		mem1dc = CreateCompatibleDC(hdc);
		mem2dc = CreateCompatibleDC(mem1dc);

		hbit1 = CreateCompatibleBitmap(hdc, 900, 800);

		SetTimer(hwnd, 1, 1, NULL);
		break;
	case WM_TIMER:
		push = FALSE;
		if (!DIE || !muscleman)
		{
			if (!Jump)
			{
				if (GetAsyncKeyState('A') & 0x8000)
				{
					push = TRUE;
					if (xpos > 0)
						moveableL = TRUE;
					else
						moveableL = FALSE;
					for (i = 0; i < 61; i++)
					{
						if (i >= 54 && i < 61)
						{
							if (list[i].flag == FALSE)
								continue;
							else
							{
								if (xpos2 >= list[i].point.left - mapx &&xpos2 <= list[i].point.right - mapx && ypos > list[i].point.top && ypos < list[i].point.bottom)
								{
									moveableL = FALSE;
									break;
								}
								else if (xpos2 >= list[i].point.left - mapx &&xpos2 <= list[i].point.right - mapx && ypos + 74 > list[i].point.top && ypos + 74 < list[i].point.bottom)
								{
									moveableL = FALSE;
									break;
								}
								else if (xpos2 >= 1280 * 2 - mapx && xpos2 <= 1376 * 2 - mapx && ypos + 74 > 694)
								{
									moveableL = FALSE;
									break;
								}
								else if (xpos2 >= 1728 * 2 - mapx && xpos2 <= 1824 * 2 - mapx && ypos + 74 > 694)
								{
									moveableL = FALSE;
									break;
								}
								else if (xpos2 >= 2720 * 2 - 18 - mapx && xpos2 <= 2816 * 2 - 18 - mapx && ypos + 74 > 694)
								{
									moveableL = FALSE;
									break;
								}
								else if (xpos2 >= 3008 * 2 - 18 - mapx && xpos2 <= 3040 * 2 - 18 - mapx && ypos + 74 > 694)
								{
									moveableL = FALSE;
									break;
								}
							}
						}
						else if (xpos2 >= list[i].point.left - mapx &&xpos2 <= list[i].point.right - mapx && ypos > list[i].point.top && ypos < list[i].point.bottom)
						{
							moveableL = FALSE;
							break;
						}
						else if (xpos2 >= list[i].point.left - mapx &&xpos2 <= list[i].point.right - mapx && ypos + 74 > list[i].point.top && ypos + 74 < list[i].point.bottom)
						{
							moveableL = FALSE;
							break;
						}
						else if (xpos2 >= 1280 * 2 - mapx && xpos2 <= 1376 * 2 - mapx && ypos + 74 > 694)
						{
							moveableL = FALSE;
							break;
						}
						else if (xpos2 >= 1728 * 2 - mapx && xpos2 <= 1824 * 2 - mapx && ypos + 74 > 694)
						{
							moveableL = FALSE;
							break;
						}
						else if (xpos2 >= 2720 * 2 - 18 - mapx && xpos2 <= 2816 * 2 - 18 - mapx && ypos + 74 > 694)
						{
							moveableL = FALSE;
							break;
						}
						else if (xpos2 >= 3008 * 2 - 18 - mapx && xpos2 <= 3040 * 2 - 18 - mapx && ypos + 74 > 694)
						{
							moveableL = FALSE;
							break;
						}
					}
					if (moveableL)
					{
						xpos -= speed;
						xpos2 -= speed;
						move = 0;
					}
				}
				if (GetAsyncKeyState('D') & 0x8000)
				{
					push = TRUE;
					if (xpos <= 400)
						moveableR1 = TRUE;
					else if (mapx <= 4050)
						moveableR1 = FALSE;
					else if (mapx > 4050)
						moveableR1 = TRUE;
					for (i = 0; i < 61; i++)
					{
						if (i == 20)
							continue;
						else if (i >= 54 && i < 61)
						{
							if (list[i].flag == FALSE)
								continue;
							else
							{
								if (xpos2 + 52 >= list[i].point.left - mapx && xpos2 + 52 <= list[i].point.right - mapx && ypos > list[i].point.top && ypos < list[i].point.bottom)
								{
									moveableR1 = FALSE;
									moveableR2 = FALSE;
									break;
								}
								else if (xpos2 + 52 >= list[i].point.left - mapx && xpos2 + 52 <= list[i].point.right - mapx && ypos + 74 > list[i].point.top && ypos + 74 < list[i].point.bottom)
								{
									moveableR1 = FALSE;
									moveableR2 = FALSE;
									break;
								}
								else if (xpos2 + 52 >= list[i].point.left - mapx && xpos2 + 52 <= list[i].point.right - mapx && ypos < list[i].point.top && ypos + 74 >= list[i].point.bottom)
								{
									moveableR1 = FALSE;
									moveableR2 = FALSE;
									break;
								}
								else if (xpos2 + 52 >= 1280 * 2 - mapx && xpos2 + 52 <= 1376 * 2 - mapx && ypos + 74 > 694)
								{
									moveableR1 = FALSE;
									moveableR2 = FALSE;
									break;
								}
								else if (xpos2 + 52 >= 1728 * 2 - mapx && xpos2 + 52 <= 1824 * 2 - mapx && ypos + 74 > 694)
								{
									moveableR1 = FALSE;
									moveableR2 = FALSE;
									break;
								}
								else if (xpos2 + 52 >= 2720 * 2 - 18 - mapx && xpos2 + 52 <= 2816 * 2 - 18 - mapx && ypos + 74 > 694)
								{
									moveableR1 = FALSE;
									moveableR2 = FALSE;
									break;
								}
								else if (xpos2 + 52 >= 3008 * 2 - 18 - mapx && xpos2 + 52 <= 3040 * 2 - 18 - mapx && ypos + 74 > 694)
								{
									moveableR1 = FALSE;
									moveableR2 = FALSE;
									break;
								}
							}
						}
						else if (xpos2 + 52 >= list[i].point.left - mapx && xpos2 + 52 <= list[i].point.right - mapx && ypos > list[i].point.top && ypos < list[i].point.bottom)
						{
							moveableR1 = FALSE;
							moveableR2 = FALSE;
							break;
						}
						else if (xpos2 + 52 >= list[i].point.left - mapx && xpos2 + 52 <= list[i].point.right - mapx && ypos + 74 > list[i].point.top && ypos + 74 < list[i].point.bottom)
						{
							moveableR1 = FALSE;
							moveableR2 = FALSE;
							break;
						}
						else if (xpos2 + 52 >= list[i].point.left - mapx && xpos2 + 52 <= list[i].point.right - mapx && ypos < list[i].point.top && ypos + 74 >= list[i].point.bottom)
						{
							moveableR1 = FALSE;
							moveableR2 = FALSE;
							break;
						}
						else if (xpos2 + 52 >= 1280 * 2 - mapx && xpos2 + 52 <= 1376 * 2 - mapx && ypos + 74 > 694)
						{
							moveableR1 = FALSE;
							moveableR2 = FALSE;
							break;
						}
						else if (xpos2 + 52 >= 1728 * 2 - mapx && xpos2 + 52 <= 1824 * 2 - mapx && ypos + 74 > 694)
						{
							moveableR1 = FALSE;
							moveableR2 = FALSE;
							break;
						}
						else if (xpos2 + 52 >= 2720 * 2 - 18 - mapx && xpos2 + 52 <= 2816 * 2 - 18 - mapx && ypos + 74 > 694)
						{
							moveableR1 = FALSE;
							moveableR2 = FALSE;
							break;
						}
						else if (xpos2 + 52 >= 3008 * 2 - 18 - mapx && xpos2 + 52 <= 3040 * 2 - 18 - mapx && ypos + 74 > 694)
						{
							moveableR1 = FALSE;
							moveableR2 = FALSE;
							break;
						}
					}
					if (i == 61)
						moveableR2 = TRUE;
					if (moveableR1)
					{
						xpos += speed;
						xpos2 += speed;
						move = 1;
					}
					else
					{
						if (moveableR2)
						{
							xpos2 += speed;
							move = 1;
							if (mapx >= 3645)
							{
								if (mapx <= 4050)
								{
									mapx += speed;
									mapx4 += speed;
									{
										if (900 <= mapx4 * 2)
											mapx5 += speed;
									}
								}
								else
								{
								}
								mapx2 = mapx3 = 0;
							}
							else if (mapx >= 1595)
							{
								mapx += speed;
								mapx2 += speed;
								{
									if (900 <= mapx2 * 2)
										mapx3 += speed;
								}
								mapx4 = mapx5 = 0;
							}
							else
								mapx += speed;
						}
					}

				}
			}
			else
			{
				if (GetAsyncKeyState('A') & 0x8000)
				{
					if (xpos > 0)
						moveableL = TRUE;
					else
						moveableL = FALSE;
					for (i = 0; i < 61; i++)
					{
						if (i >= 54 && i < 61)
						{
							if (list[i].flag == FALSE)
								continue;
							else
							{
								if (xpos2 >= list[i].point.left - mapx &&xpos2 <= list[i].point.right - mapx && ypos > list[i].point.top && ypos < list[i].point.bottom)
								{
									moveableL = FALSE;
									break;
								}
								else if (xpos2 >= list[i].point.left - mapx &&xpos2 <= list[i].point.right - mapx && ypos + 74 > list[i].point.top && ypos + 74 < list[i].point.bottom)
								{
									moveableL = FALSE;
									break;
								}
								else if (xpos2 >= 1280 * 2 - mapx && xpos2 <= 1376 * 2 - mapx && ypos + 74 > 694)
								{
									moveableL = FALSE;
									break;
								}
								else if (xpos2 >= 1728 * 2 - mapx && xpos2 <= 1824 * 2 - mapx && ypos + 74 > 694)
								{
									moveableL = FALSE;
									break;
								}
								else if (xpos2 >= 2720 * 2 - 18 - mapx && xpos2 <= 2816 * 2 - 18 - mapx && ypos + 74 > 694)
								{
									moveableL = FALSE;
									break;
								}
								else if (xpos2 >= 3008 * 2 - 18 - mapx && xpos2 <= 3040 * 2 - 18 - mapx && ypos + 74 > 694)
								{
									moveableL = FALSE;
									break;
								}
							}
						}
						else if (xpos2 >= list[i].point.left - mapx &&xpos2 <= list[i].point.right - mapx && ypos > list[i].point.top && ypos < list[i].point.bottom)
						{
							moveableL = FALSE;
							break;
						}
						else if (xpos2 >= list[i].point.left - mapx &&xpos2 <= list[i].point.right - mapx && ypos + 74 > list[i].point.top && ypos + 74 < list[i].point.bottom)
						{
							moveableL = FALSE;
							break;
						}
						else if (xpos2 >= 1280 * 2 - mapx && xpos2 <= 1376 * 2 - mapx && ypos + 74 > 694)
						{
							moveableL = FALSE;
							break;
						}
						else if (xpos2 >= 1728 * 2 - mapx && xpos2 <= 1824 * 2 - mapx && ypos + 74 > 694)
						{
							moveableL = FALSE;
							break;
						}
						else if (xpos2 >= 2720 * 2 - 18 - mapx && xpos2 <= 2816 * 2 - 18 - mapx && ypos + 74 > 694)
						{
							moveableL = FALSE;
							break;
						}
						else if (xpos2 >= 3008 * 2 - 18 - mapx && xpos2 <= 3040 * 2 - 18 - mapx && ypos + 74 > 694)
						{
							moveableL = FALSE;
							break;
						}

					}
					if (moveableL)
					{
						xpos -= speed;
						xpos2 -= speed;
						move = 0;
					}
				}
				if (GetAsyncKeyState('D') & 0x8000)
				{
					if (xpos <= 400)
						moveableR1 = TRUE;
					else if (mapx <= 4050)
						moveableR1 = FALSE;
					else if (mapx > 4050)
						moveableR1 = TRUE;
					for (i = 0; i < 61; i++)
					{
						if (i == 20)
							continue;
						else if (i >= 54 && i < 61)
						{
							if (list[i].flag == FALSE)
								continue;
							else
							{
								if (xpos2 + 52 >= list[i].point.left - mapx && xpos2 + 52 <= list[i].point.right - mapx && ypos > list[i].point.top && ypos < list[i].point.bottom)
								{
									moveableR1 = FALSE;
									moveableR2 = FALSE;
									break;
								}
								else if (xpos2 + 52 >= list[i].point.left - mapx && xpos2 + 52 <= list[i].point.right - mapx && ypos + 74 > list[i].point.top && ypos + 74 < list[i].point.bottom)
								{
									moveableR1 = FALSE;
									moveableR2 = FALSE;
									break;
								}
								else if (xpos2 + 52 >= list[i].point.left - mapx && xpos2 + 52 <= list[i].point.right - mapx && ypos < list[i].point.top && ypos + 74 >= list[i].point.bottom)
								{
									moveableR1 = FALSE;
									moveableR2 = FALSE;
									break;
								}
								else if (xpos2 + 52 >= 1280 * 2 - mapx && xpos2 + 52 <= 1376 * 2 - mapx && ypos + 74 > 694)
								{
									moveableR1 = FALSE;
									moveableR2 = FALSE;
									break;
								}
								else if (xpos2 + 52 >= 1728 * 2 - mapx && xpos2 + 52 <= 1824 * 2 - mapx && ypos + 74 > 694)
								{
									moveableR1 = FALSE;
									moveableR2 = FALSE;
									break;
								}
								else if (xpos2 + 52 >= 2720 * 2 - 18 - mapx && xpos2 + 52 <= 2816 * 2 - 18 - mapx && ypos + 74 > 694)
								{
									moveableR1 = FALSE;
									moveableR2 = FALSE;
									break;
								}
								else if (xpos2 + 52 >= 3008 * 2 - 18 - mapx && xpos2 + 52 <= 3040 * 2 - 18 - mapx && ypos + 74 > 694)
								{
									moveableR1 = FALSE;
									moveableR2 = FALSE;
									break;
								}
							}
						}
						else if (xpos2 + 52 >= list[i].point.left - mapx && xpos2 + 52 <= list[i].point.right - mapx && ypos > list[i].point.top && ypos < list[i].point.bottom)
						{
							moveableR1 = FALSE;
							moveableR2 = FALSE;
							break;
						}
						else if (xpos2 + 52 >= list[i].point.left - mapx && xpos2 + 52 <= list[i].point.right - mapx && ypos + 74 > list[i].point.top && ypos + 74 < list[i].point.bottom)
						{
							moveableR1 = FALSE;
							moveableR2 = FALSE;
							break;
						}
						else if (xpos2 + 52 >= list[i].point.left - mapx && xpos2 + 52 <= list[i].point.right - mapx && ypos < list[i].point.top && ypos + 74 >= list[i].point.bottom)
						{
							moveableR1 = FALSE;
							moveableR2 = FALSE;
							break;
						}
						else if (xpos2 + 52 >= 1280 * 2 - mapx && xpos2 + 52 <= 1376 * 2 - mapx && ypos + 74 > 694)
						{
							moveableR1 = FALSE;
							moveableR2 = FALSE;
							break;
						}
						else if (xpos2 + 52 >= 1728 * 2 - mapx && xpos2 + 52 <= 1824 * 2 - mapx && ypos + 74 > 694)
						{
							moveableR1 = FALSE;
							moveableR2 = FALSE;
							break;
						}
						else if (xpos2 + 52 >= 2720 * 2 - 18 - mapx && xpos2 + 52 <= 2816 * 2 - 18 - mapx && ypos + 74 > 694)
						{
							moveableR1 = FALSE;
							moveableR2 = FALSE;
							break;
						}
						else if (xpos2 + 52 >= 3008 * 2 - 18 - mapx && xpos2 + 52 <= 3040 * 2 - 18 - mapx && ypos + 74 > 694)
						{
							moveableR1 = FALSE;
							moveableR2 = FALSE;
							break;
						}
					}
					if (i == 61)
						moveableR2 = TRUE;
					if (moveableR1)
					{
						xpos += speed;
						xpos2 += speed;
						move = 1;
					}
					else
					{
						if (moveableR2)
						{
							xpos2 += speed;
							move = 1;
							if (mapx >= 3645)
							{
								if (mapx <= 4050)
								{
									mapx += speed;
									mapx4 += speed;
									{
										if (900 <= mapx4 * 2)
											mapx5 += speed;
									}
								}
								else
								{
								}
								mapx2 = mapx3 = 0;
							}
							else if (mapx >= 1595)
							{
								mapx += speed;
								mapx2 += speed;
								{
									if (900 <= mapx2 * 2)
										mapx3 += speed;
								}
								mapx4 = mapx5 = 0;
							}
							else
								mapx += speed;
						}
					}
				}
			}
			for (i = 0; i < 3; i++)
			{
				if (MOB[i].flag == FALSE)
				{
					if (xpos2 + 52 >= MOB[i].point.left + errx && xpos2 + 52 <= MOB[i].point.right + 2 * errx)
					{
						if (abs(ypos - MOB[i].point.top) <= 15)
						{
							if (OverPower == FALSE)
							{
								PlaySound("death.wav", NULL, SND_ASYNC);
								DIE = TRUE;
								LJUMP = FALSE;
								Jump = FALSE;
								break;
							}
						}
						else if (ypos + 74 >= MOB[i].point.top)
						{
							if (!DIE)
							{
								MOB[i].flag = TRUE;
								jcount = 0;
								LJUMP = FALSE;
								Jump = FALSE;
								Jump = TRUE;
							}
						}
					}
				}
			}
			if (xpos2 >= 815 || fish )
			{
				fish = 1;
				if (MOB[7].point.bottom >= 0)
				{
					MOB[7].point.top -= 7;
					MOB[7].point.bottom -= 7;
				}
			}

			for (i = 0; i < 20; i++)
			{
				if (abs(ypos - list[i].point.bottom) <= 4 && xpos2 + 52 >= list[i].point.left - mapx && xpos2 + 52 <= list[i].point.right - mapx)
					heading = TRUE;
				else if (abs(ypos - list[i].point.bottom) <= 4 && xpos2 >= list[i].point.left - mapx && xpos2 <= list[i].point.right - mapx)
					heading = TRUE;
			}
			for (i = 45; i < 54; i++)
			{
				if (abs(ypos - list[i].point.bottom) <= 4 && xpos2 + 52 >= list[i].point.left - mapx && xpos2 + 52 <= list[i].point.right - mapx)
				{
					heading = TRUE;
					if (list[i].flag == FALSE)
					{
						TEM[i].flag = TRUE;
						list[i].flag = TRUE;
					}
				}
				else if (abs(ypos - list[i].point.bottom) <= 4 && xpos2 >= list[i].point.left - mapx && xpos2 <= list[i].point.right - mapx)
				{
					heading = TRUE;
					if (list[i].flag == FALSE)
					{
						TEM[i].flag = TRUE;
						list[i].flag = TRUE;
					}
				}
			}
			for (i = 54; i < 61; i++)
			{
				if (abs(ypos - list[i].point.bottom) <= 4 && xpos2 + 52 >= list[i].point.left - mapx && xpos2 + 52 <= list[i].point.right - mapx)
				{
					heading = TRUE;
					list[i].flag = TRUE;
				}
				else if (abs(ypos - list[i].point.bottom) <= 4 && xpos2 >= list[i].point.left - mapx && xpos2 <= list[i].point.right - mapx)
				{
					heading = TRUE;
					list[i].flag = TRUE;
				}
			}

			if (heading)
			{
				jcount = 0;
				Jump = FALSE;
				LJUMP = FALSE;
				ypos += 7;
				for (i = 0; i < 21; i++)
					if (xpos2 >= list[i].point.left - mapx - errx && xpos2 <= list[i].point.right - mapx  && abs(ypos + 74 - list[i].point.top + erry) <= 17)
						heading = FALSE;
				for (i = 45; i < 61; i++)
					if (xpos2 >= list[i].point.left - mapx - errx && xpos2 <= list[i].point.right - mapx  && abs(ypos + 74 - list[i].point.top + erry) <= 17)
						heading = FALSE;
				if (ypos >= 620)
					heading = FALSE;
			}
			if (Jump)
			{
				jcount++;
				if (jcount > 37)
				{
					ypos += 7;
					for (i = 0; i < 61; i++)
					{
						if (i >= 54 && i < 61)
						{
							if (list[i].flag == FALSE)
								continue;
							else
							{
								if (xpos2 >= list[i].point.left - mapx - errx && xpos2 <= list[i].point.right - mapx  && ypos + 74 >= list[i].point.top - erry && ypos + 74 <= list[i].point.bottom)
								{
									ypos = list[i].point.top - erry - 74 - 5;
									jcount = 0;
									Jump = FALSE;
								}
							}
						}
						else if (xpos2 >= list[i].point.left - mapx - errx && xpos2 <= list[i].point.right - mapx  && ypos + 74 >= list[i].point.top - erry && ypos + 74 <= list[i].point.bottom)
						{
							ypos = list[i].point.top - erry - 74 - 5;
							jcount = 0;
							Jump = FALSE;
						}
					}
					if (ypos >= 620)
					{
						ypos = 620;
						jcount = 0;
						Jump = FALSE;
					}
				}
				else
					ypos -= 7;
			}
			else if (LJUMP)
			{
				jcount++;
				if (jcount > 57)
				{
					ypos += 7;
					for (i = 0; i < 61; i++)
					{
						if (i >= 54 && i < 61)
						{
							if (list[i].flag == FALSE)
								continue;
							else
							{
								if (xpos2 >= list[i].point.left - mapx - errx && xpos2 <= list[i].point.right - mapx  && ypos + 74 >= list[i].point.top - erry && ypos + 74 <= list[i].point.bottom)
								{
									ypos = list[i].point.top - erry - 74 - 5;
									jcount = 0;
									LJUMP = FALSE;
								}
							}
						}
						else if (xpos2 >= list[i].point.left - mapx - errx && xpos2 <= list[i].point.right - mapx  && ypos + 74 >= list[i].point.top - erry && ypos + 74 <= list[i].point.bottom)
						{
							ypos = list[i].point.top - erry - 74 - 5;
							jcount = 0;
							LJUMP = FALSE;
						}
					}
					if (ypos >= 620)
					{
						ypos = 620;
						jcount = 0;
						LJUMP = FALSE;
					}
					if (heading)
					{
						jcount = 0;
						LJUMP = FALSE;
						heading = FALSE;
					}
				}
				else
					ypos -= 7;
			}
			if (xpos2 <= MOB[7].point.right - mapx && xpos2 + 58 >= MOB[7].point.left - mapx &&
				ypos <= MOB[7].point.bottom && ypos + 74 >= MOB[7].point.top)
				if (OverPower == FALSE)
				{
					DIE = TRUE;
					PlaySound("death.wav", NULL, SND_ASYNC);
				}
				else if (ypos >= 800)
				{
					DIE = TRUE;
					PlaySound("death.wav", NULL, SND_ASYNC);
				}
		}
		if (DIE == TRUE)
		{
			PlaySound("death.wav", NULL, SND_ASYNC);
			jcount++;
			if (jcount > 30)
			{
				ypos += 7;
				if (jcount > 159)
				{
					jcount = 0;
					LJUMP = FALSE;
				}
			}
			else
				ypos -= 7;
		}
		if (muscleman)
			ypos += 7;

		if (mapx > 1700)
		{
			if (push)
			{
				MOB[0].point.left -= 0.5;
				MOB[0].point.right -= 0.5;
			}
			else
			{
				MOB[0].point.left -= 2;
				MOB[0].point.right -= 2;
			}
		}
		if (push)
		{
			MOB[1].point.left -= 1;
			MOB[1].point.right -= 1;
			MOB[2].point.left -= 1;
			MOB[2].point.right -= 1;
		}
		else
		{
			MOB[1].point.left -= 2;
			MOB[1].point.right -= 2;
			MOB[2].point.left -= 2;
			MOB[2].point.right -= 2;
		}
		if (xpos2 <= 3968 * 2 - mapx  && xpos2 + 58 >= 3936 * 2 - mapx  &&
			ypos >= 55 && ypos + 74 <= 639)
			win = TRUE;

		if (win)
		{
			if (ypos <= 619)
				ypos += speed;
			else
			{
				if (xpos2 <= 4128 * 2)
				{
					xpos2 += 8;;
					xpos += 8;
				}
				else
				{
					xpos2 = 60000;
				}
			}
		}
		if (!LJUMP && !Jump && !DIE && !muscleman)
		{
			for (i = 0; i < 61; i++)
			{
				if (i >= 54 && i < 61)
				{
					if (list[i].flag == FALSE)
						continue;
					else
					{
						if (xpos2 >= list[i].point.left - mapx - errx && xpos2 <= list[i].point.right - mapx - errx + 15 && ypos + 74 > list[i].point.top - erry && ypos + 74 < list[i].point.bottom - erry)
						{
							ypos = list[i].point.top - erry - 73;
							break;
						}
						else if (ypos >= 620 && xpos2 <= 1280 * 2 - mapx)
						{
							ypos = 620;
							break;
						}
						else if (ypos >= 620 && xpos2 >= 1376 * 2 - mapx - errx && xpos2 < 1728 * 2 - mapx)
						{
							ypos = 620;
							break;
						}
						else if (ypos >= 620 && xpos2 >= 1824 * 2 - mapx - errx  && xpos2 < 2304 * 2 - mapx)
						{
							ypos = 620;
							break;
						}
						else if (ypos >= 620 && xpos2 >= 2464 * 2 - mapx - errx - 4 && xpos2 < 2720 * 2 - mapx - errx)
						{
							ypos = 620;
							break;
						}
						else if (ypos >= 620 && xpos2 >= 2816 * 2 - mapx - errx - errx && xpos2 < 3008 * 2 - mapx - errx)
						{
							ypos = 620;
							break;
						}
						else if (ypos >= 620 && xpos2 >= 3040 * 2 - mapx - errx && xpos2 < 4383 * 2 - mapx - errx)
						{
							ypos = 620;
							break;
						}
						else if (ypos >= 620 && xpos2 >= 4415 * 2 - mapx - errx)
						{
							ypos = 620;
							break;
						}
					}
				}
				else if (xpos2 >= list[i].point.left - mapx - errx && xpos2 <= list[i].point.right - mapx - errx + 15 && ypos + 74 > list[i].point.top - erry && ypos + 74 < list[i].point.bottom - erry)
				{
					ypos = list[i].point.top - erry - 73;
					break;
				}
				else if (ypos >= 620 && xpos2 <= 1280 * 2 - mapx)
				{
					ypos = 620;
					break;
				}
				else if (ypos >= 620 && xpos2 >= 1376 * 2 - mapx - errx && xpos2 < 1728 * 2 - mapx)
				{
					ypos = 620;
					break;
				}
				else if (ypos >= 620 && xpos2 >= 1824 * 2 - mapx - errx  && xpos2 < 2304 * 2 - mapx)
				{
					ypos = 620;
					break;
				}
				else if (ypos >= 620 && xpos2 >= 2464 * 2 - mapx - errx - 4 && xpos2 < 2720 * 2 - mapx - errx)
				{
					ypos = 620;
					break;
				}
				else if (ypos >= 620 && xpos2 >= 2816 * 2 - mapx - errx - errx && xpos2 < 3008 * 2 - mapx - errx)
				{
					ypos = 620;
					break;
				}
				else if (ypos >= 620 && xpos2 >= 3040 * 2 - mapx - errx && xpos2 < 4383 * 2 - mapx - errx)
				{
					ypos = 620;
					break;
				}
				else if (ypos >= 620 && xpos2 >= 4415 * 2 - mapx - errx)
				{
					ypos = 620;
					break;
				}
			}
			if (i == 61)
				ypos += 7;
		}

		for (i = 45; i < 54; i++)
		{
			if (TEM[i].flag == TRUE)
			{
				if (list[i].item == 1) 
				{
					
					if (xpos2 <= TEM[i].point.right - mapx && xpos2 + 58 >= TEM[i].point.left - mapx &&
						ypos <= TEM[i].point.bottom && ypos + 74 >= TEM[i].point.top)
					{
						TEM[i].flag = FALSE;
						PlaySound("coin.wav", NULL, SND_ASYNC);
					}
				}
				else if (list[i].item == 2) {
					if (xpos2 <= TEM[i].point.right - mapx && xpos2 + 58 >= TEM[i].point.left - mapx &&
						ypos <= TEM[i].point.bottom && ypos + 74 >= TEM[i].point.top)
					{
						TEM[i].flag = FALSE;
					}
				}
				else if (list[i].item == 3) {
					if (xpos2 <= TEM[i].point.right - mapx && xpos2 + 58 >= TEM[i].point.left - mapx &&
						ypos <= TEM[i].point.bottom && ypos + 74 >= TEM[i].point.top)
					{
						TEM[i].flag = FALSE;
						muscleman = TRUE;
						PlaySound("powerup.wav", NULL, SND_ASYNC);
					}
				}
			}
		}
		if (xpos2 <= list[61].point.right - mapx && xpos2 + 58 >= list[61].point.left - mapx &&
			ypos <= list[61].point.bottom && ypos + 74 >= list[61].point.top)
		{
			savex = xpos2;
			savey = ypos;
			save1 = mapx;
			save2 = mapx2;
			save3 = mapx3;
			save4 = mapx4;
			save5 = mapx5;
			save6 = xpos;
		}

		if (xpos2 >= 2500)
		{
			list[20].point.top += 8;
			list[20].point.bottom += 8;
		}

		if ((GetAsyncKeyState('L') & 0x8000))
		{

			xpos2 = savex;
			ypos = savey;
			mapx = save1;
			mapx2 = save2;
			mapx3 = save3;
			mapx4 = save4;
			mapx5 = save5;
			xpos = save6;
			OverPower = FALSE;
			muscleman = FALSE;

			for (i = 45; i < 61; i++)
			{
				list[i].flag = FALSE;
				TEM[i].flag = FALSE;
			}
			for (i = 0; i < 100; i++)
				MOB[i].flag = FALSE;
			fishs = FALSE;
			fish = 0;
			fcount = 0;
			DIE = FALSE;
		}
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_KEYDOWN:
		if (xpos2 >= 1280 * 2 - mapx && xpos2 <= 1376 * 2 - mapx && ypos + 74 >= 694)
		{
		}
		else if (xpos2 >= 1728 * 2 - mapx && xpos2 <= 1824 * 2 - mapx && ypos + 74 >= 694)
		{
		}
		else if(xpos2 >= 2304 * 2 - mapx && xpos2 <= 2464 * 2 - mapx && ypos + 74 >= 694)
		{
		}
		else if (xpos2 >= 2720 * 2 - 18 - mapx && xpos2 <= 2816 * 2 - 18 - mapx && ypos + 74 >= 694)
		{
		}
		else if (xpos2 >= 3008 * 2 - 18 - mapx && xpos2 <= 3040 * 2 - 18 - mapx && ypos + 74 >= 694)
		{
		}
		else if (DIE)
		{

		}
		else
		{
			if ((wParam == 'W') && (GetAsyncKeyState(VK_CONTROL)))
			{
				LJUMP = TRUE;
				PlaySound("jump.wav", NULL, SND_ASYNC);
			}
			else if (wParam == 'W')
			{
				Jump = TRUE;
				PlaySound("jump.wav", NULL, SND_ASYNC);
			}
		}
		if (wParam == 'P')
			KillTimer(hwnd, 1);
		else if (wParam == 'S')
			SetTimer(hwnd, 1, 1, NULL);
		else if (wParam == 'K') {
			DIE = TRUE;
			PlaySound("death.wav", NULL, SND_ASYNC);
		}
		else if (wParam == 'O')
		{
			if (OverPower)
				OverPower = FALSE;
			else
				OverPower = TRUE;
		}
		break;
	case WM_PAINT:
		GetClientRect(hwnd, &rv);
		hdc = BeginPaint(hwnd, &ps);

		if (hbit1 == NULL)
			hbit1 = CreateCompatibleBitmap(hdc, 900, 800);

		oldbit1 = (HBITMAP)SelectObject(mem1dc, hbit1);
		FillRect(mem1dc, &rv, GetStockObject(WHITE_BRUSH));
		oldbit2 = (HBITMAP)SelectObject(mem2dc, mapbit[0]);
		StretchBlt(mem1dc, 0, 0, 900, 800, mem2dc, mapx, 0, 450, 464, SRCCOPY);

		if (mapx >= 3645)
		{
			oldbit2 = (HBITMAP)SelectObject(mem2dc, mapbit[1]);
			StretchBlt(mem1dc, 0, 0, 900, 800, mem2dc, mapx - 2048, 0, 450, 464, SRCCOPY);
			oldbit2 = (HBITMAP)SelectObject(mem2dc, mapbit[2]);
			StretchBlt(mem1dc, 900 - mapx4 * 2 - 5, 0, 900, 800, mem2dc, mapx5, 0, 450, 464, SRCCOPY);
			if (900 <= mapx4 * 2)
				StretchBlt(mem1dc, 0, 0, 900, 800, mem2dc, 0, 0, 450, 464, SRCCOPY);
		}
		else if (mapx >= 1595)
		{
			/*	oldbit2 = (HBITMAP)SelectObject(mem2dc, mapbit[0]);
			StretchBlt(mem1dc, 0, 0, 900, 800, mem2dc, mapx, 0, 450, 464, SRCCOPY);*/
			oldbit2 = (HBITMAP)SelectObject(mem2dc, mapbit[1]);
			StretchBlt(mem1dc, 900 - mapx2 * 2, 0, 900, 800, mem2dc, mapx3, 0, 450, 464, SRCCOPY);
			if (900 <= mapx2 * 2)
				StretchBlt(mem1dc, 0, 0, 900, 800, mem2dc, mapx3, 0, 450, 464, SRCCOPY);
		}
		for (i = 0; i < 3; i++)
		{
			if (MOB[i].flag == FALSE)
			{
				SelectObject(mem2dc, mobbit1[0]);
				TransparentBlt(mem1dc, MOB[i].point.left - mapx, MOB[i].point.top, 64, 58, mem2dc, 0, 0, 32, 29, RGB(153, 255, 255));
			}
		}
		if (fish)
		{
			if (fish == 1)
			{
				SelectObject(mem2dc, mobbit3[0]);
				TransparentBlt(mem1dc, MOB[7].point.left - mapx * 2, MOB[7].point.top, 64, 64, mem2dc, 0, 0, 32, 46, RGB(153, 255, 255));
			}
			else if (fish == 2)
			{
				SelectObject(mem2dc, mobbit3[1]);
				TransparentBlt(mem1dc, MOB[7].point.left - mapx * 2, MOB[7].point.top, 64, 64, mem2dc, 0, 0, 32, 46, RGB(153, 255, 255));
			}
		}
		for (i = 0; i < 20; i++)
		{
			SelectObject(mem2dc, objbit[11]);
			TransparentBlt(mem1dc, list[i].point.left - mapx * 2, list[i].point.top, 64, 64, mem2dc, 0, 0, 32, 32, RGB(153, 255, 255));
		}
		SelectObject(mem2dc, objbit[12]);
		TransparentBlt(mem1dc, list[20].point.left - mapx * 2, list[20].point.top, 320, 60 * (1.7), mem2dc, 0, 0, 160, 60, RGB(153, 255, 255));
		
		SelectObject(mem2dc, objbit[10]);
		TransparentBlt(mem1dc, list[61].point.left - mapx * 2, list[61].point.top, 114, 222, mem2dc, 0, 0, 114, 222, RGB(153, 255, 255));
		for (i = 45; i < 54; i++)
		{
			if (list[i].flag == FALSE)
			{
				SelectObject(mem2dc, objbit[13]);
				TransparentBlt(mem1dc, list[i].point.left - mapx * 2, list[i].point.top, 64, 64, mem2dc, 0, 0, 32, 32, RGB(153, 255, 255));
			}
			else if (list[i].flag == TRUE)
			{
				SelectObject(mem2dc, objbit[14]);
				TransparentBlt(mem1dc, list[i].point.left - mapx * 2, list[i].point.top, 64, 64, mem2dc, 0, 0, 32, 32, RGB(153, 255, 255));
			}

		}

		for (i = 54; i < 61; i++)
		{
			if (list[i].flag == TRUE)
			{
				SelectObject(mem2dc, objbit[14]);
				TransparentBlt(mem1dc, list[i].point.left - mapx * 2, list[i].point.top, 64, 64, mem2dc, 0, 0, 32, 32, RGB(153, 255, 255));
			}
		}

		for (i = 45; i < 54; i++)
		{
			if (TEM[i].flag == TRUE)
			{
				if (list[i].item == 1)
				{
					SelectObject(mem2dc, objbit[1]);
					TransparentBlt(mem1dc, TEM[i].point.left - mapx * 2, TEM[i].point.top, 46, 62, mem2dc, 0, 0, 23, 31, RGB(153, 255, 255));
				}
				else if (list[i].item == 2)
				{
					SelectObject(mem2dc, mobbit1[1]);
					TransparentBlt(mem1dc, TEM[i].point.left - mapx * 2, TEM[i].point.top, 64, 58, mem2dc, 0, 0, 32, 29, RGB(153, 255, 255));
				}
				if (list[i].item == 3)
				{
					SelectObject(mem2dc, objbit[2]);
					TransparentBlt(mem1dc, TEM[i].point.left - mapx * 2, TEM[i].point.top, 64, 64, mem2dc, 0, 0, 32, 32, RGB(153, 255, 255));
				}
			}
		}

		SelectObject(mem2dc, objbit[15]);
		TransparentBlt(mem1dc, 3936 * 2, 55, 584, 336, mem2dc, 0, 0, 21, 336, RGB(153, 255, 255));

		if (move == 1)
		{
			if (DIE)
			{
				SelectObject(mem2dc, Catbit[6]);
				TransparentBlt(mem1dc, xpos, ypos, 52, 74, mem2dc, 0, 0, 26, 37, RGB(153, 255, 255));
			}
			else if (muscleman)
			{
				SelectObject(mem2dc, muscle[0]);
				TransparentBlt(mem1dc, xpos, ypos, 130, 222, mem2dc, 0, 0, 65, 111, RGB(153, 255, 255));
			}
			else if (Jump || LJUMP)
			{
				SelectObject(mem2dc, Catbit[4]);
				TransparentBlt(mem1dc, xpos, ypos, 52, 74, mem2dc, 0, 0, 26, 37, RGB(153, 255, 255));
			}
			else if (push == FALSE)
			{
				SelectObject(mem2dc, Catbit[0]);
				TransparentBlt(mem1dc, xpos, ypos, 50, 74, mem2dc, 0, 0, 25, 37, RGB(153, 255, 255));
			}
			else if (push == TRUE)
			{
				SelectObject(mem2dc, Catbit[2]);
				TransparentBlt(mem1dc, xpos, ypos, 50, 74, mem2dc, 0, 0, 25, 37, RGB(153, 255, 255));
			}
		}
		else
		{
			if (DIE)
			{
				SelectObject(mem2dc, Catbit[6]);
				TransparentBlt(mem1dc, xpos, ypos, 52, 74, mem2dc, 0, 0, 26, 37, RGB(153, 255, 255));
			}
			else if (muscleman)
			{
				SelectObject(mem2dc, muscle[1]);
				TransparentBlt(mem1dc, xpos, ypos, 130, 222, mem2dc, 0, 0, 65, 111, RGB(153, 255, 255));
			}
			else if (Jump || LJUMP)
			{
				SelectObject(mem2dc, Catbit[5]);
				TransparentBlt(mem1dc, xpos, ypos, 52, 74, mem2dc, 0, 0, 26, 37, RGB(153, 255, 255));
			}
			else if (push == FALSE)
			{
				SelectObject(mem2dc, Catbit[1]);
				TransparentBlt(mem1dc, xpos, ypos, 50, 74, mem2dc, 0, 0, 25, 37, RGB(153, 255, 255));
			}
			else if (push == TRUE)
			{
				SelectObject(mem2dc, Catbit[3]);
				TransparentBlt(mem1dc, xpos, ypos, 50, 74, mem2dc, 0, 0, 25, 37, RGB(153, 255, 255));
			}
		}


		StretchBlt(hdc, 0, 0, 900, 800, mem1dc, 0, 0, 850, 800, SRCCOPY);

		SelectObject(mem1dc, oldbit1);
		SelectObject(mem2dc, oldbit2);

		DeleteObject(hbit1);
		hbit1 = NULL;
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		for (i = 0; i < 3; i++)
			DeleteObject(mapbit[i]);
		for (i = 0; i < 8; i++)
			DeleteObject(Catbit[i]);
		for (i = 0; i < 17; i++)
			DeleteObject(objbit[i]);
		for (i = 0; i < 2; i++)
			DeleteObject(muscle[i]);
		for (i = 0; i < 2; i++)
			DeleteObject(mobbit1[i]);
		for (i = 0; i < 3; i++)
			DeleteObject(mobbit2[i]);
		for (i = 0; i < 2; i++)
			DeleteObject(mobbit3[i]);
		for (i = 0; i < 2; i++)
			DeleteObject(mobbit4[i]);
		for (i = 0; i < 2; i++)
			DeleteObject(mobbit5[i]);

		DeleteDC(mem1dc);
		DeleteDC(mem2dc);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

