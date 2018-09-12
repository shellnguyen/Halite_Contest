#include "March.h"
#include "Global.h"

int March::getType() const
{
	return BehaviorType::MARCH;
}

bool March::action(hlt::Ship* s)
{
	if (s->current_target)
	{
		const hlt::possibly<hlt::Move> move = hlt::navigation::navigate_ship_to_dock(*game_map, *s, *s->current_target, hlt::constants::MAX_SPEED,moves);
		if (move.second)
		{
			moves.push_back(move.first);
			return true;
		}
	}
	return false;
}