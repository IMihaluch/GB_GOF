#include "MyTools.h"
#include "SBomber.h"
#include "Bomb.h"
#include "Ground.h"
#include "Tank.h"
#include "House.h"
#include "ScreenSingleton.h"
#include "enums/CraterSize.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>

int _kbhit()
{
    static const int STDIN = 0;
    static bool initialized = false;

    if (!initialized)
    {
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

class IFactory
{
public:
    virtual ~IFactory() {}

    DynamicObject* createPlane() const
    {
        Plane *p = createPlaneInstance();
        p->SetDirection(1, 0.1);
        p->SetSpeed(4);
        p->SetPos(5, 10);
        return p;
    }

    DynamicObject* createBomb(const double x, const double y) const
    {
        Bomb *pBomb = createBombInstance();
        pBomb->SetDirection(0.3, 1);
        pBomb->SetSpeed(2);
        pBomb->SetPos(x, y);
        pBomb->SetWidth(SMALL_CRATER_SIZE);
        return pBomb;
    }
 
    GameObject* createUI(const uint64_t passedTime, const uint64_t fps, const uint16_t bombsNumber, const int16_t score) const
    {
        LevelGUI *pGUI = createUIInstance();
        pGUI->SetParam(passedTime, fps, bombsNumber, score);
        const uint16_t maxX = ScreenSingleton::getInstance().GetMaxX();
        const uint16_t maxY = ScreenSingleton::getInstance().GetMaxY();
        const uint16_t offset = 3;
        const uint16_t width = maxX - 7;
        pGUI->SetPos(offset, offset);
        pGUI->SetWidth(width);
        pGUI->SetHeight(maxY - 4);
        pGUI->SetFinishX(offset + width - 4);
        return pGUI;
    }

     virtual GameObject* createHouse(double posX) const
    {
        const uint16_t maxY = ScreenSingleton::getInstance().GetMaxY();
        const uint16_t groundY = maxY - 5;
        House *pHouse = createHouseInstance();
        pHouse->SetWidth(13);
        pHouse->SetPos(posX, groundY - 1);
        return pHouse;
    }
 
    virtual GameObject* createGround() const
    {
        const uint16_t maxX = ScreenSingleton::getInstance().GetMaxX();
        const uint16_t maxY = ScreenSingleton::getInstance().GetMaxY();
        const uint16_t offset = 3;
        const uint16_t width = maxX - 7;
        Ground *pGr = createGroundInstance();
        const uint16_t groundY = maxY - 5;
        pGr->SetPos(offset + 1, groundY);
        pGr->SetWidth(width - 2);
        return pGr;
    }

    virtual GameObject* createTank(double posX) const
    {
        const uint16_t maxY = ScreenSingleton::getInstance().GetMaxY();
        const uint16_t groundY = maxY - 5;
        Tank *pTank = createTankInstance();
        pTank->SetWidth(13);
        pTank->SetPos(posX, groundY - 1);
        return pTank;
    }
private:
    virtual Plane* createPlaneInstance() const = 0;
    virtual Bomb* createBombInstance() const = 0;
    virtual LevelGUI* createUIInstance() const = 0;
    virtual Ground* createGroundInstance() const = 0;
    virtual Tank* createTankInstance() const = 0;
    virtual House* createHouseInstance() const = 0;
};

class RegularFactory : public IFactory
{
    Plane* createPlaneInstance() const override
    {
        return new Plane;
    }

    Bomb* createBombInstance() const override
    {
        return new Bomb;
    }

    LevelGUI* createUIInstance() const override
    {
        return new LevelGUI;
    }
    Ground* createGroundInstance() const override
    {
        return new Ground;
    }

    Tank* createTankInstance() const override
    {
        return new Tank;
    }
  
    House* createHouseInstance() const override
    {
        return new House;
    }
  };

class Command
{
public:
    virtual ~Command() {}
    virtual void Execute() = 0;
};

template <class Object>
class DeleteCommand : public Command
{
public:
    DeleteCommand(std::vector<Object*>& vecObj) : m_vecObj(vecObj), m_pObj(nullptr) {}
    void setObj(Object *pObj)
    {
        m_pObj = pObj;
    }
    void Execute()
    {
        if (!m_pObj)
        {
            return;
        }

        auto it = m_vecObj.begin();
        for (; it != m_vecObj.end(); it++)
        {
            if (*it == m_pObj)
            {
                m_vecObj.erase(it);
                break;
            }
        }
    }
private:
    std::vector<Object*>& m_vecObj;
    Object *m_pObj;
};

    class DropCommand : public Command
{
public:
    DropCommand(std::vector<DynamicObject*>& vecDynamicObj) : m_vecDynamicObj(vecDynamicObj), m_pPlane(nullptr), m_bombsNumber(nullptr), m_score(nullptr) {}
    void setParams(Plane *pPlane, uint16_t *bombsNumber, int16_t *score)
    {
        m_pPlane = pPlane;
        m_bombsNumber = bombsNumber;
        m_score = score;
    }
    void Execute()
    {
        if (*m_bombsNumber > 0)
        {
            MyTools::LoggerSingleton::getInstance().WriteToLog(std::string(__func__) + " was invoked");

            double x = m_pPlane->GetX() + 4;
            double y = m_pPlane->GetY() + 2;

            auto pFactory = new RegularFactory;
            m_vecDynamicObj.push_back(pFactory->createBomb(x, y));
            delete pFactory;

            (*m_bombsNumber)--;
            *m_score -= Bomb::BombCost;
        }
    }
private:
    std::vector<DynamicObject*>& m_vecDynamicObj;
    Plane *m_pPlane;
    uint16_t *m_bombsNumber;
    int16_t *m_score;
};

class DropBigCommand : public Command
{
public:
    DropBigCommand(std::vector<DynamicObject*>& vecDynamicObj) : m_vecDynamicObj(vecDynamicObj), m_pPlane(nullptr), m_bombsNumber(nullptr), m_score(nullptr) {}
    void setParams(Plane *pPlane, uint16_t *bombsNumber, int16_t *score)
    {
        m_pPlane = pPlane;
        m_bombsNumber = bombsNumber;
        m_score = score;
    }
    void Execute()
    {
        if (*m_bombsNumber > 0)
        {
            MyTools::LoggerSingleton::getInstance().WriteToLog(std::string(__func__) + " was invoked");

            double x = m_pPlane->GetX() + 4;
            double y = m_pPlane->GetY() + 2;

            auto pFactory = new RegularFactory;
            m_vecDynamicObj.push_back(new BombDecorator(pFactory->createBomb(x, y)));
            delete pFactory;

            (*m_bombsNumber)--;
            *m_score -= Bomb::BombCost;
        }
    }
private:
    std::vector<DynamicObject*>& m_vecDynamicObj;
    Plane *m_pPlane;
    uint16_t *m_bombsNumber;
    int16_t *m_score;
};

SBomber::SBomber() : exitFlag(false), startTime(0), finishTime(0), deltaTime(0), passedTime(0), fps(0), bombsNumber(10), score(0)
{
    MyTools::LoggerSingleton::getInstance().WriteToLog(std::string(__func__) + " was invoked");

    auto pFactory = new RegularFactory;

    vecDynamicObj.push_back(pFactory->createPlane());
    vecStaticObj.push_back(pFactory->createUI(passedTime, fps, bombsNumber, score));
    vecStaticObj.push_back(pFactory->createGround());
    vecStaticObj.push_back(pFactory->createTank(30));
    vecStaticObj.push_back(pFactory->createTank(50));
    vecStaticObj.push_back(pFactory->createHouse(80));

    delete pFactory;
}

SBomber::~SBomber()
{
    for (size_t i = 0; i < vecDynamicObj.size(); i++)
    {
        if (vecDynamicObj[i] != nullptr)
        {
            delete vecDynamicObj[i];
        }
    }

    for (size_t i = 0; i < vecStaticObj.size(); i++)
    {
        if (vecStaticObj[i] != nullptr)
        {
            delete vecStaticObj[i];
        }
    }
}

void SBomber::MoveObjects()
{
    MyTools::LoggerSingleton::getInstance().WriteToLog(std::string(__func__) + " was invoked");

     for (size_t i = 0; i < vecDynamicObj.size(); i++)
    {
        if (vecDynamicObj[i] != nullptr)
        {
            vecDynamicObj[i]->Move(deltaTime);
        }
    }
}
;

void SBomber::CheckObjects()
{
    MyTools::LoggerSingleton::getInstance().WriteToLog(std::string(__func__) + " was invoked");

    CheckPlaneAndLevelGUI();
    CheckBombsAndGround();
}
;

void SBomber::CheckPlaneAndLevelGUI()
{
    if (FindPlane()->GetX() > FindLevelGUI()->GetFinishX())
    {
        exitFlag = true;
    }
}


void SBomber::CheckBombsAndGround()
{
    std::vector<Bomb*> vecBombs = FindAllBombs();
    Ground *pGround = FindGround();
    const double y = pGround->GetY();
    for (size_t i = 0; i < vecBombs.size(); i++)
    {
        if (vecBombs[i]->GetY() >= y)
        {
            pGround->AddCrater(vecBombs[i]->GetX());
            CheckDestoyableObjects(vecBombs[i]);

            DeleteCommand<DynamicObject> delCom(SBomber::vecDynamicObj);
            delCom.setObj(vecBombs[i]);
            delCom.Execute();
        }
    }
}

void SBomber::CheckDestoyableObjects(Bomb *pBomb)
{
    std::vector<DestroyableGroundObject*> vecDestoyableObjects = FindDestoyableGroundObjects();
    const double size = pBomb->GetWidth();
    const double size_2 = size / 2;
    for (size_t i = 0; i < vecDestoyableObjects.size(); i++)
    {
        const double x1 = pBomb->GetX() - size_2;
        const double x2 = x1 + size;
        if (vecDestoyableObjects[i]->isInside(x1, x2))
        {
            score += vecDestoyableObjects[i]->GetScore();

            DeleteCommand<GameObject> delCom(SBomber::vecStaticObj);
            delCom.setObj(vecDestoyableObjects[i]);
            delCom.Execute();
        }
    }
}

std::vector<DestroyableGroundObject*> SBomber::FindDestoyableGroundObjects() const
{
    std::vector<DestroyableGroundObject*> vec;
    Tank *pTank;
    House *pHouse;
    for (size_t i = 0; i < vecStaticObj.size(); i++)
    {
        pTank = dynamic_cast<Tank*>(vecStaticObj[i]);
        if (pTank != nullptr)
        {
            vec.push_back(pTank);
            continue;
        }

        pHouse = dynamic_cast<House*>(vecStaticObj[i]);
        if (pHouse != nullptr)
        {
            vec.push_back(pHouse);
            continue;
        }
    }

    return vec;
}

Ground* SBomber::FindGround() const
{
    Ground *pGround;

    for (size_t i = 0; i < vecStaticObj.size(); i++)
    {
        pGround = dynamic_cast<Ground*>(vecStaticObj[i]);
        if (pGround != nullptr)
        {
            return pGround;
        }
    }

      return nullptr;
}

 std::vector<Bomb*> SBomber::FindAllBombs() const
{
    std::vector<Bomb*> vecBombs;

    for (size_t i = 0; i < vecDynamicObj.size(); i++)
    {
        Bomb *pBomb = dynamic_cast<Bomb*>(vecDynamicObj[i]);
        if (pBomb != nullptr)
        {
            vecBombs.push_back(pBomb);
        }
    }

     return vecBombs;
}

    Plane* SBomber::FindPlane() const
{
    for (size_t i = 0; i < vecDynamicObj.size(); i++)
    {
        Plane *p = dynamic_cast<Plane*>(vecDynamicObj[i]);
        if (p != nullptr)
        {
            return p;
        }
    }

     return nullptr;
}

    LevelGUI* SBomber::FindLevelGUI() const
{
    for (size_t i = 0; i < vecStaticObj.size(); i++)
    {
        LevelGUI *p = dynamic_cast<LevelGUI*>(vecStaticObj[i]);
        if (p != nullptr)
        {
            return p;
        }
    }

      return nullptr;
}

void SBomber::ProcessKBHit()
{
    int c = getchar();

    if (c == 224)
    {
        c = getchar();
    }

    MyTools::LoggerSingleton::getInstance().WriteToLog(std::string(__func__) + " was invoked. key = ", c);

    switch (c)
    {
        case 27: // esc
            exitFlag = true;
            break;
        case 72: // up
            FindPlane()->ChangePlaneY(-0.25);
            break;
        case 80: // down
            FindPlane()->ChangePlaneY(0.25);
            break;
        case 'b':
        {
            DropCommand dropCom(SBomber::vecDynamicObj);
            dropCom.setParams(FindPlane(), &bombsNumber, &score);
            dropCom.Execute();
            break;
        }
        case 'B':
        {
            DropBigCommand dropCom(SBomber::vecDynamicObj);
            dropCom.setParams(FindPlane(), &bombsNumber, &score);
            dropCom.Execute();
            break;
        }
        default:
            break;
    }
}

void SBomber::DrawFrame()
{
    MyTools::LoggerSingleton::getInstance().WriteToLog(std::string(__func__) + " was invoked");

     for (size_t i = 0; i < vecDynamicObj.size(); i++)
    {
        if (vecDynamicObj[i] != nullptr)
        {
            vecDynamicObj[i]->Draw();
        }
    }

  for (size_t i = 0; i < vecStaticObj.size(); i++)
    {
        if (vecStaticObj[i] != nullptr)
        {
            vecStaticObj[i]->Draw();
        }
    }

  ScreenSingleton::getInstance().GotoXY(0, 0);
    fps++;

  FindLevelGUI()->SetParam(passedTime, fps, bombsNumber, score);
}

void SBomber::TimeStart()
{
    MyTools::LoggerSingleton::getInstance().WriteToLog(std::string(__func__) + " was invoked");
    startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

void SBomber::TimeFinish()
{
    finishTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    deltaTime = uint16_t(finishTime - startTime);
    passedTime += deltaTime;

  MyTools::WriteToLog(std::string(__func__) + " deltaTime = ", (int)deltaTime);
    MyTools::LoggerSingleton::getInstance().WriteToLog(std::string(__func__) + " deltaTime = ", (int) deltaTime);
}

void SBomber::run()
{
    do
    {
        TimeStart();

        if (_kbhit())
        {
            ProcessKBHit();
        }

        ScreenSingleton::getInstance().ClrScr();

        DrawFrame();
        MoveObjects();
        CheckObjects();

        TimeFinish();

        } while (!GetExitFlag());
}
