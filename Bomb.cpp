#include "Bomb.h"
#include <iostream>
#include "ScreenSingleton.h"

void Bomb::Draw() const 
{
//ScreenSingleton::getInstance().SetColor(CC_LightMagenta);
  ScreenSingleton::getInstance().GotoXY(x, y);
  std::cout << "*";
}

void BombDecorator::Move(uint16_t time)
{
    m_bomb->Move(time * 1.6);
}

void BombDecorator::Draw() const
{
    m_bomb->Draw();
    //изменение внешнего вида бомбы
    ScreenSingleton::getInstance().GotoXY(m_bomb->GetX(), m_bomb->GetY() - 1);
    std::cout << "|";
}

void BombDecorator::SetPos(double nx, double ny)
{
    m_bomb->SetPos(nx, ny);
}

uint16_t BombDecorator::GetWidth() const
{
    return m_bomb->GetWidth();
}
