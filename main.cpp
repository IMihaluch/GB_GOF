#include "SBomber.h"
#include "MyTools.h"
#include "ScreenSingleton.h"

int main(void)
{
    MyTools::LoggerSingleton::getInstance().OpenLogFile("log.txt");

    SBomber game;
    game.run();

   MyTools::LoggerSingleton::getInstance().CloseLogFile();

    return 0;
}
