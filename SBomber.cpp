#include <conio.h>
#include <iostream>

#include "MyTools.h"
#include "SBomber.h"
#include "Bomb.h"
#include "Ground.h"
#include "Tank.h"
#include "House.h"
#include "HouseBuilder.h"

using namespace std;
using namespace MyTools;

SBomber::SBomber() 
    : exitFlag(false),
    startTime(0),
    finishTime(0),
    deltaTime(0),
    passedTime(0),
    fps(0),
    bombsNumber(10),
    
    score(0),
    verification(this)
        
    {
        cout << "What kind of house do you need?" << endl;
        component comp;
        int kind;
        cin >> kind;
        static_cast <component> (kind);
        FileLoggerSingletone::getInstance().WriteToLog(string(__FUNCTION__) + " was invoked");
        Plane* p = new Plane;
        p->SetDirection(1, 0.1);
        p->SetSpeed(4);
        p->SetPos(5, 10);
        vecDynamicObj.push_back(p);
        LevelGUI* pGUI = new LevelGUI;
        pGUI->SetParam(passedTime, fps, bombsNumber, score);
        const uint16_t maxX = ScreenSingleton::getInstance().GetMaxX();
        const uint16_t maxY = ScreenSingleton::getInstance().GetMaxY();
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
        Tank* pTank = new Tank;
        pTank->SetWidth(13);
        pTank->SetPos(30, groundY - 1);
        vecStaticObj.push_back(pTank);
        pTank = new Tank;
        pTank->SetWidth(13);
        pTank->SetPos(50, groundY - 1);
        vecStaticObj.push_back(pTank);

        HouseBuilderA builder;
        HouseDirector director;
        House* product_House = director.Cons(builder);
        product_House->SetWidth(13);
        product_House->SetPos(80, groundY - 1);
        vecStaticObj.push_back(product_House);

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
        FileLoggerSingletone::getInstance().WriteToLog(string(__FUNCTION__) + " was invoked");
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
        FileLoggerSingletone::getInstance().WriteToLog(string(__FUNCTION__) + " was invoked");

        verification.CheckPlaneAndLevelGUI();
        verification.CheckBombsAndGround();
    };

   
    void SBomber::DeleteDynamicObj(DynamicObject * pObj)
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
    void SBomber::DeleteStaticObj(GameObject * pObj)
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
    vector<DestroyableGroundObject*> SBomber::FindDestoyableGroundObjects() const
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
    Ground* SBomber::FindGround() const
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
    vector<Bomb*> SBomber::FindAllBombs() const
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
        FileLoggerSingletone::getInstance().WriteToLog(string(__FUNCTION__) + " was invoked. key = ", c);
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
        FileLoggerSingletone::getInstance().WriteToLog(string(__FUNCTION__) + " was invoked");
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
        FileLoggerSingletone::getInstance().WriteToLog(string(__FUNCTION__) + " was invoked");
        startTime = GetTickCount64();
    }
    void SBomber::TimeFinish()
    {
        finishTime = GetTickCount64();
        deltaTime = uint16_t(finishTime - startTime);
        passedTime += deltaTime;
        FileLoggerSingletone::getInstance().WriteToLog(string(__FUNCTION__) + " deltaTime = ", (int)deltaTime);
    }
    void SBomber::DropBomb()
    {
        if (bombsNumber > 0)
        {
            FileLoggerSingletone::getInstance().WriteToLog(string(__FUNCTION__) + " was invoked");
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

    void CollisionDetector::CheckPlaneAndLevelGUI() {
        if (p_SBomber->FindPlane()->GetX() > p_SBomber->FindLevelGUI()->GetFinishX()) {
            p_SBomber->exitFlag = true;
        }
        return;
    }
    void CollisionDetector::CheckBombsAndGround() {
        vector<Bomb*> vecBombs = p_SBomber->FindAllBombs();
        Ground* pGround = p_SBomber->FindGround();
        const double y = pGround->GetY();
        for (size_t i = 0; i < vecBombs.size(); i++)
        {
            if (vecBombs[i]->GetY() >= y) 
            {
                pGround->AddCrater(vecBombs[i]->GetX());
                CheckDestoyableObjects(vecBombs[i]);
                p_SBomber->DeleteDynamicObj(vecBombs[i]);
            }
        }
    }
    
    void CollisionDetector::CheckDestoyableObjects(Bomb * pBomb) {
        vector<DestroyableGroundObject*> vecDestoyableObjects = p_SBomber->FindDestoyableGroundObjects();
        const double size = pBomb->GetWidth();
        const double size_2 = size / 2;
        for (size_t i = 0; i < vecDestoyableObjects.size(); i++)
        {
            const double x1 = pBomb->GetX() - size_2;
            const double x2 = x1 + size;
            if (vecDestoyableObjects[i]->isInside(x1, x2))
            {
                p_SBomber->score += vecDestoyableObjects[i]->GetScore();
                p_SBomber->DeleteStaticObj(vecDestoyableObjects[i]);
            }
        }
    }
    
