#include "Engine.h"
#include "System.h"

#include <algorithm>

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

		for (BaseSystem* system : _systems)
		{
			delete system;
		}
	}

	EntityID Engine::Entity()
	{
		EntityID newEntity = _nextID++;

		// create record for new entity and add it to empty table
		_records.emplace_back(newEntity, 0);
		_tables[0].AddRecord(&_records.back());

		// return entity id generated
		return newEntity;
	}

	uint64_t Engine::EntityCount() const
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
			target->Init(_componentInfos, component, record->table, added);

			// add table to edge of previous table
			_tables[record->table].AddEdge(component, index);
		}
		else
		{
			target = &_tables[edge];
		}

		return target;
	}

	void Engine::RunSystems()
	{
		for (BaseSystem* system : _systems)
		{
			system->Run();
		}
	}

	forward_list<Table*> Engine::GetTables(Type subType)
	{
		forward_list<Table*> tables;

		for (Table& table : _tables)
		{
			// checks if subType is subset of type
			if (std::includes(table.type.begin(), table.type.end(), subType.begin(), subType.end()))
			{
				tables.push_front(&table);
			}
		}

		return tables;
	}

	void Engine::MoveData(ComponentData* target, const size_t datumSize, const EntityRecord* record, const void* source)
	{
		// Allocate space for moved component
		if(target->number == target->allocated)
		{
			size_t targetCount = target->number * datumSize;
			Buffer* tempTargetData = new Buffer[targetCount + datumSize * target->allocSize];
			std::memcpy(tempTargetData, target->data, targetCount);
			delete[] target->data;
			target->data = nullptr;

			target->data = tempTargetData;
			target->allocated += target->allocSize;

			Logger::Log("Allocated more memory, amount: " + std::to_string(target->allocated));
		}

		// move old component
		Buffer* targetPos = static_cast<Buffer*>(target->data) + target->number * datumSize;
		std::memcpy(targetPos, source, datumSize);

		// update component data values
		target->number++;
	}

	void Engine::RemoveData(ComponentData* source, const size_t datumSize, const size_t offset, const Buffer* position)
	{
		// Realloc source data
		size_t sourceSize = source->allocated * datumSize;
		Buffer* sourceBuffer = static_cast<Buffer*>(source->data);
		//	DEST: source			SRC: start of source				SIZE: data up to record's component
		std::memcpy(sourceBuffer, source->data, offset);
		//	DEST: source + offset	SRC: just after source component	SIZE: data after record component to end
		std::memcpy(sourceBuffer + offset, position + datumSize, sourceSize - offset);

		// update component data values
		source->number--;
	}
}