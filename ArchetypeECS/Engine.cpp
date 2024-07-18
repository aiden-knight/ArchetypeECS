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

	uint64_t Engine::EntityCount()
	{
		return _nextID + 1;
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
			target->Init(_componentSizes, component, record->table, added);

			// add table to edge of previous table
			_tables[record->table].AddEdge(component, index);
		}
		else
		{
			target = &_tables[edge];
		}

		return target;
	}
	void Engine::MoveData(ComponentData* target, const size_t datumSize, const EntityRecord* record, const void* source)
	{
		// Allocate space for moved component
		size_t targetCount = target->number * datumSize;
		Buffer* tempTargetData = new Buffer[targetCount + datumSize];
		std::memcpy(tempTargetData, target->data, targetCount);
		delete[] target->data;
		target->data = nullptr;

		// move old component
		std::memcpy(tempTargetData + targetCount, source, datumSize);

		// update component data values
		target->number++;
		target->data = tempTargetData;
	}

	void Engine::RemoveData(ComponentData* source, const size_t datumSize, const size_t offset, const Buffer* position)
	{
		// Realloc source data
		size_t newSourceSize = (source->number - 1) * datumSize;
		Buffer* tempSourceData = new Buffer[newSourceSize];
		//	DEST: temp				SRC: start of source				SIZE: data up to record's component
		std::memcpy(tempSourceData, source->data, offset);
		//	DEST: temp + offset		SRC: just after source component	SIZE: data after record component to end
		std::memcpy(tempSourceData + offset, position + datumSize, newSourceSize - offset);
		delete[] source->data;
		source->data = nullptr;

		// update component data values
		source->number--;
		source->data = tempSourceData;
	}
}