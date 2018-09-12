#include "Defend.h"
#include "Global.h"

int Defend::getType() const
{
	return BehaviorType::DEFEND;
}

bool Defend::action(hlt::Ship* s)
{
	hlt::Planet* planet = dynamic_cast<hlt::Planet*>(s->current_target);

	if (planet)
	{
		hlt::Ship friendlyShip = game_map->get_ship(player_id, planet->docked_ships[0]);

		if (friendlyShip.in_range_enemies.size() > 0)
		{
			const hlt::possibly<hlt::Move> move = hlt::navigation::navigate_ship_to_dock(*game_map, *s, friendlyShip.in_range_enemies[0], hlt::constants::MAX_SPEED, moves);
			if (move.second)
			{
				moves.push_back(move.first);
				return true;
			}
		}
		else
		{
			if (s->can_dock(*planet))
			{
				hlt::Ship nearestEnemy = GetNearestEnemyShip(s);
				if (nearestEnemy.entity_id != -1)
				{
					const hlt::possibly<hlt::Move> move = hlt::navigation::navigate_ship_to_dock(*game_map, *s, nearestEnemy, hlt::constants::MAX_SPEED, moves);
					if (move.second)
					{
						moves.push_back(move.first);
						return true;
					}
				}
			}
			else
			{
				const hlt::possibly<hlt::Move> move = hlt::navigation::navigate_ship_to_dock(*game_map, *s, *s->current_target, hlt::constants::MAX_SPEED, moves);
				if (move.second)
				{
					moves.push_back(move.first);
					return true;
				}
			}
		}
	}
	return false;
}