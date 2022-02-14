#include "SBomber.h"
#include "MyTools.h"

int main(void)
{
    MyTools::LoggerSingleton::getInstance().OpenLogFile("log.txt");

    SBomber game;
    game.run();

   MyTools::LoggerSingleton::getInstance().CloseLogFile();

    return 0;
}
