#include "stdafx.h"
#include "ChessBoard.h"
#include <time.h>


CChessBoard::CChessBoard(int X0, int Y0, int SideLength)
{
	for (int i = 0; i <= 14; i++)
	{
		for (int j = 0; j <= 14; j++)
		{
			ChessPs.push_back(ChessPoint(X0, Y0));
			X0 += SideLength;
		}
		X0 -= SideLength * 15;
		Y0 += SideLength;
		ChessR = 0.4 * SideLength;
	}
}
CChessBoard::~CChessBoard(void)
{
}

void CChessBoard::DrawChess(CGoBangTCPDlg * const ThisDlg)
{
	CClientDC dc(ThisDlg);
	CPoint CCenter;
	for (std::vector<ChessPoint>::iterator iter= ChessPs.begin(); iter != ChessPs.end(); iter++)
	{
		if (iter->Status == 1)//黑棋
		{
			CCenter = CPoint(iter->x, iter->y);
			CPen pNewPen;  
			CPen *pOldPen;  
			pNewPen.CreatePen(PS_SOLID,1,RGB(0,0,0));  
			pOldPen = dc.SelectObject(&pNewPen);
			//定义画刷填充圆  
			CBrush fillbrush;  
			CBrush *oldbrush;  
			fillbrush.CreateSolidBrush(RGB(0,0,0));  
			oldbrush=dc.SelectObject(&fillbrush);  
  
			dc.Ellipse(CCenter.x - ChessR, CCenter.y - ChessR, CCenter.x + ChessR, CCenter.y + ChessR);   
  
			dc.SelectObject(oldbrush);  
			dc.SelectObject(pOldPen);  
			fillbrush.DeleteObject();  
			pNewPen.DeleteObject();
		}
		else if (iter->Status == -1)//白棋
		{
			CCenter = CPoint(iter->x, iter->y);
			CPen pNewPen;  
			CPen *pOldPen;  
			pNewPen.CreatePen(PS_SOLID,1,RGB(255,255,255));  
			pOldPen = dc.SelectObject(&pNewPen);
			//定义画刷填充圆  
			CBrush fillbrush;  
			CBrush *oldbrush;  
			fillbrush.CreateSolidBrush(RGB(255,255,255));  
			oldbrush=dc.SelectObject(&fillbrush);  
  
			dc.Ellipse(CCenter.x - ChessR, CCenter.y - ChessR, CCenter.x + ChessR, CCenter.y + ChessR);   
  
			dc.SelectObject(oldbrush);  
			dc.SelectObject(pOldPen);  
			fillbrush.DeleteObject();  
			pNewPen.DeleteObject();
		}
	}
}
void CChessBoard::DrawBoard(CGoBangTCPDlg * const ThisDlg)
{
	CClientDC dc(ThisDlg);
	//绘制棋盘
	for (int i = 0; i <= 14; i++)
	{
		int PointLeft = i * 15, PointRight = PointLeft + 14;
		char c = 'A' + i;
		CString Text(c);
		dc.TextOutW(ChessPs[PointLeft].x - 35, ChessPs[PointLeft].y - 9, Text);
		dc.MoveTo(ChessPs[PointLeft].x, ChessPs[PointLeft].y);
		dc.LineTo(ChessPs[PointRight].x, ChessPs[PointRight].y);
	}
	for (int i = 0; i<= 14; i++)
	{
		int PointUp = i, PointDown = PointUp + 210;
		char c[3];
		sprintf_s(c, 3, "%d", i +1);
		CString Text(c);
		if (i < 9)
		{
			dc.TextOutW(ChessPs[PointUp].x - 4, ChessPs[PointUp].y - 40, Text);
		}
		else
		{
			dc.TextOutW(ChessPs[PointUp].x - 8, ChessPs[PointUp].y - 40, Text);
		}
		dc.MoveTo(ChessPs[PointUp].x, ChessPs[PointUp].y);	
		dc.LineTo(ChessPs[PointDown].x, ChessPs[PointDown].y);
	}
	//绘制棋子
	DrawChess(ThisDlg);
}


CRect CChessBoard::GetBoardRect()
{
	return CRect(ChessPs[0].x, ChessPs[0].y, ChessPs[224].x, ChessPs[224].y);
}
int CChessBoard::GetSideLength()
{
	return (ChessPs[1].x - ChessPs[0].x);
}

void CChessBoard::NewGame()
{
	for (std::vector<ChessPoint>::iterator iter = ChessPs.begin(); iter != ChessPs.end(); iter++)
	{
		if (iter->Status != 0)
		{
			iter->Status = 0;
		}
	}
}

int CChessBoard::TakeStep(CPoint &Positon, int Black)
{
	for (std::vector<ChessPoint>::iterator iter = ChessPs.begin(); iter != ChessPs.end(); iter ++)
	{
		if (iter->InRange(Positon, ChessR))
		{
			if (iter->Status == 0)
			{
				iter->Status = Black;
				return iter - ChessPs.begin();
			}
			else
			{
				return -1;
			}
		}
	}
	return -1;
}
void CChessBoard::TakeStep(int num, int IsBlack)
{
	ChessPs[num].Status = IsBlack;
}
int CChessBoard::IsEnd(int LastChess)//判断是否结束
{
	BOOL flag = TRUE;
	for (std::vector<ChessPoint>::iterator iter = ChessPs.begin(); iter != ChessPs.end(); iter ++)
	{
		if (iter->Status == 0)
		{
			flag = FALSE; // 不是平局
			break;
		}
	}
	if (flag)
	{
		return 3; //3表示平局
	}
//2为白赢，4为黑赢
	int color = ChessPs[LastChess].Status;
	int cnt = 0, temp = LastChess;
	//横向五连？
	int LineNum = LastChess / 15;
	while (temp >= 0 &&  int(temp / 15) == LineNum && ChessPs[temp].Status == color)
	{
		cnt++;
		temp++;
	}
	temp = LastChess - 1;
	while (temp >= 0 && int(temp / 15) == LineNum && ChessPs[temp].Status == color)
	{
		cnt++;
		temp--;
	}
	if (cnt == 5)
	{
		return color + 3;
	}
	//纵向五连？
	cnt = 0;
	temp = LastChess;
	int ColumnNum = LastChess % 15;
	while (temp >= 0 && temp <= 224 && ChessPs[temp].Status == color)
	{
		cnt++;
		temp += 15;
	}
	temp = LastChess - 15;
	while (temp >= 0 && temp <= 224 && ChessPs[temp].Status == color )
	{
		cnt++;
		temp -= 15;
	}
	if (cnt == 5)
	{
		return color + 3;
	}
	//' \'向五连？
	cnt = 0;
	temp = LastChess;
	while (ChessPs[temp].Status == color)
	{
		if (temp % 15 == 14 || temp >= 210)
		{
			cnt++;
			break;
		}
		else
		{
			cnt++;
			temp += 16;
		}
	}
	cnt -= 1;
	temp = LastChess;
	while (ChessPs[temp].Status == color)
	{
		if (temp % 15 == 0 || temp <= 14)
		{
			cnt++;
			break;
		}
		else
		{
			cnt++;
			temp -= 16;
		}
	}
	if (cnt == 5)
	{
		return color + 3;
	}
	// '/'向五连
	cnt = 0;
	temp = LastChess;
	while (ChessPs[temp].Status == color)
	{
		if (temp % 15 == 0 || temp >= 210)
		{
			cnt++;
			break;
		}
		else
		{
			cnt++;
			temp += 14;
		}
	}
	cnt -= 1;
	temp = LastChess;
	while (ChessPs[temp].Status == color)
	{
		if (temp % 15 == 14 || temp <= 14)
		{
			cnt++;
			break;
		}
		else
		{
			cnt++;
			temp -= 14;
		}
	}
	if (cnt == 5)
	{
		return color + 3;
	}
	return 0;
}

void CChessBoard::Regret(int Last1, int Last2)
{
	ChessPs[Last1].Status = 0;
	ChessPs[Last2].Status = 0;
}

