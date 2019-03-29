#include "Entity.h"
#include <iostream>
static std::ostream& operator<<(std::ostream& LHS, const engineECS::Entity& RHS)
{
	return LHS << RHS.Id;
}
