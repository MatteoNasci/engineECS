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
		unsigned int flags1 : 15;
		unsigned int meshType : 13;//8192 //65.536?
		unsigned int flags2 : 19;
		unsigned int programType : 13;//8192 //65.536?
	};
}
