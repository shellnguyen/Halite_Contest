#ifndef AVOIDING_H
#define AVOIDING_H

#include "State.h"

class Avoiding : public State
{
public:
	Avoiding();
	int getStateIndex() const override;
	bool behavior(hlt::Ship* s) override;
};

#endif // !AVOIDING_H

