#ifndef ENGAGING_H
#define ENGAGING_H

#include "State.h"

class Engaging : public State
{
private:
	int m_spacing;
public:
	Engaging();
	int getStateIndex() const override;
	bool behavior(hlt::Ship* s) override;
};

#endif // !ENGAGING_H

