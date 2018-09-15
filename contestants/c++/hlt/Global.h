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
#define MAX_ATTACK_TARGET 10
#define MAX_COLONIZE_ENEMY_PLANET 8
#define MAX_COLONIZE_NEUTRAL_PLANET 6
#define MAX_SHIP_DEFEND 3
#define MAX_SHIP_HARASS 5

extern hlt::PlayerId player_id;
extern hlt::Map* game_map;
extern std::vector<hlt::Ship> enemy_ships, enemy_docked_ship;
extern std::vector<hlt::Ship> player_ships, player_undocked_ships;
extern std::vector<hlt::Planet> neutral_planets, enemy_planets, player_planets, non_player_planets;
extern std::vector<hlt::Move> moves;
extern std::unordered_map<hlt::EntityId, hlt::EntityId> last_turn_targets;
extern int nShipHarass;
extern std::vector<hlt::Planet*> potential_planet_targets;
extern std::vector<hlt::Ship*> potential_ship_targets;
extern int current_turn;
extern bool rush_mode;

extern void UpdatePlanetList();
extern void UpdateShipList();
extern void UpdateNearbyShip();
extern void UpdateShipScore();
extern void UpdatePlanetScore();
extern void UpdateLastTurnTargets();
extern bool GetNearestPlanet(hlt::Ship* ship);
extern bool GetNearestPlayerPlanet(hlt::Ship* ship);
extern bool GetNearestNeutralPlanet(hlt::Ship* ship);
extern bool GetNearestEnemyPlanet(hlt::Ship* ship);
extern bool GetNearestPlayerPlanetForDefend(hlt::Ship* ship);
extern bool GetNearestEnemyShip(hlt::Ship* myShip);
extern bool GetNearestDockedEnemyShip(hlt::Ship* myShip);
extern int CountShipInRadius(double radius, hlt::Entity* s, bool friendlyOnly = false);
extern bool ShouldDockToPlanet(hlt::Planet* planet);
extern void SortPotentialTargets(hlt::Ship*);

#endif // !GLOBAL_H

