#ifndef GLOBAL_H
#define GLOBAL_H

#include "hlt.hpp"
#include "Idle.h"
#include "navigation.hpp"
#include <vector>

extern hlt::PlayerId player_id;
extern hlt::Map* game_map;
extern std::vector<hlt::Ship> enemy_ships, player_ships;
extern std::vector<hlt::Planet> neutral_planets, enemy_planets, player_planets, non_player_planets;
extern std::vector<hlt::Move> moves;

extern void UpdatePlanetList();
extern void UpdateShipList();
extern void UpdateNearbyShip();
extern hlt::Planet GetNearestPlanet(const hlt::Ship ship);
extern hlt::Ship GetNearestEnemyShip(const hlt::Ship myShip);

#endif // !GLOBAL_H

