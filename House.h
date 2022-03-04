#pragma once

#include "DestroyableGroundObject.h"
#include <cstdint>


    class House : public DestroyableGroundObject
    {
    public:
        
        House() {}

        House(const House& _house)
        {
            x = _house.x;
            y = _house.y;
            width = _house.width;
        }

        bool isInside(double x1, double x2) const override;

        inline uint16_t GetScore() const override
        {
            return score;
        }

        void Draw() const override;

        House* clone() const
        {
            return new House(*this);
        }

    private:
        const uint16_t score = 40;
    };
