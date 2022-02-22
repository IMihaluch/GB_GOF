#pragma once

#include <vector>

#include "LevelGUI.h"
#include "Plane.h"
#include "Bomb.h"
#include "Ground.h"
#include "Tank.h"
#include "DeCollision.h"

class SBomber;

class CollisionDetector
{
public:
    CollisionDetector(SBomber* p_SBomb) : p_SBomber(p_SBomb) {}
    CollisionDetector() = delete;
    void CheckPlaneAndLevelGUI();
    void CheckBombsAndGround();
    void CheckDestoyableObjects(Bomb* pBomb);
private:
    SBomber* p_SBomber;
};

class SBomber
{
public:

    SBomber();
    ~SBomber();
    
    inline bool GetExitFlag() const { return exitFlag; }

    void ProcessKBHit();
    void TimeStart();
    void TimeFinish();

    void DrawFrame();
    void MoveObjects();
    void CheckObjects();

private:

    void CheckPlaneAndLevelGUI();
    void CheckBombsAndGround();
    void  CheckDestoyableObjects(Bomb* pBomb);

    void  DeleteDynamicObj(DynamicObject * pBomb);
    void  DeleteStaticObj(GameObject* pObj);

    Ground * FindGround() const;
    Plane * FindPlane() const;
    LevelGUI * FindLevelGUI() const;
    std::vector<DestroyableGroundObject*> FindDestoyableGroundObjects() const;
    std::vector<Bomb*> FindAllBombs() const;

    void DropBomb();

    std::vector<DynamicObject*> vecDynamicObj;
    std::vector<GameObject*> vecStaticObj;
    
    bool exitFlag;

    uint64_t startTime, finishTime, passedTime;
    uint16_t bombsNumber, deltaTime, fps;
    int16_t score;
    CollisionDetector verification;

friend class CollisionDetector;
};
