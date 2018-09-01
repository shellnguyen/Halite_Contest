#ifndef ATTACK_H
#define ATTACK_H

#include "Behavior.h"

class Attack : public Behavior
{
public:
	int getType() const override;
	bool action(hlt::Ship* s) override;
};

#endif // !ATTACK_H

