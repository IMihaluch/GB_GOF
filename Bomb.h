#pragma once

#include "DynamicObject.h"

class Bomb : public DynamicObject
{
public:

	static const uint16_t BombCost = 10;

	void Draw() const override;

private:

};

class BombDecorator: public DynamicObject
{
public:
    BombDecorator(DynamicObject *bomb) : m_bomb(bomb)
    {
    }
    void Move(uint16_t time) override;
    void Draw() const override;

    void SetPos(double nx, double ny) override;
    uint16_t GetWidth() const override;
private:
    DynamicObject *m_bomb;
};
