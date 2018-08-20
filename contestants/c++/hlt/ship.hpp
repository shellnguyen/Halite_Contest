#pragma once

#include "constants.hpp"
#include "types.hpp"
#include "planet.hpp"

class State;

namespace hlt {
    /// The states a ship can be in regarding docking.
    enum class ShipDockingStatus {
        Undocked = 0,
        Docking = 1,
        Docked = 2,
        Undocking = 3,
    };

    struct Ship : Entity {
        /// The turns left before the ship can fire again.
        int weapon_cooldown;

        ShipDockingStatus docking_status;

        /// The number of turns left to complete (un)docking.
        int docking_progress;

        /// The id of the planet this ship is docked to. Only valid if
        /// Ship::docking_status is -not- DockingStatus::Undocked.
        EntityId docked_planet;

        /// Check if this ship is close enough to dock to the given planet.
        bool can_dock(const Planet& planet) const {
            return location.get_distance_to(planet.location) <= (constants::SHIP_RADIUS + constants::DOCK_RADIUS + planet.radius);
        }

		//Entity* last_target;
		//Entity* current_target;
		State* current_state;

		std::vector<hlt::Ship> in_range_enemies;
		std::vector<hlt::Ship> in_range_allies;

		Ship();
		Ship(const Ship& other);

		void action();
    };
}
