#ifndef HARASS_H
#define HARASS_H

#include "Behavior.h"

class Harass : public Behavior
{
public:
	int getType() const override;
	bool action(hlt::Ship* s) override;
};

#endif // !HARASS_H

