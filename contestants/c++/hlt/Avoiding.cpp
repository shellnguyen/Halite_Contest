#include "Avoiding.h"
#include "Engaging.h"
#include "Idle.h"
#include "Global.h"

Avoiding::Avoiding()
{
	m_stateIndex = STATE::AVOIDING;
}

int Avoiding::getStateIndex() const
{
	return m_stateIndex;
}

bool Avoiding::behavior(hlt::Ship* s)
{
	if (!s->current_target->is_alive())
	{
		delete s->current_target;
		s->current_target = nullptr;

		for (auto& friendly : s->in_range_allies)
		{
			if (!friendly.is_alive())
			{
				continue;
			}

			s->current_target = &friendly;
			break;
		}
	}

	int nDockedEnemy = 0;
	for (auto& enemy : s->in_range_enemies) //Check nearby docked enemy
	{
		if (s->docking_status == hlt::ShipDockingStatus::Docked || s->docking_status == hlt::ShipDockingStatus::Docking)
		{
			nDockedEnemy++;
		}
	}

	if (s->in_range_enemies.size() > 0)
	{
		if (s->in_range_allies.size() > (s->in_range_enemies.size() - nDockedEnemy))
		{
			delete s->current_target;
			s->current_target = nullptr;
			s->current_target = &s->in_range_enemies[0];

			hlt::possibly<hlt::Move> move = hlt::navigation::navigate_ship_to_dock(*game_map, *s, *s->current_target, hlt::constants::MAX_SPEED);
			if (move.second)
			{
				moves.push_back(move.first);
				m_nextState = STATE::ENGAGING;
				return  true;
			}
		}
	}
	else
	{
		m_nextState = STATE::IDLE;
		return true;
	}

	hlt::possibly<hlt::Move> move = hlt::navigation::navigate_ship_to_dock(*game_map, *s, *s->current_target, hlt::constants::MAX_SPEED);
	if (move.second)
	{
		moves.push_back(move.first);
	}
	return false;
}