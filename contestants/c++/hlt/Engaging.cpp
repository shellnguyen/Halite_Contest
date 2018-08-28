#include "Engaging.h"
#include "Idle.h"
#include "Avoiding.h"
#include "Global.h"

Engaging::Engaging()
{
	m_spacing = 1;
	m_nextState = STATE::IDLE;
	m_stateIndex = STATE::ENGAGING;
}

int Engaging::getStateIndex() const
{
	return m_stateIndex;
}

bool Engaging::behavior(hlt::Ship* s)
{
	if (!s->current_target->is_alive())
	{
		m_nextState = STATE::IDLE;
		return true;
	}

	hlt::possibly<hlt::Move> move = hlt::navigation::navigate_ship_to_dock(*game_map, *s, *s->current_target, hlt::constants::MAX_SPEED);
	if (move.second)
	{
		moves.push_back(move.first);
	}

	return false;
}