#include "Moving.h"
#include "Idle.h"
#include "Global.h"

Moving::Moving()
{
	m_stateIndex = STATE::MOVING;
	m_nextState = STATE::IDLE;
}

int Moving::getStateIndex() const
{
	return m_stateIndex;
}

State* Moving::getNextState()
{
	switch (m_nextState)
	{
	case STATE::IDLE:
		return new Idle();
		break;
	case STATE::AVOIDING:
		break;
	case STATE::DOCKING:
		break;
	case STATE::ENGAGING:
		break;
	}
}

bool Moving::behavior(hlt::Ship* s)
{
	if (s->current_target->getType() == 1)
	{

	}
	else
	{
		hlt::Planet* current_target = dynamic_cast<hlt::Planet*>(s->current_target);


	}

	return false;
}