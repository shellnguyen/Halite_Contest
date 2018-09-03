#include "planet.hpp"
#include "Global.h"

int hlt::Planet::getScore()
{
	this->score = SCORE_BASE_PLANET * this->docking_spots;
	if (this->owner_id != player_id)
	{
		this->score += SCORE_ENEMY_PLANET;
	}
	return this->score;
}