#include "Docking.h"
#include "Idle.h"

Docking::Docking()
{
	m_stateIndex = STATE::DOCKING;
}

int Docking::getStateIndex() const
{
	return m_stateIndex;
}

bool Docking::behavior(hlt::Ship* s)
{
	return false;
}