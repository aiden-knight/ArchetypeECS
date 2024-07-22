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
		const EntityID entity;

		EntityRecord(EntityID entity, size_t table) :
			entity{entity}
		{
			this->table = table;
			tableIndex = -1;
		}
	};
}
