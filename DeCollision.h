#pragma once

#include <vector>

class DeCollision 
{
public:

    DeCollision(SBomber* p_SBomb) : p_SBomber(p_SBomb) {}
    DeCollision() = delete;
    void CheckPlaneAndLevelGUI();
    void CheckBombsAndGround();
    void CheckDestoyableObjects(Bomb* pBomb);
    
private:

    SBomber* p_SBomber;
};
