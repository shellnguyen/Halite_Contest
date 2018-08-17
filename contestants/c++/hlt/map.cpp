#include "map.hpp"

namespace hlt {
    Map::Map(const int width, const int height) : map_width(width), map_height(height) 
	{
    }

	Map::Map(const Map& other)
	{
		this->map_width = other.map_width;
		this->map_height = other.map_height;

		// copy the base planet and ship objects first
		for (int i = 0; i < other.planets.size(); ++i) 
		{
			hlt::Planet p = Planet(other.planets[i]);
			this->planets.push_back(p);
			this->planet_map[p.entity_id] = i;
		}

		for (int i = 0; i < other.ships.size(); ++i) 
		{
			std::vector<hlt::Ship>& ship_vec = this->ships[i];
			entity_map<unsigned int>& ship_e_map = this->ship_map[i];

			int n_ships = other.ships.at(i).size();
			ship_vec.reserve(n_ships);
			for (int j = 0; j < n_ships; ++j) 
			{
				hlt::Ship s = Ship(other.ships.at(i).at(j));
				ship_vec.push_back(s);
				ship_e_map[s.entity_id] = j;
			}
		}

		//Need to figure out a way to merge in_range_enemies/alllies
		//for (int i = 0; i < other.ships.size(); ++i)
		//{
		//	for (hlt::Ship& s : this->ships.at(i))
		//	{
		//		for (int j = 0; j < s.in_range_enemies.size(); ++j)
		//		{
		//			const hlt::Ship enemyShip = this->get_ship(s.in_range_enemies[j].owner_id, s.in_range_enemies[j].entity_id);
		//		}
		//	}
		//}
	}
}
