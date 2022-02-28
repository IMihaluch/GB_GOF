#pragma once

#include <vector>
#include <string>
#include <deque>
#include "GameObject.h"

class Tank;

struct Tank_mess
{
	Tank* pr_Tank;
	std::deque <std::string> messages;
};

class Mediator 

{
	Mediator();
	Mediator(const Mediator&) = delete;
	Mediator& operator = (const Mediator&) = delete;
	std::vector<Tank_mess> tank_mess;
  
public:
	void Tank_add(Tank*);
	void Tank_dell(Tank*);
	void T_message(const std::string&, Tank*);
	std::vector <std::string>* Get_message();
	std::vector <double>* Get_pos_tank();
	static Mediator& getInstance();
};
