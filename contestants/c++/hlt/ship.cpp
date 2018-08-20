#include "ship.hpp"
#include "State.h"

hlt::Ship::Ship() : Entity()
{
	this->docking_status = hlt::ShipDockingStatus::Undocked;
	this->docking_progress = -1;
	this->docked_planet = -1;
	this->weapon_cooldown = 0;
	//this->last_target = NULL;
	this->current_target = NULL;
	this->current_state = NULL;
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
	this->current_state = other.current_state;

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

void hlt::Ship::action()
{
	if (this->current_state->behavior(this))
	{
		this->current_state = this->current_state->getNextState();
	}
}