#pragma once
#include <stdint.h>
#include "ComponentID.h"

namespace ECS
{
	typedef uint64_t EntityID;

	class Table;

	class EntityRecord
	{
	public:
		size_t table;
		size_t tableIndex;

		EntityRecord(size_t table)
		{
			this->table = table;
			tableIndex = -1;
		}
	};
}
