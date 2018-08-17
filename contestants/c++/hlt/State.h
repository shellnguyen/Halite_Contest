#ifndef STATE_H
#define STATE_H

class State
{
protected:
	State();
public:
	unsigned int stateIndex;
	virtual State* getNextState() = 0;
	virtual bool Behaviour() = 0;
};

#endif // !STATE_H