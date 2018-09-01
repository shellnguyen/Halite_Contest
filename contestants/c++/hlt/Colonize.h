#ifndef COLONIZE_H
#define COLONIZE_H

#include "Behavior.h"

class Colonize : public Behavior
{
public:
	int getType() const override;
	bool action(hlt::Ship* s) override;
};

#endif // !COLONIZE_H

