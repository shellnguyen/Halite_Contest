#ifndef DOCKING_H
#define DOCKING_H

#include "Behavior.h"

class Docking : public Behavior
{
public:
	int getType() const override;
	bool action(hlt::Ship* s) override;
};

#endif // !DOCKING_H

