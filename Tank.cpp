
#include <iostream>

#include "Tank.h"
#include "MyTools.h"
#include "ScreenSingleton.h"

using namespace std;
using namespace MyTools;

bool Tank::isInside(double x1, double x2) const
{
	const double XBeg = x + 2;
	const double XEnd = x + width - 1;

	if (x1 < XBeg && x2 > XEnd)
	{
		return true;
	}

	if (x1 > XBeg && x1 < XEnd)
	{
		return true;
	}

	if (x2 > XBeg && x2 < XEnd)
	{
		return true;
	}

	return false;
}

void Tank::Draw() const
{
	ScreenSingleton::getInstance().SetColor(CC_Cyan);
	ScreenSingleton::getInstance().GotoXY(x, y - 3);
	cout << "    #####";
	ScreenSingleton::getInstance().GotoXY(x-2, y - 2);
	cout << "#######   #";
	ScreenSingleton::getInstance().GotoXY(x, y - 1);
	cout << "    #####";
	ScreenSingleton::getInstance().GotoXY(x,y);
	cout << " ###########";
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
TankAdaptee::TankAdaptee() : x(0.0), y(0.0), width(0) { }

inline uint16_t TankAdaptee::GetScore() const { return score; }

inline void TankAdaptee::SetPos(double nx, double ny) { x = nx; y = ny; }

inline double TankAdaptee::GetY() const { return y; }

inline double TankAdaptee::GetX() const { return x; }

inline void TankAdaptee::SetWidth(uint16_t widthN) { width = widthN; }

inline uint16_t TankAdaptee::GetWidth() const { return width; }

void TankAdaptee::Paint() const
{
	ScreenSingleton::getInstance().SetColor(CC_Brown);
	ScreenSingleton::getInstance().GotoXY(x, y - 3);
	 std::cout << "    #####";
	ScreenSingleton::getInstance().GotoXY(x - 2, y - 2);
	 std::cout << "#######   #";
	ScreenSingleton::getInstance().GotoXY(x, y - 1);
	 std::cout << "    #####";
	ScreenSingleton::getInstance().GotoXY(x, y);
	 std::cout << " ###########";
}

bool __fastcall TankAdaptee::isInRange(double x1, double x2) const
{
	const double XBeg = x + 2;
	const double XEnd = x + width - 1;

	if (x1 < XBeg && x2 > XEnd)
	{
		return true;
	}

	if (x1 > XBeg && x1 < XEnd)
	{
		return true;
	}

	if (x2 > XBeg && x2 < XEnd)
	{
		return true;
	}

	return false;
}

void TankAdapter::SetPos(double nx, double ny)
{
	tank.SetPos(nx, ny);
}

double TankAdapter::GetY() const { return tank.GetY(); }

double TankAdapter::GetX() const { return tank.GetX(); }

void TankAdapter::SetWidth(uint16_t widthN) { tank.SetWidth(widthN); }

uint16_t TankAdapter::GetWidth() const
{
	return tank.GetWidth();
}

void TankAdapter::Draw() const
{
	tank.Paint();
}

uint16_t TankAdapter::GetScore() const
{
	return tank.GetScore();
}

bool __fastcall TankAdapter::isInside(double x1, double x2) const
{
	return tank.isInRange(x1, x2);
} 
