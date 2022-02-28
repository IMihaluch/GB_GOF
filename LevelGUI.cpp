#include <iostream>

#include <vector>
#include "LevelGUI.h"
#include "MyTools.h"

using namespace std;
using namespace MyTools;
void LevelGUI::Draw() const
{
    ScreenSingleton::getInstance().SetColor(CC_White);

    static vector <string>* T_message = Mediator::getInstance().Get_message();;
    static vector <double>* T_position = Mediator::getInstance().Get_pos_tank();;
    ScreenSingleton::getInstance().GotoXY(x, y);
    char* buf = new (nothrow) char[width + 1];
    if (buf == nullptr)
    {
        return;
    }
    memset(buf, '+', width);
    buf[width] = '\0';
    cout << buf;
    ScreenSingleton::getInstance().GotoXY(x, y + height);
    cout << buf;
    delete[] buf;
    buf = nullptr;

    for (size_t i = size_t(y); i < height + y; i++)
    {
        ScreenSingleton::getInstance().GotoXY(x, (double)i);
        cout << "+";
        ScreenSingleton::getInstance().GotoXY(x + width - 1, (double)i);
        cout << "+";
    }
    ScreenSingleton::getInstance().GotoXY(3, 1);
    cout << "FramePerSecond: " << static_cast<int>(fps / (passedTime / 1000.0));
    ScreenSingleton::getInstance().GotoXY(25, 1);
    cout << "PassedTime: " << static_cast<int>(passedTime / 1000.0) << " sec";
    ScreenSingleton::getInstance().GotoXY(46, 1);
    cout << "BombsNum: " << bombsNumber;
    ScreenSingleton::getInstance().GotoXY(62, 1);
    cout << "Score: " << score;
    static int i = (passedTime / 1000.0);
    if (i + 3 < (passedTime / 1000.0))
 {
        T_message = Mediator::getInstance().Get_message();
        T_position = Mediator::getInstance().Get_pos_tank();
    }
    for (int i = 0; i < T_message->size(); i++) {
        double pos = (*T_position)[i];
        ScreenSingleton::getInstance().GotoXY(pos, 25);
        cout << (*T_message)[i];
    }

}

void __fastcall LevelGUI::SetParam(uint64_t passedTimeNew, uint64_t fpsNew, uint16_t bombsNumberNew, int16_t scoreNew)
{
    passedTime = passedTimeNew;
    fps = fpsNew;
    bombsNumber = bombsNumberNew;
    score = scoreNew;
}
