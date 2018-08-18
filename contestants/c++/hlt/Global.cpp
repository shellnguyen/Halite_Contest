#include "Global.h"
#include <algorithm>

hlt::Map* game_map;
std::vector<hlt::Ship> enemy_ships, player_ships;
std::vector<hlt::Planet> neutral_planets, enemy_planets, player_planets, non_player_planets;

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