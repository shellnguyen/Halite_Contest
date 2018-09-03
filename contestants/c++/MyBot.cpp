#include "hlt/navigation.hpp"
#include "hlt/Global.h"
#include "hlt/Colonize.h"
#include "hlt/Attack.h"
#include "hlt/March.h"
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


    for (;;) {
        moves.clear();
        hlt::Map map = hlt::in::get_map();
		game_map = &map;
		UpdatePlanetList();
		UpdateShipList();

		hlt::Log::log("player_ships.size = " + to_string(player_ships.size()));
        for (hlt::Ship& ship : player_ships)
		{
			hlt::Log::log("start");
            if (ship.docking_status != hlt::ShipDockingStatus::Undocked) 
			{
                continue;
            }

			int nFriendly = 0;
			int nEnemy = 0;

			hlt::Planet nearestPlanet = GetNearestPlanet(&ship);
			hlt::Ship nearestEnemy = GetNearestEnemyShip(&ship);

			if (nearestPlanet.entity_id != -1)
			{
				hlt::Log::log("found planet");
				if (ship.can_dock(nearestPlanet))
				{
					hlt::Log::log("can dock planet");
					if (!nearestPlanet.owned)
					{
						hlt::Log::log("planet not owned");
						nFriendly = CountShipInRadius(25.0, &ship, true);
						nEnemy = CountShipInRadius(35.0, &ship);

						if (nFriendly > nEnemy)
						{
							if (ShouldDockToPlanet(&nearestPlanet))
							{
								ship.current_behavior = new Colonize();
								ship.current_target = &nearestPlanet;
							}
							else
							{
								ship.current_behavior = new Attack();
								ship.current_target = &nearestEnemy;
							}
						}
					}
					else
					{
						if (nearestPlanet.owner_id != player_id)
						{
							hlt::Log::log("planet belong to enemy");
							//nFriendly = CountShipInRadius(25.0, &ship, true);
							hlt::Log::log("friendly = " + to_string(nFriendly));
							//nEnemy = CountShipInRadius(35.0, &ship);
							hlt::Log::log("enemy = " + to_string(nFriendly));

							//if (nFriendly > nEnemy)
							//{
								hlt::Log::log("friendly > enemy");
								ship.current_behavior = new Attack();
								if (ship.in_range_enemies.size() > 0)
								{
									ship.current_target = &ship.in_range_enemies[0];
								}
								else
								{
									ship.current_target = &nearestEnemy;
								}
							//}
						}
						else
						{
							if (!nearestPlanet.is_full())
							{
								hlt::Log::log("planet belong to us and not full");
								ship.current_behavior = new Colonize();
								ship.current_target = &nearestPlanet;
							}
						}
					}
				}

				if (!ship.current_behavior)
				{
					hlt::Log::log("move to planet");
					ship.current_behavior = new March();
					ship.current_target = &nearestPlanet;
				}
				else
				{
					hlt::Log::log("wtf");
				}
			}
			else
			{
				if (nearestEnemy.entity_id != -1)
				{
					ship.current_behavior = new Attack();
					ship.current_target = &nearestEnemy;
				}
			}

			hlt::Log::log("start action");
			ship.action();
			hlt::Log::log("end action");
        }

        if (!hlt::out::send_moves(moves)) 
		{
            hlt::Log::log("send_moves failed; exiting");
            break;
        }
		hlt::Log::log("send move success");
    }
}
