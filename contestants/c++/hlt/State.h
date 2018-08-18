#ifndef STATE_H
#define STATE_H

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
public:
	virtual int getStateIndex() const = 0;
	virtual State* getNextState() = 0;
	virtual void behavior(const hlt::Ship s) = 0;
};

#endif // !STATE_H
