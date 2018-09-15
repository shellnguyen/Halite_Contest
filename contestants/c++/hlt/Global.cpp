#include "Global.h"
#include <algorithm>

hlt::PlayerId player_id;
hlt::Map* game_map;
std::vector<hlt::Ship> enemy_ships, enemy_docked_ship;
std::vector<hlt::Ship> player_ships, player_undocked_ships;
std::vector<hlt::Planet> neutral_planets, enemy_planets, player_planets, non_player_planets;
std::vector<hlt::Move> moves;
std::unordered_map<hlt::EntityId, hlt::EntityId> last_turn_targets; //store ship last turn target id
int nShipHarass;
std::vector<hlt::Planet*> potential_planet_targets;
std::vector<hlt::Ship*> potential_ship_targets;
int current_turn;
bool rush_mode;

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
						if (s2.docking_status != hlt::ShipDockingStatus::Undocked)
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
						if (s2.docking_status != hlt::ShipDockingStatus::Undocked)
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

void UpdateShipScore()
{
	for (auto& ship_pair : game_map->ships)
	{
		for (auto& ship : ship_pair.second)
		{
			ship.score = SCORE_BASE_SHIP;
			if (ship.docking_status != hlt::ShipDockingStatus::Undocked)
			{
				if (ship.owner_id == player_id)
				{
					ship.score += SCORE_FRIENDLY_DOCKED_SHIP;
				}
				else
				{
					ship.score += SCORE_ENEMY_DOCKED_SHIP;
				}
			}
		}
	}
}

void UpdatePlanetScore()
{
	for (hlt::Planet& planet : game_map->planets)
	{
		if (!planet.owned || planet.owner_id == player_id)
		{
			planet.score = SCORE_BASE_PLANET;
		}
		else
		{
			planet.score = SCORE_ENEMY_PLANET;
		}
		planet.score += SCORE_PER_DOCKING_SPOT * planet.docking_spots;
	}
}

void UpdateLastTurnTargets()
{
	bool isAlived = false;
	std::vector<hlt::EntityId> pair_need_deleted;
	for (auto& pair : last_turn_targets)
	{
		for (int i = 0; i < player_undocked_ships.size(); ++i)
		{
			if (pair.first == player_undocked_ships[i].entity_id)
			{
				isAlived = true;
				break;
			}
		}

		if (!isAlived)
		{
			pair_need_deleted.push_back(pair.first);
		}
	}

	for (int i = 0; i < pair_need_deleted.size(); ++i)
	{
		last_turn_targets.erase(pair_need_deleted[i]);
	}
}

//Logic function
struct ComparePlanetDistance
{
	hlt::Ship* ship;
	bool operator()(hlt::Planet p1, hlt::Planet p2)
	{
		return (p1.score - ship->location.get_distance_to(p1.location)) > (p2.score - ship->location.get_distance_to(p2.location));
	}
	ComparePlanetDistance(hlt::Ship* ship) : ship(ship) {};
};

struct CompareShipDistance
{
	hlt::Ship* playerShip;
	CompareShipDistance(hlt::Ship* ship) : playerShip(ship) {};

	bool operator()(hlt::Ship s1, hlt::Ship s2)
	{
		return (s1.score - playerShip->location.get_distance_to(s1.location)) > (s2.score - playerShip->location.get_distance_to(s2.location));
	}
};

bool GetNearestPlanet(hlt::Ship* ship) //Find nearest planet for colonize
{
	hlt::Log::log("GetNearestPlanet");
	ComparePlanetDistance comp(ship);
	//std::sort(non_player_planets.begin(), non_player_planets.end(), [ship](hlt::Planet* a, hlt::Planet* b) { return a->location.get_distance_to(ship.location) < b->location.get_distance_to(ship.location); });

	std::sort(game_map->planets.begin(), game_map->planets.end(), comp);

	for (hlt::Planet& planet : game_map->planets)
	{
		if ((planet.owner_id == ship->owner_id) && planet.is_full())
		{
			hlt::Log::log("continue");
			continue;
		}

		if (!planet.owned && planet.targeted >= planet.docking_spots)
		{
			continue;
		}

		if (planet.targeted >= MAX_ATTACK_TARGET)
		{
			continue;
		}

		potential_planet_targets.push_back(&planet);
		return true;
	}

	hlt::Log::log("no planet");
	return false;
}

bool GetNearestPlayerPlanetForDefend(hlt::Ship* ship) //Find nearest Player Planet that could be target by Enemy
{
	hlt::Log::log("GetNearestPlanet");
	ComparePlanetDistance comp(ship);
	//std::sort(non_player_planets.begin(), non_player_planets.end(), [ship](hlt::Planet* a, hlt::Planet* b) { return a->location.get_distance_to(ship.location) < b->location.get_distance_to(ship.location); });

	std::sort(player_planets.begin(), player_planets.end(), comp);

	for (hlt::Planet& planet : player_planets)
	{
		int nEnemy = CountShipInRadius(30.0, &planet);
		int nFriendly = CountShipInRadius(20.0, &planet, true);

		hlt::Log::log("enemy = " + to_string(nEnemy) + ", friendly = " + to_string(nFriendly));

		if ((nEnemy > nFriendly) && ((planet.targeted + nFriendly - nEnemy) < 2) && (planet.targeted < MAX_ATTACK_TARGET))
		{
			potential_planet_targets.push_back(&planet);
			return true;
		}
	}

	hlt::Log::log("no friendly planet need defend");
	return false;
}

bool GetNearestEnemyShip(hlt::Ship* myShip)
{
	CompareShipDistance comp(myShip);
	//std::sort(enemy_ships.begin(), enemy_ships.end(), [myShip](hlt::Ship* a, hlt::Ship* b) { return a->location.get_distance_to(myShip->location) < b->location.get_distance_to(myShip->location); });

	//Sort enemyShip's list
	std::sort(enemy_ships.begin(), enemy_ships.end(), comp);

	for (hlt::Ship& enemyShip : enemy_ships)
	{
		if (!enemyShip.is_alive() || enemyShip.docking_status != hlt::ShipDockingStatus::Undocked)
		{
			continue;
		}

		if (enemyShip.targeted > enemyShip.in_range_allies.size() + MAX_ATTACK_TARGET)
		{
			continue;
		}

		potential_ship_targets.push_back(&enemyShip);
		return true;
	}

	hlt::Log::log("no enemy ship");
	return false;
}

bool GetNearestDockedEnemyShip(hlt::Ship* myShip)
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

		potential_ship_targets.push_back(&enemyShip);
		return true;
	}

	hlt::Log::log("no enemy docked ship");
	return false;
}

bool GetNearestPlayerPlanet(hlt::Ship* ship)
{
	hlt::Log::log("GetNearestPlayerPlanet");
	ComparePlanetDistance comp(ship);

	std::sort(player_planets.begin(), player_planets.end(), comp);

	for (hlt::Planet& planet : player_planets)
	{
		if (planet.is_full())
		{
			continue;
		}

		//if (planet.docking_spots >= planet.targeted)
		//{
		//	continue;
		//}

		potential_planet_targets.push_back(&planet);
		return true;
	}

	hlt::Log::log("no player planet");
	return false;
}

bool GetNearestNeutralPlanet(hlt::Ship* ship)
{
	hlt::Log::log("GetNearestNeutralPlanet");
	ComparePlanetDistance comp(ship);

	std::sort(neutral_planets.begin(), neutral_planets.end(), comp);

	for (hlt::Planet& planet : neutral_planets)
	{
		if (planet.targeted >= planet.docking_spots)
		{
			continue;
		}

		if (planet.targeted >= MAX_COLONIZE_NEUTRAL_PLANET)
		{
			continue;
		}

		potential_planet_targets.push_back(&planet);
		return true;
	}

	hlt::Log::log("no neutral planet");
	return false;
}

bool GetNearestEnemyPlanet(hlt::Ship* ship)
{
	hlt::Log::log("GetNearestEnemyPlanet");
	ComparePlanetDistance comp(ship);

	std::sort(enemy_planets.begin(), enemy_planets.end(), comp);

	for (hlt::Planet& planet : enemy_planets)
	{
		if (planet.targeted >= MAX_COLONIZE_ENEMY_PLANET)
		{
			continue;
		}

		potential_planet_targets.push_back(&planet);
		return true;
	}

	hlt::Log::log("no enemy planet");
	return false;
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

void SortPotentialTargets(hlt::Ship* ship)
{
	std::sort(potential_planet_targets.begin(), potential_planet_targets.end(), [ship](hlt::Planet* a, hlt::Planet* b) { return (a->getScore() - ship->location.get_distance_to(a->location)) > (b->getScore() - ship->location.get_distance_to(b->location)); });
	std::sort(potential_ship_targets.begin(), potential_ship_targets.end(), [ship](hlt::Ship* a, hlt::Ship* b) { return (a->getScore() - ship->location.get_distance_to(a->location)) > (b->getScore() - ship->location.get_distance_to(b->location)); });

	for (auto& planet : potential_planet_targets)
	{
		hlt::Log::log("planet_target[" + to_string(planet->entity_id) + "].score = " + to_string(planet->getScore()));
		hlt::Log::log("planet_target[" + to_string(planet->entity_id) + "].distance to ship = " + to_string(ship->location.get_distance_to(planet->location)));
	}

	for (auto& s : potential_ship_targets)
	{
		hlt::Log::log("ship_target[" + to_string(s->entity_id) + "].score = " + to_string(s->getScore()));
		hlt::Log::log("ship_target[" + to_string(s->entity_id) + "].distance to ship = " + to_string(ship->location.get_distance_to(s->location)));
	}
}