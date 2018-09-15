#include "ship.hpp"
#include "log.hpp"
#include "hlt.hpp"
#include "Global.h"
#include "Behavior.h"

hlt::Ship::Ship() : Entity()
{
	this->docking_status = hlt::ShipDockingStatus::Undocked;
	this->docking_progress = -1;
	this->docked_planet = -1;
	this->weapon_cooldown = 0;
	//this->last_target = NULL;
	this->current_target = NULL;
	this->current_behavior = NULL;
	this->score = health;
}

hlt::Ship::Ship(const Ship& other) : Entity(other)
{
	this->docking_status = other.docking_status;
	this->docked_planet = other.docked_planet;
	this->docking_progress = other.docking_progress;
	this->weapon_cooldown = other.weapon_cooldown;

	//if (other.current_target)
	//{
	this->current_target = other.current_target;
	//}
	//if (other.last_target)
	//{
	//	this->last_target = other.current_target;
	//}
	this->current_behavior = other.current_behavior;
	this->score = other.score;

	this->score = SCORE_BASE_SHIP;

	//if (other.current_state)
	//{
	//	
	//}

	//if (other.in_range_enemies.size() > 0)
	//{
	//	this->in_range_enemies.assign(other.in_range_enemies.begin(), other.in_range_enemies.end());
	//}

	//if (other.in_range_allies.size() > 0)
	//{
	//	this->in_range_allies.assign(other.in_range_allies.begin(), other.in_range_allies.end());
	//}
}

hlt::Ship::~Ship()
{
	SAFE_DELETE(current_behavior);
	//SAFE_DELETE(current_target);
	//hlt::Log::log("after delete current_target");
}

void hlt::Ship::action()
{
	if (this->current_behavior)
	{
		hlt::Log::log("ship.behavior = " + to_string(this->current_behavior->getType()));
		if (this->current_target)
		{
			hlt::Log::log("ship.targetId = " + to_string(this->current_target->entity_id));
			hlt::Log::log("ship.targetType = " + to_string(this->current_target->getType()));
		}
		
		if (!this->current_behavior->action(this))
		{
			hlt::Log::log("ship[" + to_string(this->entity_id) + "] do action failed !!!");
		}
	}
	else
	{
		hlt::Log::log("assign task went wrong !!!");
	}
}

double hlt::Ship::getScore() const
{
	return this->score;
}