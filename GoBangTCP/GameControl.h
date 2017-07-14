#pragma once
#include "stdafx.h"
#include <vector>
#include "ChessBoard.h"

class Ordinate
{
protected:
	char c;
	int n;
	Ordinate(char c, int n): c(c), n(n) {}
};
class GameRecord:public Ordinate
{
public:
	int TurnNum;
	GameRecord(char c, int n, int Turn): Ordinate(c, n), TurnNum(Turn) {}
	CString GetOneRecord();
	int GetChessNum();
};
class GameControl
{
private:
	std::vector<GameRecord> Record;
public:
	int turn;
	GameControl(void);
	~GameControl(void);
	int TakeStep(CPoint &Position, CChessBoard *Board);
	CString GetRecord();
	void ClearRecord();
	void Regret(CChessBoard *Board);
	int TakeStep(int num, CChessBoard *Board);
	int LastChess();
};

