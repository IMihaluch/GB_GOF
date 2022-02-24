#pragma once

class Bomb;
class Plane;

class Visitor
{

public:
	virtual void log(const Bomb* bomb) = 0;
	virtual void log(const Plane* plane) = 0;
};

class LogVisitor :public Visitor 
{

public:
	
	virtual void log(const Bomb* bomb) override;
	virtual void log(const Plane* plane) override;
};
