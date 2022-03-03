#include <conio.h>
#include <windows.h>
#include <string>
#include "MyTools.h"
#include "SBomber.h"
#include "Bomb.h"
#include "Ground.h"
#include "Tank.h"
#include "House.h"

extern MyTools::FileLogger logger;

using namespace std;
using namespace MyTools;

class SBomber::SBomberImpl 
{
public:
    void CheckPlaneAndLevelGUI();
    void CheckBombsAndGround();
    void __fastcall CheckDestoyableObjects(Bomb* pBomb);

    void __fastcall DeleteDynamicObj(DynamicObject* pBomb);
    void __fastcall DeleteStaticObj(GameObject* pObj);

    Ground* FindGround() const;
    Plane* FindPlane() const;
    LevelGUI* FindLevelGUI() const;
    std::vector<DestroyableGroundObject*> FindDestoyableGroundObjects() const;
    std::vector<Bomb*> FindAllBombs() const;

    void DropBomb();

    std::vector<DynamicObject*> vecDynamicObj;
    std::vector<GameObject*> vecStaticObj;

    bool exitFlag;

    uint64_t startTime, finishTime, passedTime;
    uint16_t bombsNumber, deltaTime, fps;
    int16_t score;
};

SBomber::SBomber()
    : PIMPL(new SBomberImpl)
{   
    PIMPL->exitFlag = false;
    PIMPL->startTime = 0;
    PIMPL->finishTime = 0;
    PIMPL->deltaTime = 0;
    PIMPL->passedTime = 0;
    PIMPL->fps = 0;
    PIMPL->bombsNumber = 10;
    PIMPL->score = 0;
    logger.WriteToLog(string(__FUNCTION__) + " was invoked");

    Plane* p = new Plane;
    p->SetDirection(1, 0.1);
    p->SetSpeed(4);
    p->SetPos(5, 10);
    PIMPL->vecDynamicObj.push_back(p);

    LevelGUI* pGUI = new LevelGUI;
    pGUI->SetParam(PIMPL->passedTime, PIMPL->fps, PIMPL->bombsNumber, PIMPL->score);
    const uint16_t maxX = GetMaxX();
    const uint16_t maxY = GetMaxY();
    const uint16_t offset = 3;
    const uint16_t width = maxX - 7;
    pGUI->SetPos(offset, offset);
    pGUI->SetWidth(width);
    pGUI->SetHeight(maxY - 4);
    pGUI->SetFinishX(offset + width - 4);
    PIMPL->vecStaticObj.push_back(pGUI);

    Ground* pGr = new Ground;
    const uint16_t groundY = maxY - 5;
    pGr->SetPos(offset + 1, groundY);
    pGr->SetWidth(width - 2);
    PIMPL->vecStaticObj.push_back(pGr);

    Tank* pTank = new Tank;
    pTank->SetWidth(13);
    pTank->SetPos(30, groundY - 1);
    PIMPL->vecStaticObj.push_back(pTank);

    pTank = new Tank;
    pTank->SetWidth(13);
    pTank->SetPos(50, groundY - 1);
    PIMPL->vecStaticObj.push_back(pTank);

    House* pHouse = new House;
    pHouse->SetWidth(13);
    pHouse->SetPos(80, groundY - 1);
    PIMPL->vecStaticObj.push_back(pHouse);

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
    
        for (size_t i = 0; i < PIMPL->vecDynamicObj.size(); i++)
        {
            
                if (PIMPL->vecDynamicObj[i] != nullptr)
                {
                    delete PIMPL->vecDynamicObj[i];
                }
        }

        for (size_t i = 0; i < PIMPL->vecStaticObj.size(); i++)
        {
            
                if (PIMPL->vecStaticObj[i] != nullptr)
                {
                    delete PIMPL->vecStaticObj[i];
                }
        }
}

void SBomber::MoveObjects()
{
    logger.WriteToLog(string(__FUNCTION__) + " was invoked");

        for (size_t i = 0; i < PIMPL->vecDynamicObj.size(); i++)
        {
               if (PIMPL->vecDynamicObj[i] != nullptr)
                {
                    PIMPL->vecDynamicObj[i]->Move(PIMPL->deltaTime);
                }
        }
};

void SBomber::CheckObjects()
{
    logger.WriteToLog(string(__FUNCTION__) + " was invoked");

    PIMPL->CheckPlaneAndLevelGUI();
    PIMPL->CheckBombsAndGround();
};

void SBomber::SBomberImpl::CheckPlaneAndLevelGUI()
{
    if (FindPlane()->GetX() > FindLevelGUI()->GetFinishX())
    {
        exitFlag = true;
    }
}


void SBomber::SBomberImpl::CheckBombsAndGround()
{
    vector<Bomb*> vecBombs = FindAllBombs();
    Ground* pGround = FindGround();
    const double y = pGround->GetY();
    for (size_t i = 0; i < vecBombs.size(); i++)
    {
        if (vecBombs[i]->GetY() >= y) // Пересечение бомбы с землей
        {
            pGround->AddCrater(vecBombs[i]->GetX());
            CheckDestoyableObjects(vecBombs[i]);
            DeleteDynamicObj(vecBombs[i]);
        }
    }

}


void SBomber::SBomberImpl::CheckDestoyableObjects(Bomb* pBomb)
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

void SBomber::SBomberImpl::DeleteDynamicObj(DynamicObject* pObj)
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

void SBomber::SBomberImpl::DeleteStaticObj(GameObject* pObj)
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


vector<DestroyableGroundObject*> SBomber::SBomberImpl::FindDestoyableGroundObjects() const
{
    vector<DestroyableGroundObject*> vec;
    Tank* pTank;
    House* pHouse;
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

Ground* SBomber::SBomberImpl::FindGround() const
{
    Ground* pGround;

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

vector<Bomb*> SBomber::SBomberImpl::FindAllBombs() const
{
    vector<Bomb*> vecBombs;

    for (size_t i = 0; i < vecDynamicObj.size(); i++)
    {
        Bomb* pBomb = dynamic_cast<Bomb*>(vecDynamicObj[i]);
        if (pBomb != nullptr)
        {
            vecBombs.push_back(pBomb);
        }
    }
    return vecBombs;
}

Plane* SBomber::SBomberImpl::FindPlane() const
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

LevelGUI* SBomber::SBomberImpl::FindLevelGUI() const
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

    
    logger.WriteToLog(string(__FUNCTION__) + " was invoked. key = ", c);

    switch (c) 
    {

    case 27: // esc
        
        PIMPL->exitFlag = true;
        break;

    case 72: // up
        
        PIMPL->FindPlane()->ChangePlaneY(-0.25);
        break;

    case 80: // down
        
        PIMPL->FindPlane()->ChangePlaneY(0.25);
        break;

    case 'b':
       
        PIMPL->DropBomb();
        break;

    case 'B':
        
        PIMPL->DropBomb();
        break;

    default:
        break;
    }
}

void SBomber::DrawFrame()
{
   
    logger.WriteToLog(string(__FUNCTION__) + " was invoked");

   
        for (size_t i = 0; i < PIMPL->vecDynamicObj.size(); i++)
        {
           
                if (PIMPL->vecDynamicObj[i] != nullptr)
                {
                   
                    PIMPL->vecDynamicObj[i]->Draw();
                }
        }

   
        for (size_t i = 0; i < PIMPL->vecStaticObj.size(); i++)
        {
           
                if (PIMPL->vecStaticObj[i] != nullptr)
                {
                   
                    PIMPL->vecStaticObj[i]->Draw();
                }
        }

    GotoXY(0, 0);
   
    (PIMPL->fps)++;

    PIMPL->FindLevelGUI()->SetParam(PIMPL->passedTime, PIMPL->fps, PIMPL->bombsNumber, PIMPL->score);
}

void SBomber::TimeStart()
{
    
    logger.WriteToLog(string(__FUNCTION__) + " was invoked");
    PIMPL->startTime = GetTickCount64();
}

void SBomber::TimeFinish()
{
    PIMPL->finishTime = GetTickCount64();
    PIMPL->deltaTime = uint16_t(PIMPL->finishTime - PIMPL->startTime);
    PIMPL->passedTime += PIMPL->deltaTime;

    
    logger.WriteToLog(string(__FUNCTION__) + " deltaTime = ", (int)(PIMPL->deltaTime));
}


void SBomber::SBomberImpl::DropBomb()
{
    if (bombsNumber > 0)
    {
       
        logger.WriteToLog(string(__FUNCTION__) + " was invoked");

        Plane* PIMPL = FindPlane();
        double x = PIMPL->GetX() + 4;
        double y = PIMPL->GetY() + 2;
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
bool SBomber::GetExitFlag() 
const { return (PIMPL->exitFlag); }
