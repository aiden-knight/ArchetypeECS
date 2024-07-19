#pragma once
#include <vector>
#include <unordered_map>
#include <cstring>
#include <forward_list>
#include <type_traits>

#include "Logger.h"
#include "Table.h"
#include "ComponentID.h"
#include "Entity.h"

namespace ECS
{
	typedef unsigned char Buffer;
	using std::unordered_map;
	using std::vector;
	using std::forward_list;

	class System;

	class Engine
	{
	// FUNCTIONS
	public:
		/// <summary>
		/// Creates empty table where entities are placed when created
		/// </summary>
		Engine();

		/// <summary>
		/// Deletes all component data from tables
		/// </summary>
		~Engine();

		/// <summary>
		/// Creates new entity
		/// </summary>
		/// <returns>EntityID of new entity</returns>
		EntityID Entity();

		/// <summary>
		/// Gets the current amount of entity's
		/// </summary>
		/// <returns></returns>
		uint64_t EntityCount();

		/// <summary>
		/// Get's the table the entity should move into given component to add or remove
		/// </summary>
		/// <param name="record">Entity's record</param>
		/// <param name="component">Component added or removed</param>
		/// <param name="added">Whether the component was added or removed</param>
		/// <returns>Pointer to table entity should now belong to</returns>
		Table* GetTarget(EntityRecord* record, ComponentID component, bool added);


		void InitSystems();

		/// <summary>
		/// Runs systems
		/// </summary>
		void RunSystems();

		/// <summary>
		/// Gets list of tables that have subType as a subset of their type
		/// </summary>
		/// <param name="subType">Type looking for</param>
		/// <returns>Linked list of tables that match</returns>
		forward_list<Table*> GetTables(Type subType); // @TODO substitute with some sort of query that only checks for new tables if changed?
	
	private:
		void MoveData(ComponentData* target, const size_t datumSize, const EntityRecord* record, const void* source);
		void RemoveData(ComponentData* source, const size_t datumSize, const size_t offset, const Buffer* position);

	// VARIABLES
	private:
		vector<Table> _tables;
		vector<EntityRecord> _records;
		vector<System*> _systems;

		/// <summary>
		/// When a component is registered their size is stored here for table initialisation
		/// </summary>
		unordered_map<ComponentID, size_t> _componentSizes;

		EntityID _nextID = 0;

	// TEMPLATES
	public:
		/// <summary>
		/// Registers system to run in the ECS Engine
		/// </summary>
		/// <typeparam name="DerivedSystem">System to register, must derive from class ECS::System</typeparam>
		template<typename DerivedSystem>
		void RegisterSystem()
		{
			static_assert(std::is_base_of<System, DerivedSystem>::value, "Engine::RegisterSystem called with class not deriving from Sytem");
			_systems.emplace_back(new DerivedSystem);
		}

		/// <summary>
		/// Registers type as a component
		/// </summary>
		/// <typeparam name="Component">Type of component to register</typeparam>
		/// <returns>The generated component ID</returns>
		template<typename Component>
		ComponentID RegisterComponent()
		{
			ComponentID createdID = StaticID<Component>::Init();
			_componentSizes.emplace(createdID, sizeof(Component));
			return createdID;
		}
		
		/// <summary>
		/// Get componentID of component, given it has been registered
		/// </summary>
		/// <typeparam name="Component">Component type to get ID for</typeparam>
		/// <returns>ComponentID or StaticID<Component>::null </returns>
		template<typename Component>
		ComponentID GetComponentID()
		{
			return StaticID<Component>::GetID();
		}

		/// <summary>
		/// Gets component from entity if both entity exists and has component
		/// </summary>
		/// <typeparam name="Component">Type of component to get</typeparam>
		/// <param name="entity">EnityID of entity to get component of</param>
		/// <returns>Pointer to entity's component or nullptr if not found</returns>
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

			// get component data from table then extract entity's component
			ComponentData* components = recordTable->GetComponentData(componentID);
			Buffer* componentPos = static_cast<Buffer*>(components->data) + record.tableIndex * components->datumSize;
			return reinterpret_cast<Component*>(componentPos);
		}

		/// <summary>
		/// Checks validity of entity and component, also checks components existance
		/// Used for component operations such as AddComponent and RemoveComponent
		/// </summary>
		/// <typeparam name="Component"></typeparam>
		/// <param name="entity"></param>
		/// <param name="shouldExist"></param>
		/// <param name="source"></param>
		/// <param name="outID"></param>
		/// <param name="outRecord"></param>
		/// <returns></returns>
		template<typename Component>
		bool CheckComponent(EntityID entity, bool shouldExist, std::string source, ComponentID& outID, EntityRecord* &outRecord)
		{
			// check if entity record exists
			if (_records.size() <= entity)
			{
				Logger::Warning("Entity record not found {" + source + "}");
				return false;
			}

			// get component id and check if valid component
			outID = GetComponentID<Component>();
			if (outID == StaticID<Component>::null)
			{
				Logger::Warning("Component not registered {" + source + "}");
				return false;
			}

			// check if entity has component
			// shouldExist = 1 (true), count should be 1
			// shouldExist = 0 (false), count should be 0 
			outRecord = &_records[entity];
			if (_tables[outRecord->table].type.count(outID) != shouldExist)
			{
				std::string exist = shouldExist ? "true" : "false";
				Logger::Warning("Component existance error, should exist = " + exist + " {" + source + "}");
				return false;
			}

			return true;
		}

		/// <summary>
		/// Adds component to given entity, then moves entity to correct table.
		/// </summary>
		/// <typeparam name="Component">Component type to add</typeparam>
		/// <param name="entity">EntityID of entity to add component to</param>
		/// <param name="defaultValue">Component values the given entity will acquire, defaulted to default constructor</param>
		template<typename Component>
		void AddComponent(EntityID entity, Component&& defaultValue = Component())
		{
			ComponentID componentID;
			EntityRecord* record = nullptr;

			// check for validity of operation
			if (!CheckComponent<Component>(entity, false, "Engine::AddComponent", componentID, record))
				return;


			// get the table the entity needs to move to
			Table* target = GetTarget(record, componentID, true);

			// correct index of entities in old table
			Table* recordTable = &_tables[record->table];
			recordTable->RemoveEntity(record->tableIndex);

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
					// Add component to new table with given default value
					MoveData(targetData, datumSize, record, &defaultValue);
					continue;
				}

				ComponentData* sourceData = recordTable->GetComponentData(component);

				// get position and offset of component in source data
				size_t sourceCompOffset = datumSize * record->tableIndex;
				Buffer* sourceComponentPos = static_cast<Buffer*>(sourceData->data) + sourceCompOffset;

				// Move component to target table first
				MoveData(targetData, datumSize, record, sourceComponentPos);

				// Remove component from old table
				RemoveData(sourceData, datumSize, sourceCompOffset, sourceComponentPos);
			}

			// update record
			target->AddRecord(record);
		}

		/// <summary>
		/// Removes component from given entity, then moves entity to correct table.
		/// </summary>
		/// <typeparam name="Component">Component type to remove</typeparam>
		/// <param name="entity">EntityID of entity to remove component from</param>
		template<typename Component>
		void RemoveComponent(EntityID entity)
		{
			ComponentID componentID;
			EntityRecord* record = nullptr;

			// check for validity of operation
			if (!CheckComponent<Component>(entity, true, "Engine::RemoveComponent", componentID, record))
				return;

			// get the table the entity needs to move to
			Table* target = GetTarget(record, componentID, false);

			// correct index of entities in old table
			Table* recordTable = &_tables[record->table];
			recordTable->RemoveEntity(record->tableIndex);

			// setup components for entity in new table
			for (ComponentID component : recordTable->type)
			{
				// get component data of current component
				ComponentData* sourceData = recordTable->GetComponentData(component);

				// size of a single component
				size_t datumSize = sourceData->datumSize;

				// get position and offset of component in source data
				size_t sourceCompOffset = datumSize * record->tableIndex;
				Buffer* sourceComponentPos = static_cast<Buffer*>(sourceData->data) + sourceCompOffset;

				// removed component
				if (component == componentID)
				{
					// Remove component from old table
					RemoveData(sourceData, datumSize, sourceCompOffset, sourceComponentPos);
					continue;
				}

				ComponentData* targetData = target->GetComponentData(component);

				// Move component to target table first
				MoveData(targetData, datumSize, record, sourceComponentPos);

				// Remove component from old table
				RemoveData(sourceData, datumSize, sourceCompOffset, sourceComponentPos);
			}

			// update record
			target->AddRecord(record);
		}
	};
}
