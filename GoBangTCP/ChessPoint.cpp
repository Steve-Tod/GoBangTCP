#pragma once

#include "stdafx.h"
#include "ChessPoint.h"
BOOL ChessPoint::InRange(CPoint &Position, int r)
{
	if (abs(Position.x - x) <= r && abs(Position.y - y) <=r)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}