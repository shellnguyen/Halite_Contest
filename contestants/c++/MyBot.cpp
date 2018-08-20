#include "hlt/navigation.hpp"
#include "hlt/Global.h"
#include <algorithm>

bool IsPlayerShip(const hlt::Map map, const hlt::EntityId entityId, const hlt::PlayerId playerId);
int GetNumberOfSurroundShips(const std::vector<hlt::Ship> ships, const hlt::Entity entity);
int GetNumberOfEnemyShips(const hlt::Map map, const hlt::Planet planet, hlt::PlayerId player_id);

int main()
{
    const hlt::Metadata metadata = hlt::initialize("LegendaryShell");
	player_id = metadata.player_id;

    const hlt::Map& initial_map = metadata.initial_map;

    // We now have 1 full minute to analyse the initial map.
    std::ostringstream initial_map_intelligence;
    initial_map_intelligence
            << "width: " << initial_map.map_width
            << "; height: " << initial_map.map_height
            << "; players: " << initial_map.ship_map.size()
            << "; my ships: " << initial_map.ship_map.at(player_id).size()
            << "; planets: " << initial_map.planets.size();
    hlt::Log::log(initial_map_intelligence.str());

    std::vector<hlt::Move> moves;
    for (;;) {
        moves.clear();
        hlt::Map map = hlt::in::get_map();
		game_map = &map;
		UpdatePlanetList();
		UpdateShipList();
		//UpdateNearbyShip();

        for (hlt::Ship ship : player_ships)
		{
            if (ship.docking_status != hlt::ShipDockingStatus::Undocked) 
			{
                continue;
            }

			if (!ship.current_state)
			{
				ship.current_state = new Idle();
			}

			//ship.action();

			const hlt::Planet nearestPlanet = GetNearestPlanet(ship);

			//if (nearestPlanet.entity_id != -1)
			//{
			//	if (GetNumberOfEnemyShips(map, nearestPlanet, player_id) <= GetNumberOfSurroundShips(map.ships.at(player_id), ship))
			//	{
			//		if (ship.can_dock(nearestPlanet))
			//		{
			//			if (!nearestPlanet.is_full())
			//			{
			//				moves.push_back(hlt::Move::dock(ship.entity_id, nearestPlanet.entity_id));
			//				continue;
			//			}
			//			else
			//			{
			//				hlt::Ship nearestEnemyShip = GetNearestEnemyShip(map, ship);
			//				const hlt::possibly<hlt::Move> move = hlt::navigation::navigate_ship_towards_target(map, ship, nearestEnemyShip.location, hlt::constants::MAX_SPEED, true, 25, 5);
			//				if (move.second)
			//				{
			//					moves.push_back(move.first);
			//					continue;
			//				}
			//			}
			//		}
			//	}
			//}

			if (nearestPlanet.entity_id != -1)
			{
				if (ship.can_dock(nearestPlanet))
				{
					hlt::Log::log("can dock");
					if (!nearestPlanet.is_full() && (nearestPlanet.owner_id == player_id || !nearestPlanet.owned))
					{
						hlt::Log::log("1");
						moves.push_back(hlt::Move::dock(ship.entity_id, nearestPlanet.entity_id));
						continue;
					}
					else
					{
						hlt::Log::log("2");
						hlt::Ship nearestEnemyShip = GetNearestEnemyShip(ship);
						if ((nearestEnemyShip.health > ship.health) && nearestEnemyShip.docking_status == hlt::ShipDockingStatus::Undocked)
						{
							const hlt::possibly<hlt::Move> suicideAttack = hlt::navigation::navigate_ship_towards_target(*game_map, ship, nearestEnemyShip.location, hlt::constants::MAX_SPEED, true, 90, 1);
							if (suicideAttack.second)
							{
								moves.push_back(suicideAttack.first);
								continue;
							}
						}
						else
						{
							const hlt::possibly<hlt::Move> move = hlt::navigation::navigate_ship_to_dock(*game_map, ship, nearestEnemyShip, hlt::constants::MAX_SPEED);
							if (move.second)
							{
								moves.push_back(move.first);
								continue;
							}
						}

					}
				}

				hlt::Log::log("can not dock");
				const hlt::possibly<hlt::Move> move = hlt::navigation::navigate_ship_to_dock(*game_map, ship, nearestPlanet, hlt::constants::MAX_SPEED);
				if (move.second)
				{
					moves.push_back(move.first);
				}
				else
				{
					hlt::Log::log("can't move to dock");
				}
			}
			else
			{
				hlt::Log::log("-1");
			}
        }

        if (!hlt::out::send_moves(moves)) 
		{
            hlt::Log::log("send_moves failed; exiting");
            break;
        }
    }
}

int GetNumberOfEnemyShips(const hlt::Map map, const hlt::Planet planet, hlt::PlayerId player_id)
{
	int numOfEnemyShips = 0;
	std::vector<hlt::Ship> enemyShips;

	//Get list of enemy ship
	for (auto const& shipPair : map.ships)
	{
		if (shipPair.first != player_id)
		{
			enemyShips = shipPair.second;
		}
	}

	std::vector<hlt::Ship> filterEnemyShip;
	//std::copy_if(enemyShips.begin(), enemyShips.end(), filterEnemyShip, [](hlt::Ship s) {return s.docking_status != hlt::ShipDockingStatus::Undocked; });

	numOfEnemyShips = GetNumberOfSurroundShips(filterEnemyShip, planet);

	return numOfEnemyShips;
}

int GetNumberOfSurroundShips(const std::vector<hlt::Ship> ships, const hlt::Entity entity)
{
	int numOfShips = 0;

	for (hlt::Ship ship : ships)
	{
		if (entity.location.get_distance_to(ship.location) <= 6.0)
		{
			numOfShips++;
		}
	}

	return numOfShips;
}

bool IsPlayerShip(const hlt::Map map, const hlt::EntityId entityId, const hlt::PlayerId playerId)
{
	if (entityId == -1)
	{
		return false;
	}

	for (const hlt::Ship& ship : map.ships.at(playerId))
	{
		if (ship.entity_id == entityId)
		{
			return true;
		}
	}

	return false;
}
