#ifndef GLOBAL_H
#define GLOBAL_H

#include "hlt.hpp"
#include <vector>

extern hlt::Map* game_map;
extern std::vector<hlt::Ship> enemy_ships, player_ships;
extern std::vector<hlt::Planet> neutral_planets, enemy_planets, player_planets, non_player_planets;

extern hlt::Planet GetNearestPlanet(const hlt::Map map, const hlt::Ship ship);
extern hlt::Ship GetNearestEnemyShip(const hlt::Map map, const hlt::Ship myShip);

#endif // !GLOBAL_H

