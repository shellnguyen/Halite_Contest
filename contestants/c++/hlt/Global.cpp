#include "Global.h"
#include <algorithm>

hlt::PlayerId player_id;
hlt::Map* game_map;
std::vector<hlt::Ship> enemy_ships, player_ships;
std::vector<hlt::Planet> neutral_planets, enemy_planets, player_planets, non_player_planets;

//Update all list in begining of turn
void UpdatePlanetList()
{
	neutral_planets.clear();
	enemy_planets.clear();
	player_planets.clear();
	non_player_planets.clear();

	for (const hlt::Planet& planet : game_map->planets)
	{
		if (!planet.owned)
		{
			neutral_planets.push_back(planet);
			non_player_planets.push_back(planet);
		}
		else
		{
			if (planet.owner_id != player_id)
			{
				enemy_planets.push_back(planet);
				non_player_planets.push_back(planet);
			}
			else
			{
				player_planets.push_back(planet);
			}
		}
	}
}

void UpdateShipList()
{
	enemy_ships.clear();
	player_ships.clear();

	for (int i = 0; i < game_map->ships.size(); ++i)
	{
		int nShip = game_map->ships.at(i).size();
		if (i == player_id)
		{
			player_ships.reserve(nShip);
			player_ships.assign(game_map->ships.at(i).begin(), game_map->ships.at(i).end());
		}
		else
		{
			nShip += enemy_planets.size();
			enemy_planets.reserve(nShip);
			enemy_ships.insert(enemy_ships.end(), game_map->ships.at(i).begin(), game_map->ships.at(i).end());
		}
	}
}

void UpdateNearbyShip()
{
	double range = hlt::constants::SHIP_RADIUS * 2 + hlt::constants::WEAPON_RADIUS + hlt::constants::MAX_SPEED * 2;

	for (auto& player_v_pair : game_map->ships) 
	{
		for (hlt::Ship& s : player_v_pair.second)
		{
			s.in_range_allies.clear();
			s.in_range_enemies.clear();
			for (auto& player_v_pair_2 : game_map->ships) {
				if (player_v_pair_2.first == player_v_pair.first) 
				{
					for (hlt::Ship& s2 : player_v_pair_2.second) 
					{
						double d = s.location.get_distance_to(s2.location);
						if (d <= range && d != 0) {
							s.in_range_allies.push_back(s2);
						}
					}
				}
				else 
				{
					for (hlt::Ship& s2 : player_v_pair_2.second) 
					{
						if (s.location.get_distance_to(s2.location) <= range)
						{
							s.in_range_enemies.push_back(s2);
						}
					}
				}
			}

			// sort the vectors by distance, closest to furthest            
			std::sort(s.in_range_enemies.begin(), s.in_range_enemies.end(), [s](hlt::Ship a, hlt::Ship b) { return a.location.get_distance_to(s.location) < b.location.get_distance_to(s.location); });
			std::sort(s.in_range_allies.begin(), s.in_range_allies.end(), [s](hlt::Ship a, hlt::Ship b) { return a.location.get_distance_to(s.location) < b.location.get_distance_to(s.location); });
		}
	}
}

//Logic function
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

hlt::Planet GetNearestPlanet(const hlt::Ship ship)
{
	ComparePlanetDistance comp(ship);
	//std::sort(non_player_planets.begin(), non_player_planets.end(), [ship](hlt::Planet* a, hlt::Planet* b) { return a->location.get_distance_to(ship.location) < b->location.get_distance_to(ship.location); });

	std::sort(game_map->planets.begin(), game_map->planets.end(), comp);

	for (const hlt::Planet& planet : game_map->planets)
	{
		hlt::Log::log("check planet");
		if ((planet.owner_id == ship.owner_id) && planet.is_full())
		{
			hlt::Log::log("continue");
			continue;
		}

		return planet;
	}

	hlt::Log::log("no planet");
	hlt::Planet noPlanet;
	noPlanet.entity_id = -1;
	return noPlanet;
}

hlt::Ship GetNearestEnemyShip(const hlt::Ship myShip)
{
	CompareShipDistance comp(myShip);
	//std::sort(enemy_ships.begin(), enemy_ships.end(), [myShip](hlt::Ship* a, hlt::Ship* b) { return a->location.get_distance_to(myShip->location) < b->location.get_distance_to(myShip->location); });

	//Sort enemyShip's list
	std::sort(enemy_ships.begin(), enemy_ships.end(), comp);

	for (hlt::Ship& enemyShip : enemy_ships)
	{
		if (!enemyShip.is_alive())
		{
			continue;
		}

		return enemyShip;
	}
}