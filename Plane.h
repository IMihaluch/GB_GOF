#pragma once

#include "DynamicObject.h"
class Plane : public DynamicObject 
{
public:

    void Draw() const override;
    virtual void Color() const;
    virtual void DrawBody() const;
    virtual void DrawWings() const;
    virtual void DrawTail() const;

    inline void ChangePlaneY(double dy) { yDirection += dy; }

private:

};

class ColorPlane : public Plane 
{
public:
    void Color() const override;


};

class BigPlane : public Plane 
{
    void DrawBody() const;
    void DrawWings() const;
    void DrawTail() const;
};
