#include "ship.hpp"

hlt::Ship::Ship() : Entity()
{
	this->docking_status = hlt::ShipDockingStatus::Undocked;
	this->docking_progress = -1;
	this->docked_planet = -1;
	this->weapon_cooldown = 0;
	this->last_target = NULL;
	this->current_target = NULL;
	this->current_state = NULL;
}

hlt::Ship::Ship(const Ship& other) : Entity(other)
{
	this->docking_status = other.docking_status;
	this->docked_planet = other.docked_planet;
	this->docking_progress = other.docking_progress;
	this->weapon_cooldown = other.weapon_cooldown;

	if (other.current_target)
	{
		this->current_target = other.current_target;
	}
	if (other.last_target)
	{
		this->last_target = other.current_target;
	}

	if (other.current_state)
	{
		this->current_state = other.current_state;
	}
}