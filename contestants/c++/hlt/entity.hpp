#pragma once

#include "location.hpp"
#include "types.hpp"

namespace hlt {
    struct Entity {
        EntityId entity_id;
        PlayerId owner_id;
        Location location;
        int health;
        double radius;
		int score;
		int targeted;

        bool is_alive() const {
            return health > 0;
        }

		virtual int getType()
		{
			return 0;
		}

		Entity();

		Entity(const Entity& other);

		virtual int getScore() = 0;
    };
}
