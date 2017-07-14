#pragma once

#include "stdafx.h"
#include "ChessPoint.h"
#include "GoBangTCP.h"
#include "GoBangTCPDlg.h"
#include <vector>

class CChessBoard
{
private:
	std::vector<ChessPoint> ChessPs;
	int ChessR;
public:
	CChessBoard(int X0, int Y0, int SideLength);
	~CChessBoard(void);
	void DrawBoard(CGoBangTCPDlg * const ThisDlg);
	void DrawChess(CGoBangTCPDlg * const ThisDlg);
	CRect GetBoardRect();
	int GetSideLength();
	void NewGame();
	int TakeStep(CPoint &Positon, BOOL Black);
	int IsEnd(int LastChess);
	void Regret(int Last1, int Last2);
	void TakeStep(int num, int IsBlack);
};

