#include "hlt/navigation.hpp"
#include "hlt/Global.h"
#include "hlt/Colonize.h"
#include "hlt/Attack.h"
#include "hlt/March.h"
#include "hlt/Defend.h"
#include "hlt/Docking.h"
#include <algorithm>

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

	nShipHarass = 0;
	current_turn = 0;
	rush_mode = false;

    for (;;) {
		current_turn++;
        moves.clear();
        hlt::Map map = hlt::in::get_map();
		game_map = &map;
		UpdateNearbyShip();
		UpdateShipScore();
		UpdatePlanetScore();
		UpdatePlanetList();
		UpdateShipList();
		UpdateLastTurnTargets();

		hlt::Log::log("player_ships.size = " + to_string(player_ships.size()));
        for (hlt::Ship& ship : player_ships)
		{
			potential_planet_targets.clear();
			potential_ship_targets.clear();
			std::pair<hlt::EntityId, hlt::EntityId> ship_target_pair;
			hlt::Log::log("ship[" + to_string(ship.entity_id) + "]");

			int nFriendly = 0;
			int nEnemy = 0;

			bool neutralPlanetFound = GetNearestNeutralPlanet(&ship);
			bool enemyPlanetFound = GetNearestEnemyPlanet(&ship);
			bool friendlyPlanetFound = GetNearestPlayerPlanet(&ship);
			bool attackedPlanetFound = GetNearestPlayerPlanetForDefend(&ship);
			bool enemyFound = GetNearestEnemyShip(&ship);
			bool enemyDockedFound = GetNearestDockedEnemyShip(&ship);

			SortPotentialTargets(&ship);

			if (current_turn == 1)
			{
				if (ship.entity_id == 0)
				{
					ship.current_behavior = new March();
					ship.current_target = potential_planet_targets[0];
					ship.current_target->targeted++;
				}
				else
				{
					ship.current_behavior = new March();
					ship.current_target = player_ships[0].current_target;
					ship.current_target->targeted++;
				}
			}

			if (ship.docking_status == hlt::ShipDockingStatus::Undocked)
			{
				if (potential_planet_targets.size() > 0)
				{
					if (ship.can_dock(*potential_planet_targets[0]))
					{
						if (!enemyDockedFound && nEnemy > 0)
						{
							rush_mode = true;
						}
						else
						{
							if (enemyFound)
							{
								if (current_turn < 20 || player_undocked_ships.size() < 5)
								{
									nFriendly = CountShipInRadius(25.0, &ship, true);
									nEnemy = CountShipInRadius(85.0, &ship, false);

									if ((nFriendly > nEnemy) && (ship.location.get_distance_to(potential_ship_targets[0]->location) > 80.0))
									{
										ship.current_behavior = new Colonize();
										ship.current_target = potential_planet_targets[0];
										ship.current_target->targeted++;
									}
									else
									{
										if (nShipHarass <= (player_undocked_ships.size() / 5))
										{
											ship.current_behavior = new Attack();
											ship.current_target = potential_ship_targets[0];
											ship.current_target->targeted++;
											nShipHarass++;
										}
										else
										{
											ship.current_behavior = new Colonize();
											ship.current_target = potential_planet_targets[0];
											ship.current_target->targeted++;
										}
									}
								}
								else
								{
									nFriendly = CountShipInRadius(25.0, &ship, true);
									nEnemy = CountShipInRadius(50.0, &ship, false);

									if (nFriendly > nEnemy)
									{
										ship.current_behavior = new Colonize();
										ship.current_target = potential_planet_targets[0];
										ship.current_target->targeted++;
									}
									else
									{
										ship.current_behavior = new Attack();
										ship.current_target = potential_ship_targets[0];
										ship.current_target->targeted++;
									}
								}

							}
							else
							{
								ship.current_behavior = new Colonize();
								ship.current_target = potential_planet_targets[0];
								ship.current_target->targeted++;
							}
						}
					}
				}


				if (rush_mode)
				{
					if (enemyFound || enemyDockedFound)
					{
						ship.current_behavior = new Attack();
						ship.current_target = potential_ship_targets[0];
						ship.current_target->targeted++;
					}
				}
				else
				{
					if (ship.current_behavior == nullptr)
					{
						if (potential_ship_targets.size() > 0)
						{
							if (potential_ship_targets[0]->docking_status != hlt::ShipDockingStatus::Undocked)
							{
								if (nShipHarass <= player_undocked_ships.size() / 4)
								{
									ship.current_behavior = new Attack();
									ship.current_target = potential_ship_targets[0];
									ship.current_target->targeted++;
									nShipHarass++;
								}
							}
							else
							{
								if (IsNearestOurDockedShip(potential_ship_targets[0]))
								{
									ship.current_behavior = new Defend();
									ship.current_target = potential_ship_targets[0];
									ship.current_target->targeted++;
								}
							}
						}

						if (ship.current_behavior == nullptr)
						{
							if (potential_planet_targets.size() > 0)
							{
								ship.current_behavior = new March();
								ship.current_target = potential_planet_targets[0];
								ship.current_target->targeted++;
							}
						}
					}
				}
			}
			else
			{
				ship.current_behavior = new Docking();
			}

			hlt::Log::log("start action");
			ship.action();
			hlt::Log::log("end action");

			hlt::Log::log("end\n");
        }

        if (!hlt::out::send_moves(moves)) 
		{
            hlt::Log::log("send_moves failed; exiting");
            break;
        }
		nShipHarass = 0;
		hlt::Log::log("send move success");
    }
}
