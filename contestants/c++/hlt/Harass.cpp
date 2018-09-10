#include "Harass.h"
#include "Global.h"

int Harass::getType() const
{
	return BehaviorType::HARASS;
}

bool Harass::action(hlt::Ship* s)
{
	return false;
}