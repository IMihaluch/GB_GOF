#include <iostream>
#include "House.h"
#include "MyTools.h"
#include "ScreenSingleton.h"
#include "HouseBuilder.h"

using namespace std;
using namespace MyTools;

bool House::isInside(double x1, double x2) const
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

void House::Draw() const
{
	ScreenSingleton::getInstance().SetColor(CC_Yellow);
    for (int i = 6; i >= 0; i--) 
 {
	ScreenSingleton::getInstance().GotoXY(x, y - i);
	for (int e = 0; e < 14; e++) 
	{
		cout << look[i][e];
	}
 }
}

void HouseBuilderA::Constube()
{
	for (int i = 0; i < 6; i++) 
		product_House->look[6][i] = ' ';
	    product_House->look[6][6] = '#';
	for (int i = 7; i < 14; i++)
		product_House->look[6][i] = ' ';
}

void HouseBuilderA::Consroof()
{
	for (int i = 0; i < 3; i++) 
		product_House->look[5][i] = ' ';
	for (int i = 3; i < 11; i++) 
		product_House->look[5][i] = '#';
	for (int i = 11; i < 14; i++) 
		product_House->look[5][i] = ' ';
}

void HouseBuilderA::Conswalls() 
{
	product_House->look[0][0] = ' ';
	for (int i = 1; i <= 12; i++) 
	{
		product_House->look[0][i] = '#';
	}
	product_House->look[0][13] = ' ';
	for (int e = 1; e <= 2; e++) 
	{
		product_House->look[e][0] = ' ';
		product_House->look[e][1] = '#';
		product_House->look[e][12] = '#';
		for (int i = 2; i < 12; i++) 
		{
			product_House->look[e][i] = ' ';
		}
		product_House->look[e][13] = ' ';
	}
}
void HouseBuilderA::Conswindow() 
{
	product_House->look[3][0] = ' ';
	for (int i = 1; i <= 12; i++)
	{
		product_House->look[3][i] = '#';
	}
	product_House->look[3][13] = ' ';
	product_House->look[4][0] = ' ';
	product_House->look[4][1] = '#';
	product_House->look[4][12] = '#';
	for (int i = 2; i < 12; i++) 
	{
		product_House->look[4][i] = ' ';
	}
	product_House->look[4][13] = ' ';
}

House* HouseDirector::Cons(HouseBuilder& builder) 
{

	builder.Constube();
	builder.Consroof();
	builder.Conswalls();
	builder.Conswindow();
	
	return builder.getHouse();
}
House* HouseBuilder::getHouse() { return product_House; }

