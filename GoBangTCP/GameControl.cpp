#include "stdafx.h"
#include "GameControl.h"

CString GameRecord::GetOneRecord()
{
	CString s;
	s.Format(L"第%d步（%c, %d）\r\n", TurnNum, c, n);
	return s;
}
int GameRecord::GetChessNum()
{
	return ((c - 'A') * 15 + n - 1);
}
GameControl::GameControl(void)
{
	turn = 1;//黑棋先手
}
GameControl::~GameControl(void)
{
}

int GameControl::TakeStep(CPoint &Position, CChessBoard *Board)
{
	int Result = Board->TakeStep(Position, 2 * (turn % 2) -1);
	if (Result < 0)
	{
		return 0;
	}
	else
	{
		char c = Result / 15 + 'A';
		int n = Result % 15  + 1;
		Record.push_back(GameRecord(c, n, turn));
		turn++;
		int end = Board->IsEnd(Result);
		if (end != 0)
		{
			return end;
		}
		else
		{
			return 1;
		}
	}
}
int GameControl::TakeStep(int num, CChessBoard *Board)
{
	Board->TakeStep(num, 2 * (turn %2) - 1);
	char c = num / 15 + 'A';
	int n = num % 15  + 1;
		Record.push_back(GameRecord(c, n, turn));
		turn++;
		int end = Board->IsEnd(num);
		if (end != 0)
		{
			return end;
		}
		else
		{
			return 1;
		}
}
CString GameControl::GetRecord()
{
	CString s, temp;
	std::vector<GameRecord>::iterator iter = Record.end();
	iter--;
	if (iter->TurnNum % 2)
	{
		temp = L"黑棋";
		s += temp + iter->GetOneRecord();
	}
	else
	{
		temp = L"白棋";
		s += temp + iter->GetOneRecord();
	}
	return s;
}

void GameControl::ClearRecord()
{
	Record.clear();
	turn = 1;
}

void GameControl::Regret(CChessBoard *Board)
{
	std::vector<GameRecord>::iterator iter = Record.end();
	iter--;
	int Last2 = iter->GetChessNum();
	iter--;
	int Last1 = iter->GetChessNum();
	Record.pop_back();
	Record.pop_back();
	Board->Regret(Last1, Last2);
	turn -= 2;
}

int GameControl::LastChess()
{
	std::vector<GameRecord>::iterator iter = Record.end();
	iter--;
	return iter->GetChessNum();
}