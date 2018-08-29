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


    for (;;) {
		hlt::Log::log("begin");
        moves.clear();
		hlt::Log::log("clear map");
        hlt::Map map = hlt::in::get_map();
		hlt::Log::log("get new map");
		game_map = &map;
		hlt::Log::log("after merger map");
		UpdatePlanetList();
		hlt::Log::log("after UpdatePlanetList");
		UpdateShipList();
		hlt::Log::log("after UpdateShipList");
		//UpdateNearbyShip();

		hlt::Log::log("player_ships.size = " + to_string(player_ships.size()));
        for (hlt::Ship& ship : player_ships)
		{
			hlt::Log::log("start");
            if (ship.docking_status != hlt::ShipDockingStatus::Undocked) 
			{
                continue;
            }

			hlt::Log::log("not docking");
			if (!ship.current_state)
			{
				hlt::Log::log("current state = Idle");
				ship.current_state = new Idle();
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
