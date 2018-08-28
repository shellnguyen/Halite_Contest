#include "Moving.h"
#include "Idle.h"
#include "Global.h"
#include "Avoiding.h"
#include "Docking.h"
#include "Engaging.h"

Moving::Moving()
{
	m_stateIndex = STATE::MOVING;
	m_nextState = STATE::IDLE;
	hlt::Log::log("can dock");
}

int Moving::getStateIndex() const
{
	return m_stateIndex;
}

bool Moving::behavior(hlt::Ship* s)
{
	if (!s->current_target->is_alive())
	{
		delete s->current_target;
		s->current_target = nullptr;
		m_nextState = STATE::IDLE;
		return true;
	}

	if (s->current_target->getType() == 2)
	{
		hlt::Planet* current_target = dynamic_cast<hlt::Planet*>(s->current_target);
		if (s->can_dock(*current_target))
		{
			hlt::Log::log("can dock");
			if (!current_target->is_full() && (current_target->owner_id == player_id || !current_target->owned))
			{
				hlt::Log::log("1");
				moves.push_back(hlt::Move::dock(s->entity_id, current_target->entity_id));
				m_nextState = STATE::DOCKING;
				return true;
			}
			else
			{
				hlt::Log::log("2");
				hlt::Ship nearestEnemyShip = GetNearestEnemyShip(s);
				if ((nearestEnemyShip.health > s->health) && nearestEnemyShip.docking_status == hlt::ShipDockingStatus::Undocked)
				{
					const hlt::possibly<hlt::Move> suicideAttack = hlt::navigation::navigate_ship_towards_target(*game_map, *s, nearestEnemyShip.location, hlt::constants::MAX_SPEED, true, 90, 1);
					if (suicideAttack.second)
					{
						moves.push_back(suicideAttack.first);
						return false;
					}
				}
				else
				{
					const hlt::possibly<hlt::Move> move = hlt::navigation::navigate_ship_to_dock(*game_map, *s, nearestEnemyShip, hlt::constants::MAX_SPEED);
					if (move.second)
					{
						moves.push_back(move.first);
						return false;
					}
				}
			}
		}
	}

	if (s->in_range_enemies.size() > 0)
	{
		int nDockedEnemy = 0;
		for (auto& enemy : s->in_range_enemies) //Check nearby docked enemy
		{
			if (s->docking_status == hlt::ShipDockingStatus::Docked || s->docking_status == hlt::ShipDockingStatus::Docking)
			{
				nDockedEnemy++;
			}
		}

		if (s->in_range_enemies[0].location.get_distance_to(s->location) <= hlt::constants::WEAPON_RADIUS + 2)
		{
			if (s->in_range_allies.size() > (s->in_range_enemies.size() - nDockedEnemy))
			{
				delete s->current_target;
				s->current_target = nullptr;
				s->current_target = &s->in_range_enemies[0];
				hlt::possibly<hlt::Move> move = hlt::navigation::navigate_ship_to_dock(*game_map, *s, *s->current_target, hlt::constants::MAX_SPEED);
				if (move.second)
				{
					m_nextState = STATE::ENGAGING;
					moves.push_back(move.first);
					return true;
				}
			}
			else
			{
				hlt::Ship friendly = s->in_range_allies[0];
				delete s->current_target;
				s->current_target = nullptr;
				s->current_target = &friendly;
				hlt::possibly<hlt::Move> move = hlt::navigation::navigate_ship_to_dock(*game_map, *s, *s->current_target, hlt::constants::MAX_SPEED);
				if (move.second)
				{
					m_nextState = STATE::AVOIDING;
					moves.push_back(move.first);
					return true;
				}
			}
		}
	}

	return false;
}