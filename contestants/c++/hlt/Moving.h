#ifndef MOVING_H
#define MOVING_H

#include "State.h"

class Moving : public State
{
public:
	Moving();
	int getStateIndex() const override;
	bool behavior(hlt::Ship* s) override;
};

#endif // !MOVING_H

