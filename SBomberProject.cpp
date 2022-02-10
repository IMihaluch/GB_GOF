
#include <conio.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>	
#include <unistd.h>	
#include <fcntl.h>	


#include "SBomber.h"
#include "MyTools.h"

using namespace std;

//========================================================================================================================

int main(void)
{
    MyTools::LoggerSingleton::getInstance().OpenLogFile("log.txt");

    SBomber game;

    do {
        game.TimeStart();

        if (_kbhit())
        {
            game.ProcessKBHit();
        }

        MyTools::ClrScr();

        game.DrawFrame();
        game.MoveObjects();
        game.CheckObjects();

        game.TimeFinish();

    } while (!game.GetExitFlag());

     MyTools::LoggerSingleton::getInstance().CloseLogFile();

    return 0;
}
