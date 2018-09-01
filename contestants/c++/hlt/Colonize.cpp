#include "Colonize.h"
#include "Global.h"

int Colonize::getType() const
{
	return BehaviorType::COLONIZE;
}

bool Colonize::action(hlt::Ship* s)
{
	if (s->current_target != nullptr)
	{
		moves.push_back(hlt::Move::dock(s->entity_id, s->current_target->entity_id));
		return true;
	}
	return false;
}
