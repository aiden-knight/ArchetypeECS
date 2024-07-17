#pragma once
#include <vector>
#include <cstring>

#include "Logger.h"
#include "Table.h"
#include "ComponentID.h"
#include "Entity.h"

namespace ECS
{
	typedef unsigned char Buffer;
	using std::vector;

	class Engine
	{
	public:
		Engine();
		~Engine();

		EntityID Entity();

		Table* GetTarget(EntityRecord* record, ComponentID component, bool added);

	private:
		vector<Table> _tables;
		vector<EntityRecord> _records;

		EntityID _nextID = 0;

	// TEMPLATES
	public:
		template<typename Component>
		ComponentID RegisterComponent()
		{
			ComponentID createdID = StaticID<Component>::Init();
			Table::ComponentSizes.emplace(createdID, sizeof(Component));
			return createdID;
		}

		template<typename Component>
		ComponentID GetComponentID()
		{
			return StaticID<Component>::GetID();
		}

		template<typename Component>
		Component* GetComponent(EntityID entity)
		{
			// check if entity record exists
			if (_records.size() <= entity)
			{
				Logger::Warning("Entity record not found {Engine::GetComponent}");
				return nullptr;
			}

			// check if entity has component
			ComponentID componentID = GetComponentID<Component>();
			EntityRecord& record = _records[entity];
			Table* recordTable = &_tables[record.table];
			if (recordTable->type.count(componentID) == 0)
			{
				Logger::Warning("Entity does not have component requested {Engine::GetComponent}");
				return nullptr;
			}

			// get component data from table then extract entitie's component
			ComponentData* components = recordTable->GetComponentData(componentID);
			Buffer* componentPos = static_cast<Buffer*>(components->data) + record.tableIndex * components->datumSize;
			return reinterpret_cast<Component*>(componentPos);
		}

		template<typename Component>
		void AddComponent(EntityID entity, Component defaultValue = Component())
		{
			// check if entity record exists
			if (_records.size() <= entity)
			{
				Logger::Warning("Entity record not found {Engine::AddComponent}");
				return;
			}

			// get component id and check if entity's table already has that component
			ComponentID componentID = GetComponentID<Component>();
			EntityRecord& record = _records[entity];
			if (_tables[record.table].type.count(componentID) != 0)
			{
				Logger::Warning("Entity already has component {Engine::AddComponent}");
				return;
			}

			// get the table the entity needs to move to
			Table* target = GetTarget(&record, componentID, true);

			// correct index of entities in old table
			Table* recordTable = &_tables[record.table];
			recordTable->RemoveEntity(record.tableIndex);

			// setup components for entity in new table
			for (ComponentID component : target->type)
			{
				// get component data of current component
				ComponentData* targetData = target->GetComponentData(component);

				// size of a single component
				size_t datumSize = targetData->datumSize;

				// new component
				if (component == componentID)
				{
					// Allocate space for moved component
					size_t targetCount = targetData->number * datumSize;
					Buffer* tempTargetData = new Buffer[targetCount + datumSize];
					std::memcpy(tempTargetData, targetData->data, targetCount);
					delete[] targetData->data;
					targetData->data = nullptr;

					targetData->number++;
					targetData->data = tempTargetData;

					// set component to default value
					Buffer* componentPos = static_cast<Buffer*>(targetData->data) + targetCount;
					std::memcpy(componentPos, &defaultValue, datumSize);

					continue;
				}

				ComponentData* sourceData = recordTable->GetComponentData(component);

				// Allocate space for moved component
				size_t targetCount = targetData->number * datumSize;
				Buffer* tempTargetData = new Buffer[targetCount + datumSize];
				std::memcpy(tempTargetData, targetData->data, targetCount);
				delete[] targetData->data;
				targetData->data = nullptr;

				// move old component
				size_t sourceCompOffset = datumSize * record.tableIndex;
				Buffer* sourceComponentPos = static_cast<Buffer*>(sourceData->data) + sourceCompOffset;
				std::memcpy(tempTargetData + targetCount, sourceComponentPos, datumSize);

				// Realloc source data
				size_t newSourceSize = (sourceData->number - 1) * datumSize;
				Buffer* tempSourceData = new Buffer[newSourceSize];
				//	DEST: temp				SRC: start of source				SIZE: data up to record's component
				std::memcpy(tempSourceData, sourceData->data, sourceCompOffset);
				//	DEST: temp + offset		SRC: just after source component	SIZE: data after record component to end
				std::memcpy(tempSourceData + sourceCompOffset, sourceComponentPos + datumSize, newSourceSize - sourceCompOffset);
				delete[] sourceData->data;
				sourceData->data = nullptr;

				// update component data values
				targetData->number++;
				targetData->data = tempTargetData;
				sourceData->number--;
				sourceData->data = tempSourceData;
			}

			// update record
			target->AddRecord(&record);
		}
	};
}
