#pragma once

#include <vector>
#include "LevelGUI.h"
#include "Plane.h"
#include "Bomb.h"
#include "Ground.h"
#include "Tank.h"

class SBomber
{
public:
    SBomber();
    ~SBomber();

    inline bool GetExitFlag() const 
{ 
return exitFlag; 
}
    void ProcessKBHit();
    void TimeStart();
    void TimeFinish();
    void DrawFrame();
    void MoveObjects();
    void CheckObjects();

class BombIterator
    {

 public:

        BombIterator(std::vector<DynamicObject*>& dat);
        void reset();

        BombIterator& operator++();
        BombIterator& operator--();
        Bomb* operator* ();

        bool operator== (BombIterator it);
        bool operator!=(BombIterator it);

        BombIterator& operator= (const BombIterator& it);
        BombIterator begin();
        BombIterator end();

private:

        int Index;
        Bomb** butt;
        std::vector<DynamicObject*>& VecItertor;

    };

    BombIterator erase(BombIterator it);

private:

    void CheckPlaneAndLevelGUI();
    void CheckBombsAndGround();
    void CheckDestoyableObjects(Bomb* pBomb);
    void DeleteDynamicObj(DynamicObject * pBomb);
    void DeleteStaticObj(GameObject* pObj);
    Ground * FindGround() const;
    Plane * FindPlane() const;
    LevelGUI * FindLevelGUI() const;
    std::vector<DestroyableGroundObject*> FindDestoyableGroundObjects() const;


    void DropBomb();

    std::vector<DynamicObject*> vecDynamicObj;
    std::vector<GameObject*> vecStaticObj;

    bool exitFlag;
    uint64_t startTime, finishTime, passedTime;
    uint16_t bombsNumber, deltaTime, fps;
    int16_t score;
};
