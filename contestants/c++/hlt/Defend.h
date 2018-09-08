#ifndef DEFEND_H
#define DEFEND_H

#include "Behavior.h"

class Defend : public Behavior
{
public:
	int getType() const override;
	bool action(hlt::Ship* s) override;
};

#endif // !DEFEND_H

