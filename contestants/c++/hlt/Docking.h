#ifndef DOCKING_H
#define DOCKING_H

#include "State.h"

class Docking : public State
{
public:
	Docking();
	int getStateIndex() const override;
	bool behavior(hlt::Ship* s) override;
};

#endif // !DOCKING_H

