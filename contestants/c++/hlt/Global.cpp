#include "Global.h"
#include <algorithm>

hlt::PlayerId player_id;
hlt::Map* game_map;
std::vector<hlt::Ship> enemy_ships, enemy_docked_ship;
std::vector<hlt::Ship> player_ships, player_undocked_ships;
std::vector<hlt::Planet> neutral_planets, enemy_planets, player_planets, non_player_planets;
std::vector<hlt::Move> moves;
int nShipHarass;

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
	hlt::Log::log("UpdateShipList");
	enemy_ships.clear();
	enemy_docked_ship.clear();
	hlt::Log::log("clear enemy_ships");
	player_ships.clear();
	hlt::Log::log("clear player_ships");
	player_undocked_ships.clear();

	for (int i = 0; i < game_map->ships.size(); ++i)
	{
		int nShip = game_map->ships.at(i).size();
		if (i == player_id)
		{
			hlt::Log::log("add ship to player_ships");
			player_ships.reserve(nShip);
			player_ships.assign(game_map->ships.at(i).begin(), game_map->ships.at(i).end());
			hlt::Log::log("after add ship to player_ships");
		}
		else
		{
			hlt::Log::log("add ship to enemy_ships");
			nShip += enemy_planets.size();
			enemy_planets.reserve(nShip);
			enemy_ships.insert(enemy_ships.end(), game_map->ships.at(i).begin(), game_map->ships.at(i).end());
			hlt::Log::log("after add ship to enemy_ships");
		}
	}

	for (auto& ship : player_ships)
	{
		if (ship.docking_status != hlt::ShipDockingStatus::Undocked)
		{
			continue;
		}

		player_undocked_ships.push_back(ship);
	}

	for (auto& enemy : enemy_ships)
	{
		if (enemy.docking_status != hlt::ShipDockingStatus::Undocked)
		{
			enemy_docked_ship.push_back(enemy);
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
			for (auto& player_v_pair_2 : game_map->ships) 
			{
				if (player_v_pair_2.first == player_v_pair.first) 
				{
					for (hlt::Ship& s2 : player_v_pair_2.second) 
					{
						if (s2.docking_status != hlt::ShipDockingStatus::Docked)
						{
							continue;
						}
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
						if (s2.docking_status != hlt::ShipDockingStatus::Docked)
						{
							continue;
						}
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
	hlt::Ship* ship;
	bool operator()(hlt::Planet p1, hlt::Planet p2)
	{
		return (ship->location.get_distance_to(p1.location) < ship->location.get_distance_to(p2.location));
	}
	ComparePlanetDistance(hlt::Ship* ship) : ship(ship) {};
};

struct CompareShipDistance
{
	hlt::Ship* playerShip;
	CompareShipDistance(hlt::Ship* ship) : playerShip(ship) {};

	bool operator()(hlt::Ship s1, hlt::Ship s2)
	{
		return (playerShip->location.get_distance_to(s1.location) < playerShip->location.get_distance_to(s2.location));
	}
};

hlt::Planet GetNearestPlanet(hlt::Ship* ship) //Find nearest planet for colonize
{
	hlt::Log::log("GetNearestPlanet");
	ComparePlanetDistance comp(ship);
	//std::sort(non_player_planets.begin(), non_player_planets.end(), [ship](hlt::Planet* a, hlt::Planet* b) { return a->location.get_distance_to(ship.location) < b->location.get_distance_to(ship.location); });

	std::sort(game_map->planets.begin(), game_map->planets.end(), comp);

	for (const hlt::Planet& planet : game_map->planets)
	{
		if ((planet.owner_id == ship->owner_id) && planet.is_full())
		{
			hlt::Log::log("continue");
			continue;
		}

		if (planet.targeted >= MAX_TARGETED)
		{
			continue;
		}

		return planet;
	}

	hlt::Log::log("no planet");
	hlt::Planet noPlanet;
	noPlanet.entity_id = -1;
	return noPlanet;
}

hlt::Planet GetNearestPlayerPlanet(hlt::Ship* ship) //Find nearest Player Planet that could be target by Enemy
{
	hlt::Log::log("GetNearestPlanet");
	ComparePlanetDistance comp(ship);
	//std::sort(non_player_planets.begin(), non_player_planets.end(), [ship](hlt::Planet* a, hlt::Planet* b) { return a->location.get_distance_to(ship.location) < b->location.get_distance_to(ship.location); });

	std::sort(player_planets.begin(), player_planets.end(), comp);

	for (auto& planet : player_planets)
	{
		int nEnemy = CountShipInRadius(10, &planet);
		int nFriendly = CountShipInRadius(15.0, &planet, true);

		hlt::Log::log("enemy = " + to_string(nEnemy) + ", friendly = " + to_string(nFriendly));

		if ((nEnemy > nFriendly) && ((planet.targeted + nFriendly - nEnemy) < 2) && (planet.targeted < MAX_TARGETED))
		{
			return planet;
		}
	}

	hlt::Log::log("no planet");
	hlt::Planet noPlanet;
	noPlanet.entity_id = -1;
	return noPlanet;
}

hlt::Ship GetNearestEnemyShip(hlt::Ship* myShip)
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

		if (enemyShip.targeted > enemyShip.in_range_allies.size() + MAX_TARGETED)
		{
			continue;
		}

		return enemyShip;
	}
}

hlt::Ship GetNearestDockedEnemyShip(hlt::Ship* myShip)
{
	CompareShipDistance comp(myShip);
	//std::sort(enemy_ships.begin(), enemy_ships.end(), [myShip](hlt::Ship* a, hlt::Ship* b) { return a->location.get_distance_to(myShip->location) < b->location.get_distance_to(myShip->location); });

	//Sort enemyShip's list
	std::sort(enemy_ships.begin(), enemy_ships.end(), comp);

	for (hlt::Ship& enemyShip : enemy_ships)
	{
		if (!enemyShip.is_alive() || (enemyShip.docking_status == hlt::ShipDockingStatus::Undocked))
		{
			continue;
		}

		return enemyShip;
	}
}

int CountShipInRadius(double radius, hlt::Entity* s, bool friendlyOnly)
{
	int nShip = 0;

	if (friendlyOnly)
	{	
		for (auto& frienly : player_ships)
		{
			if (frienly.docking_status != hlt::ShipDockingStatus::Undocked)
			{
				continue;
			}

			if (s->location.get_distance_to(frienly.location) <= radius + s->radius)
			{
				nShip++;
			}
		}
	}
	else
	{
		for (auto& enemy : enemy_ships)
		{
			if (enemy.docking_status != hlt::ShipDockingStatus::Undocked)
			{
				continue;
			}

			if (s->location.get_distance_to(enemy.location) <= radius + s->radius)
			{
				nShip++;
			}
		}
	}

	return nShip;
}

bool ShouldDockToPlanet(hlt::Planet* planet)
{
	if (planet)
	{
		if (planet->docked_ships.size() < planet->docking_spots)
		{
			return true;
		}
	}
	return false;
}