#ifndef MARCH_H
#define MARCH_H

#include "Behavior.h"

class March : public Behavior
{
public:
	int getType() const override;
	bool action(hlt::Ship* s) override;
};

#endif // !MARCH_H

