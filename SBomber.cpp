#include <conio.h>
#include <thread>
#include <chrono>

#include "MyTools.h"
#include "SBomber.h"
#include "Bomb.h"
#include "Ground.h"
#include "Tank.h"
#include "House.h"
#include "ScreenSingleton.h"
#include "enums/CraterSize.h"

using namespace std;
using namespace MyTools;

SBomber::SBomber():
    exitFlag(false),
    startTime(0),
    finishTime(0),
    deltaTime(0),
    passedTime(0),
    fps(0),
    bombsNumber(10),
    score(0)
{
    MyTools::LoggerSingleton::getInstance().WriteToLog(std::string(__FUNCTION__) + " was invoked");
    Plane* p = new Plane;
    p->SetDirection(1, 0.1);
    p->SetSpeed(4);
    p->SetPos(5, 10);
    vecDynamicObj.push_back(p);

    LevelGUI* pGUI = new LevelGUI;
    pGUI->SetParam(passedTime, fps, bombsNumber, score);
    const uint16_t maxX = GetMaxX();
    const uint16_t maxY = GetMaxY(); 
    const uint16_t offset = 3;
    const uint16_t width = maxX - 7;
    pGUI->SetPos(offset, offset);
    pGUI->SetWidth(width);
    pGUI->SetHeight(maxY - 4);
    pGUI->SetFinishX(offset + width - 4);
    vecStaticObj.push_back(pGUI);

    Ground* pGr = new Ground;
    const uint16_t groundY = maxY - 5;
    pGr->SetPos(offset + 1, groundY);
    pGr->SetWidth(width - 2);
    vecStaticObj.push_back(pGr);

    TankAdapter* pTank = new TankAdapter;
    pTank->SetWidth(13);
    pTank->SetPos(30, groundY - 1);
    vecStaticObj.push_back(pTank);

    pTank = new TankAdapter;
    pTank->SetWidth(13);
    pTank->SetPos(50, groundY - 1);
    vecStaticObj.push_back(pTank);

    House * pHouse = new House;
    pHouse->SetWidth(13);
    pHouse->SetPos(80, groundY - 1);
    vecStaticObj.push_back(pHouse);

    /*
    Bomb* pBomb = new Bomb;
    pBomb->SetDirection(0.3, 1);
    pBomb->SetSpeed(2);
    pBomb->SetPos(51, 5);
    pBomb->SetSize(SMALL_CRATER_SIZE);
    vecDynamicObj.push_back(pBomb);
    */
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
  MyTools::LoggerSingleton::getInstance().WriteToLog(std::string(__FUNCTION__) + " was invoked");

    for (size_t i = 0; i < vecDynamicObj.size(); i++)
    {
        if (vecDynamicObj[i] != nullptr)
        {
            vecDynamicObj[i]->Move(deltaTime);
        }
    }
};

void SBomber::CheckObjects()
{
    MyTools::LoggerSingleton::getInstance().WriteToLog(std::string(__FUNCTION__) + " was invoked");

    CheckPlaneAndLevelGUI();
    CheckBombsAndGround();
};

void SBomber::CheckPlaneAndLevelGUI()
{
    if (FindPlane()->GetX() > FindLevelGUI()->GetFinishX())
    {
        exitFlag = true;
    }
}

void SBomber::CheckBombsAndGround() 
{
    Ground* pGround = FindGround();
    const double y = pGround->GetY();
   for (auto itBomb = begin(); itBomb != end(); ++itBomb)
    {
       if ((*itBomb)->GetY() >= y)  // Пересечение бомбы с землей
        {
           pGround->AddCrater((*itBomb)->GetX());
            CheckDestoyableObjects(*itBomb);
            --itBomb;
            itBomb = erase(itBomb);
            DeleteDynamicObj(*itBomb);
        }
    }
}

void SBomber::CheckDestoyableObjects(Bomb * pBomb)
{
    vector<DestroyableGroundObject*> vecDestoyableObjects = FindDestoyableGroundObjects();
    const double size = pBomb->GetWidth();
    const double size_2 = size / 2;
    for (size_t i = 0; i < vecDestoyableObjects.size(); i++)
    {
        const double x1 = pBomb->GetX() - size_2;
        const double x2 = x1 + size;
        if (vecDestoyableObjects[i]->isInside(x1, x2))
        {
            score += vecDestoyableObjects[i]->GetScore();
            DeleteStaticObj(vecDestoyableObjects[i]);
        }
    }
}

void SBomber::DeleteDynamicObj(DynamicObject* pObj)
{
    auto it = vecDynamicObj.begin();
    for (; it != vecDynamicObj.end(); it++)
    {
        if (*it == pObj)
        {
            vecDynamicObj.erase(it);
            break;
        }
    }
}

void SBomber::DeleteStaticObj(GameObject* pObj)
{
    auto it = vecStaticObj.begin();
    for (; it != vecStaticObj.end(); it++)
    {
        if (*it == pObj)
        {
            vecStaticObj.erase(it);
            break;
        }
    }
}

std::vector<DestroyableGroundObject*> SBomber::FindDestoyableGroundObjects() const
{
    std::vector<DestroyableGroundObject*> vec;
    TankAdapter* pTank;
    House* pHouse;
    for (size_t i = 0; i < vecStaticObj.size(); i++)
    {
        pTank = dynamic_cast<TankAdapter*>(vecStaticObj[i]);
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
    Ground* pGround;

    for (size_t i = 0; i < vecStaticObj.size(); i++)
    {
        pGround = dynamic_cast<Ground *>(vecStaticObj[i]);
        if (pGround != nullptr)
        {
            return pGround;
        }
    }

    return nullptr;
}

Plane* SBomber::FindPlane() const
{
    for (size_t i = 0; i < vecDynamicObj.size(); i++)
    {
        Plane* p = dynamic_cast<Plane*>(vecDynamicObj[i]);
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
        LevelGUI* p = dynamic_cast<LevelGUI*>(vecStaticObj[i]);
        if (p != nullptr)
        {
            return p;
        }
    }

    return nullptr;
}

void SBomber::ProcessKBHit()
{
    int c = _getch();

    if (c == 224)
    {
        c = _getch();
    }
    MyTools::LoggerSingleton::getInstance().WriteToLog(std::string(__FUNCTION__) + " was invoked. key = ", c);

    switch (c) {

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
        DropBomb();
        break;

    case 'B':
        DropBomb();
        break;

    default:
        break;
    }
}

void SBomber::DrawFrame()
{
    MyTools::LoggerSingleton::getInstance().WriteToLog(std::string(__FUNCTION__) + " was invoked");

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
    MyTools::LoggerSingleton::getInstance().WriteToLog(std::string(__FUNCTION__) + " was invoked");

}

void SBomber::TimeFinish()
{
    finishTime = GetTickCount64();
    deltaTime = uint16_t(finishTime - startTime);
    passedTime += deltaTime;

    MyTools::LoggerSingleton::getInstance().WriteToLog(std::string(__FUNCTION__) + " deltaTime = ", (int)deltaTime);

}

void SBomber::DropBomb()
{
    if (bombsNumber > 0)
    {
        MyTools::LoggerSingleton::getInstance().WriteToLog(std::string(__FUNCTION__) + " was invoked");

        Plane* pPlane = FindPlane();
        double x = pPlane->GetX() + 4;
        double y = pPlane->GetY() + 2;

        Bomb* pBomb = new Bomb;
        pBomb->SetDirection(0.3, 1);
        pBomb->SetSpeed(2);
        pBomb->SetPos(x, y);
        pBomb->SetWidth(SMALL_CRATER_SIZE);

        vecDynamicObj.push_back(pBomb);
        bombsNumber--;
        score -= Bomb::BombCost;
    }
}

SBomber::BombIterator SBomber::begin()
{
    BombIterator it(vecDynamicObj);
    return it;
}

SBomber::BombIterator SBomber::end()
{
    BombIterator it(vecDynamicObj);
    it.reset();
    return it;
}

SBomber::BombIterator SBomber::erase(SBomber::BombIterator it)
{

    for (auto iterVec = vecDynamicObj.begin(); iterVec != vecDynamicObj.end(); iterVec++)
    {
        if (*iterVec == *it)
        {
            vecDynamicObj.erase(iterVec);
            break;
        }
    }

    return --it;
}

SBomber::BombIterator::BombIterator(std::vector<DynamicObject*>& dat) : VecItertor(dat),butt(nullptr), Index(-1) { ++(*this); }

void SBomber::BombIterator::reset()
 { 
Index = -1;
butt = nullptr;
 }

SBomber::BombIterator& SBomber::BombIterator::operator++()
{
    ++Index;

    if (Index == -1)
        Index = 0;

    for (; Index < VecItertor.size(); ++Index)
    {

        Bomb* pBomb = dynamic_cast<Bomb*>(VecItertor[Index]);
        if (pBomb != nullptr)
        {
           butt = &pBomb;
            break;
        }
    }

    if (Index == VecItertor.size())
    {
        Index = -1;
        butt = nullptr;
    }

    return *this;
}

SBomber::BombIterator& SBomber::BombIterator::operator--()
{
    --Index;

    if (Index == -1)
        Index = VecItertor.size() - 1;

    for (; Index >= 0; --Index)
    {
        Bomb* pBomb = dynamic_cast<Bomb*>(VecItertor[Index]);
        if (pBomb != nullptr)
        {
           butt = &pBomb;
           break;
        }
    }

    if (Index == -1)
       butt = nullptr;

    return *this;
}

Bomb* SBomber::BombIterator::operator* ()
{
    return dynamic_cast<Bomb*>(VecItertor.at(Index));
}

bool SBomber::BombIterator::operator== (BombIterator it)
{
         if (index == it.index &&butt == it.butt && VecItertor == it.VecItertor)
   {
      return true;
   }
   return false;
}

bool SBomber::BombIterator::operator!=(BombIterator it)
{
    return !(*this == it);
}

SBomber::BombIterator& SBomber::BombIterator::operator= (const BombIterator& it)
{

    VecItertor = it.VecItertor;
    Index = it.Index;
    butt = it.butt;

    return *this;
}
