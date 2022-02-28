#include <iostream>
#include "Plane.h"
#include "MyTools.h"
using namespace std;
using namespace MyTools;

void Plane::Draw() const
{
    Color();
    DrawBody();
    DrawTail();
    DrawWings();

    /*   ScreenSingleton::getInstance().GotoXY(x, y);
       cout << "=========>";
       ScreenSingleton::getInstance().GotoXY(x - 2, y - 1);
       cout << "===";
       ScreenSingleton::getInstance().GotoXY(x + 3, y - 1);
       cout << "\\\\\\\\";
       ScreenSingleton::getInstance().GotoXY(x + 3, y + 1);
       cout << "////";
   */
}
void Plane::Color() const 
{
    ScreenSingleton::getInstance().SetColor(CC_LightBlue);
}
void Plane::DrawBody() const
{
    ScreenSingleton::getInstance().GotoXY(x, y);
    cout << "=========>";
}
void Plane::DrawTail() const
{
    ScreenSingleton::getInstance().GotoXY(x - 2, y - 1);
    cout << "===";
}
void Plane::DrawWings() const
{
    ScreenSingleton::getInstance().GotoXY(x + 3, y - 1);
    cout << "\\\\\\\\";
    ScreenSingleton::getInstance().GotoXY(x + 3, y + 1);
    cout << "////";
}
void ColorPlane::Color() const 
{
    ScreenSingleton::getInstance().SetColor(CC_Brown);
}
void BigPlane::DrawBody() const 
{
    ScreenSingleton::getInstance().GotoXY(x, y);
    cout << "===============>";
}
void BigPlane::DrawTail() const 
{
    ScreenSingleton::getInstance().GotoXY(x - 2, y - 1);
    cout << "======";
}
void BigPlane::DrawWings() const 
{
    ScreenSingleton::getInstance().GotoXY(x + 3, y - 1);
    cout << "\\\\\\\\\\\\\\";
    ScreenSingleton::getInstance().GotoXY(x + 3, y + 1);
    cout << "////////";
}
