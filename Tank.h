#pragma once

#include <stdint.h>

#include "DestroyableGroundObject.h"

class Tank : public DestroyableGroundObject
{
public:
	Tank() {}

	Tank(const Tank& _tank)
	{
		x = _tank.x;
		y = _tank.y;
		width = _tank.width;
	}

	bool  isInside(double x1, double x2) const override;

	inline uint16_t GetScore() const override { return score; }

	void Draw() const override;

	Tank* clone() const
	{
		return new Tank(*this);
	}

private:

	const uint16_t score = 30;
};
