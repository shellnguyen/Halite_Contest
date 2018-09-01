#ifndef BEHAVIOR_H
#define BEHAVIOR_H

namespace hlt
{
	struct Ship;
}

enum BehaviorType
{
	COLONIZE = 1,
	MARCH = 2,
	RUSH = 3,
	ATTACK = 4,
	DEFEND = 5,
	HARASS = 6
};

class Behavior
{
public:
	virtual int getType() const = 0;
	virtual bool action(hlt::Ship* s) = 0;
};

#endif // !BEHAVIOR_H

