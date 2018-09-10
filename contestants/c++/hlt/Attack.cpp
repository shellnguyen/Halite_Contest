#include "Attack.h"
#include "Global.h"

int Attack::getType() const
{
	return BehaviorType::ATTACK;
}

bool Attack::action(hlt::Ship* s)
{
	//int nFriendly = CountShipInRadius(25.0, s, true);
	//hlt::Log::log("friendly = " + to_string(nFriendly));
	//int nEnemy = CountShipInRadius(35.0, s);
	//hlt::Log::log("enemy = " + to_string(nFriendly));

	//if (nFriendly >= nEnemy)
	//{
		if (s->current_target)
		{
			const hlt::possibly<hlt::Move> move = hlt::navigation::navigate_ship_to_dock(*game_map, *s, *s->current_target, hlt::constants::MAX_SPEED);
			if (move.second)
			{
				moves.push_back(move.first);
				return true;
			}
		}
	//}
	//else
	//{

	//}

	return false;
}