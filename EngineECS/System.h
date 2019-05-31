#pragma once

#include <functional>

#include "private.h"
#include "Entity.h"

namespace engineECS
{
	struct System final
	{
	public:
		const SYSTEM_FUNCTION& getCallback() const;

		System(const SYSTEM_FUNCTION& inCallback);
		System();
		~System();
	private:
		const SYSTEM_FUNCTION& callback;

	};
}