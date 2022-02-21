#pragma once

#include <vector>


class DeCollision 
{
public:

    DeCollision(SBomber* p_SBomb) : pSBomber(pSBomb) {}
    DeCollision() = delete;
    void CheckPlaneAndLevelGUI();
    void CheckBombsAndGround();
    void CheckDestoyableObjects(Bomb* pBomb);
    
private:

    SBomber* p_SBomber;
};
