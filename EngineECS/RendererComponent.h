#pragma once

#include "private.h"
#include "constants.h"
#include "Component.h"

namespace engineECS
{
	struct RendererComponent : public Component<RendererComponent, MaxEntities>
	{
	public:
		int isEnabled : 1;
		int isSkeletal : 1;
		int receivesShadow : 1;
		int makesShadow : 1;
		unsigned int flags1 : 12;
		unsigned int meshType : 16; //65.536?
		unsigned int flags2 : 16;
		unsigned int programType : 16; //65.536?
	};
}
