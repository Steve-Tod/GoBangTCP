#pragma once

#include "stdafx.h"

class ChessPoint
{
public:
	int x, y;
	int Status;
	ChessPoint(int X, int Y, int Sta = 0): x(X), y(Y), Status(Sta) {}
	BOOL InRange(CPoint &Position, int r);
};