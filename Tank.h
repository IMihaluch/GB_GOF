#pragma once

#include <string>
#include <stdint.h>
#include "DestroyableGroundObject.h"
#include "Mediator.h"

class Tank : public DestroyableGroundObject
{
public:
	bool __fastcall isInside(double x1, double x2) const override;

	inline uint16_t GetScore() const override { return score; }

	void send_mes();

	void Draw() const override;
	Tank();
	~Tank();

private:

	Mediator& mediator;
	const uint16_t score = 30;
	std::string messages[4] = { "You'll regret this!", "You are in my sights!",
	"Get out rusty can!", "Your bombs are powerless against my armor!" };
};
