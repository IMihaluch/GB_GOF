#pragma once

#include "DynamicObject.h"
#include "Visitor.h"

class Plane : public DynamicObject 
{
public:

    void Draw() const override;

    inline void ChangePlaneY(double dy) { yDirection += dy; }

    void __fastcall Accept(Visitor* v)
    {
        v->log(this);
    }

private:

};
