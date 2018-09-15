#include "Colonize.h"
#include "Global.h"

int Colonize::getType() const
{
	return BehaviorType::COLONIZE;
}

bool Colonize::action(hlt::Ship* s)
{
	if (s->current_target != nullptr)
	{
		hlt::Planet* planet = dynamic_cast<hlt::Planet*>(s->current_target);

		if (!planet->owned || planet->owner_id == player_id)
		{
			moves.push_back(hlt::Move::dock(s->entity_id, s->current_target->entity_id));
			return true;
		}
		else
		{
			s->current_target = potential_ship_targets[0];
			if (s->current_target)
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
