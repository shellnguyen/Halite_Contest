#include "entity.hpp"

hlt::Entity::Entity()
{
		this->entity_id = -1;
		this->owner_id = -999;
		this->location.pos_x = -1;
		this->location.pos_y = -1;
		this->health = 0;
		this->radius = 0;
		this->score = 0;
		this->targeted = 0;
}

hlt::Entity::Entity(const Entity& other)
{
	this->entity_id = other.entity_id;
	this->owner_id = other.owner_id;
	this->location.pos_x = other.location.pos_x;
	this->location.pos_y = other.location.pos_y;
	this->health = other.health;
	this->radius = other.radius;
	this->score = other.score;
	this->targeted = other.targeted;
}