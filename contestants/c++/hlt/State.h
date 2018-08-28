#ifndef STATE_H
#define STATE_H

#include "hlt.hpp"

namespace hlt
{
	struct Ship;
}

enum STATE
{
	IDLE = 0,
	MOVING,
	ENGAGING,
	AVOIDING,
	DOCKING
};

class State
{
protected:
	unsigned int m_stateIndex;
	int m_nextState;
public:
	virtual ~State() { hlt::Log::log("delete State"); };
	virtual int getStateIndex() const = 0;
	virtual int getNextState() { return m_nextState; }
	virtual bool behavior(hlt::Ship* s) = 0;
};

#endif // !STATE_H
