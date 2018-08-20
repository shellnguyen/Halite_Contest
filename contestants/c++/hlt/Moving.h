#ifndef MOVING_H
#define MOVING_H

#include "State.h"

class Moving : public State
{
private:
	unsigned int m_nextState;
public:
	Moving();
	int getStateIndex() const override;
	State* getNextState() override;
	bool behavior(hlt::Ship* s) override;
};

#endif // !MOVING_H

