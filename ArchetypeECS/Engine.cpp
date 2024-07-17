#include "Engine.h"

namespace ECS
{
	Engine::Engine()
	{
		_tables.emplace_back(Table({}, 0));
	}

	Engine::~Engine()
	{
		for (Table& table : _tables)
		{
			table.DeleteComponents();
		}
	}

	EntityID Engine::Entity()
	{
		// create record for new entity and add it to empty table
		_records.emplace_back(0);
		_tables[0].AddRecord(&_records.back());

		// return entity id generated
		return _nextID++;
	}

	Table* Engine::GetTarget(EntityRecord* record, ComponentID component, bool added)
	{
		Table* target;

		size_t edge = _tables[record->table].GetEdge(component);
		if (edge == -1)
		{
			// create new table
			size_t index = _tables.size();

			_tables.emplace_back(_tables[record->table].type, index);
			target = &_tables.back();
			target->Init(component, record->table, added);

			// add table to edge of previous table
			_tables[record->table].AddEdge(component, index);
		}
		else
		{
			target = &_tables[edge];
		}

		return target;
	}
}

