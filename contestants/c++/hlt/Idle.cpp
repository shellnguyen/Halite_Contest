#include "Idle.h"
#include "Moving.h"
#include "planet.hpp"
#include "Global.h"

Idle::Idle()
{
	m_stateIndex = STATE::IDLE;
}

int Idle::getStateIndex() const
{
	return m_stateIndex;
}

State* Idle::getNextState()
{
	return new Moving();
}

void Idle::behavior(const hlt::Ship s)
{
}
