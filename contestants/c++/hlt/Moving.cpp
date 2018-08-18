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

void Moving::behavior(const hlt::Ship s)
{

}