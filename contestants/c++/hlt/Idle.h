#ifndef IDLE_H
#define IDLE_H

#include "State.h"

class Idle : public State
{
public:
	Idle();
	int getStateIndex() const override;
	State* getNextState() override;
	void behavior(const hlt::Ship s) override;
};

#endif // !IDLE_H

