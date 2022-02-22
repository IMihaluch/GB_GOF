#pragma once

#include "House.h"

class HouseBuilder 

{
public:
HouseBuilder() : product_House(new House) {}
               virtual void Constube() {};
               virtual void Consroof() {};
               virtual void Conswalls() {};
               virtual void Conswindow() {};
House* getHouse();

protected:

House* product_House;
};

class HouseBuilderA : public HouseBuilder
 {
public:
              void Constube() override;
              void Consroof() override;
              void Conswalls() override;
              void Conswindow() override;
};

class HouseDirector 
{
public:
	House* Cons(HouseBuilderA& builder);
};
