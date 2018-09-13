#ifndef GLOBAL_H
#define GLOBAL_H

#include "hlt.hpp"
#include "navigation.hpp"
#include <vector>

#define SAFE_DELETE(p) if ((p) != NULL) { delete (p); (p) = NULL; }

//Score for ship
#define SCORE_FRIENDLY_DOCKED_SHIP 40
#define SCORE_ENEMY_DOCKED_SHIP 50
#define SCORE_BASE_SHIP 5

//Score for planet
#define SCORE_BASE_PLANET 50
#define SCORE_ENEMY_PLANET 30
#define SCORE_PER_DOCKING_SPOT 6

//Other
#define MAX_TARGETED 10
#define MAX_SHIP_DEFEND 3
#define MAX_SHIP_HARASS 5

extern hlt::PlayerId player_id;
extern hlt::Map* game_map;
extern std::vector<hlt::Ship> enemy_ships, enemy_docked_ship;
extern std::vector<hlt::Ship> player_ships, player_undocked_ships;
extern std::vector<hlt::Planet> neutral_planets, enemy_planets, player_planets, non_player_planets;
extern std::vector<hlt::Move> moves;
extern int nShipHarass;

extern void UpdatePlanetList();
extern void UpdateShipList();
extern void UpdateNearbyShip();
extern void UpdateShipScore();
extern void UpdatePlanetScore();
extern hlt::Planet GetNearestPlanet(hlt::Ship* ship);
extern hlt::Planet GetNearestPlayerPlanet(hlt::Ship* ship);
extern hlt::Ship GetNearestEnemyShip(hlt::Ship* myShip);
extern hlt::Ship GetNearestDockedEnemyShip(hlt::Ship* myShip);
extern int CountShipInRadius(double radius, hlt::Entity* s, bool friendlyOnly = false);
extern bool ShouldDockToPlanet(hlt::Planet* planet);

#endif // !GLOBAL_H

