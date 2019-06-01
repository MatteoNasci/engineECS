#include "Entity.h"

#include <iostream>

static std::ostream& operator<<(std::ostream& lhs, const engineECS::Entity& rhs)
{
	return lhs << rhs.id;
}