#include "Idle.h"
#include "Moving.h"
#include "planet.hpp"
#include "Global.h"
#include "navigation.hpp"

Idle::Idle()
{
	m_stateIndex = STATE::IDLE;
}

int Idle::getStateIndex() const
{
	return m_stateIndex;
}

State* Idle::getNextState()
{
	return new Moving();
}

bool Idle::behavior(hlt::Ship* s)
{
	hlt::Planet nearestPlanet = GetNearestPlanet(*s);

	if (nearestPlanet.entity_id != -1)
	{
		s->current_target = &nearestPlanet;
		return true;
	}
	else
	{
		hlt::Ship nearestEnemyShip = GetNearestEnemyShip(*s);
		s->current_target = &nearestEnemyShip;
		return true;
	}

	return false;

	//if (nearestPlanet.entity_id != -1)
	//{
	//	if (s->can_dock(nearestPlanet))
	//	{
	//		hlt::Log::log("can dock");
	//		if (!nearestPlanet.is_full() && (nearestPlanet.owner_id == player_id || !nearestPlanet.owned))
	//		{
	//			hlt::Log::log("1");
	//			moves.push_back(hlt::Move::dock(s->entity_id, nearestPlanet.entity_id));
	//			return;
	//		}
	//		else
	//		{
	//			hlt::Log::log("2");
	//			hlt::Ship nearestEnemyShip = GetNearestEnemyShip(*s);
	//			if ((nearestEnemyShip.health > s->health) && nearestEnemyShip.docking_status == hlt::ShipDockingStatus::Undocked)
	//			{
	//				const hlt::possibly<hlt::Move> suicideAttack = hlt::navigation::navigate_ship_towards_target(*game_map, *s, nearestEnemyShip.location, hlt::constants::MAX_SPEED, true, 90, 1);
	//				if (suicideAttack.second)
	//				{
	//					moves.push_back(suicideAttack.first);
	//					return;
	//				}
	//			}
	//			else
	//			{
	//				const hlt::possibly<hlt::Move> move = hlt::navigation::navigate_ship_to_dock(*game_map, *s, nearestEnemyShip, hlt::constants::MAX_SPEED);
	//				if (move.second)
	//				{
	//					moves.push_back(move.first);
	//					return;
	//				}
	//			}

	//		}
	//	}

	//	hlt::Log::log("can not dock");
	//	const hlt::possibly<hlt::Move> move = hlt::navigation::navigate_ship_to_dock(*game_map, *s, nearestPlanet, hlt::constants::MAX_SPEED);
	//	if (move.second)
	//	{
	//		moves.push_back(move.first);
	//	}
	//	else
	//	{
	//		hlt::Log::log("can't move to dock");
	//	}
	//}
	//else
	//{
	//	hlt::Log::log("-1");
	//}
}
