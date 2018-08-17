#include "hlt/hlt.hpp"
#include "hlt/navigation.hpp"
#include <algorithm>

hlt::Map* game_map;
hlt::Map* last_turn_map;
std::vector<hlt::Ship> enemy_ships, player_ships;
std::vector<hlt::Planet> neutral_planets, enemy_planets, player_planets;

struct ComparePlanetDistance
{
	const hlt::Ship ship;
	bool operator()(hlt::Planet p1, hlt::Planet p2)
	{
		return (ship.location.get_distance_to(p1.location) < ship.location.get_distance_to(p2.location));
	}
	ComparePlanetDistance(const hlt::Ship ship) : ship(ship) {};
};

struct CompareShipDistance
{
	const hlt::Ship playerShip;
	CompareShipDistance(const hlt::Ship ship) : playerShip(ship) {};

	bool operator()(hlt::Ship s1, hlt::Ship s2)
	{
		return (playerShip.location.get_distance_to(s1.location) < playerShip.location.get_distance_to(s2.location));
	}
};

hlt::Planet GetNearestPlanet(const hlt::Map map, const hlt::Ship ship);
hlt::Ship GetNearestEnemyShip(const hlt::Map map, const hlt::Ship myShip);
bool IsPlayerShip(const hlt::Map map, const hlt::EntityId entityId, const hlt::PlayerId playerId);
int GetNumberOfSurroundShips(const std::vector<hlt::Ship> ships, const hlt::Entity entity);
int GetNumberOfEnemyShips(const hlt::Map map, const hlt::Planet planet, hlt::PlayerId player_id);
void UpdateEntityLists();

int main()
{
    const hlt::Metadata metadata = hlt::initialize("LegendaryShell");
    const hlt::PlayerId player_id = metadata.player_id;

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
		//if (game_map)
		//{
		//	last_turn_map = game_map;
		//}
		game_map = &map;
		UpdateEntityLists();

        for (const hlt::Ship& ship : game_map->ships.at(player_id))
		{
            if (ship.docking_status != hlt::ShipDockingStatus::Undocked) 
			{
                continue;
            }

			const hlt::Planet nearestPlanet = GetNearestPlanet(*game_map, ship);

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
					if (!nearestPlanet.is_full() && (nearestPlanet.owner_id == player_id || !nearestPlanet.owned))
					{
						moves.push_back(hlt::Move::dock(ship.entity_id, nearestPlanet.entity_id));
						continue;
					}
					else
					{
						hlt::Ship nearestEnemyShip = GetNearestEnemyShip(*game_map, ship);
						if (nearestEnemyShip.health > ship.health && nearestEnemyShip.docking_status == hlt::ShipDockingStatus::Undocked)
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

				const hlt::possibly<hlt::Move> move = hlt::navigation::navigate_ship_to_dock(*game_map, ship, nearestPlanet, hlt::constants::MAX_SPEED);
				if (move.second)
				{
					moves.push_back(move.first);
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

hlt::Planet GetNearestPlanet(const hlt::Map map, const hlt::Ship ship)
{
	ComparePlanetDistance comp(ship);
	std::vector<hlt::Planet> planets = map.planets;
	std::sort(planets.begin(), planets.end(), comp);

	for (const hlt::Planet& planet : planets)
	{
		hlt::Log::log("check planet");
		if ((planet.owner_id == ship.owner_id) && planet.is_full())
		{
			hlt::Log::log("continue");
			continue;
		}

		//if (GetNumberOfEnemyShips(map, planet, ship.owner_id) > GetNumberOfSurroundShips(map.ships.at(ship.owner_id), ship))
		//{
		//	continue;
		//}

		return planet;
	}

	hlt::Log::log("no planet");
	hlt::Planet noPlanet;
	noPlanet.entity_id = -1;
	return noPlanet;
}

hlt::Ship GetNearestEnemyShip(const hlt::Map map, const hlt::Ship myShip)
{
	CompareShipDistance comp(myShip);
	std::vector<hlt::Ship> enemyShips;

	//Get list of enemy ship
	for (auto const& shipPair : map.ships)
	{
		if (shipPair.first != myShip.owner_id)
		{
			enemyShips = shipPair.second;
		}
	}

	//Sort enemyShip's list
	std::sort(enemyShips.begin(), enemyShips.end(), comp);

	for (hlt::Ship& enemyShip : enemyShips)
	{
		if (!enemyShip.is_alive())
		{
			continue;
		}

		return enemyShip;
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

void UpdateEntityLists()
{

}
