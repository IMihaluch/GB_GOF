#include "Tank.h"


Mediator::Mediator() {}
Mediator& Mediator::getInstance()
{
	static Mediator m;
	return m;
}
void Mediator::Tank_add(Tank* p_tank) 
{
	T_messages messages;
	messages.pr_Tank = p_tank;
	t_messages.push_back(messages);
}
void Mediator::Tank_dell(Tank* p_tank)
{
	int i;
	for (i = 0; i < t_messages.size(); i++)
	{
		if (t_messages[i].pr_Tank == p_tank) break;
	}
	t_messages.erase(t_messages.begin() + i);
}
void Mediator::T_message(const std::string& mes, Tank* p_tank) 
{
	for (int i = 0; i < t_messages.size(); i++) 
	{
		if (t_messages[i].pr_Tank == p_tank) 
		{
			if (t_messages[i].messages.size() < 10) t_messages[i].messages.push_front(mes);
		}
	}
	return;
}
std::vector <std::string>* Mediator::Get_message() 
{
	std::vector <std::string>* mes = new std::vector <std::string>;
	for (int i = 0; i < t_messages.size(); i++) 
	{
		if (!t_messages[i].messages.empty()) 
		{
			mes->push_back(*(t_messages[i].messages.end() - 1));
			t_messages[i].messages.pop_back();
		}
	}
	return mes;
}

std::vector <double>* Mediator::Get_pos_tank()
{
	std::vector <double>* pos = new std::vector<double>;
	for (int i = 0; i < t_messages.size(); i++)
	{
		if (!t_messages[i].messages.empty())
		{
			pos->push_back((t_messages[i].pr_Tank)->GetX());
			t_messages[i].messages.pop_back();
		}
	}
	return pos;
}
