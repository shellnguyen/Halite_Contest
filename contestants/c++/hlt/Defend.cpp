#include "Defend.h"
#include "Global.h"

int Defend::getType() const
{
	return BehaviorType::DEFEND;
}

bool Defend::action(hlt::Ship* s)
{
	const hlt::possibly<hlt::Move> move = hlt::navigation::navigate_ship_to_dock(*game_map, *s, *s->current_target, hlt::constants::MAX_SPEED, moves);
	if (move.second)
	{
		moves.push_back(move.first);
		return true;
	}

	return false;
}