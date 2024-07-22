#pragma once
#include <unordered_map>

#include "Entity.h"
#include "ComponentID.h"
#include "Component.h"

namespace ECS
{
	using std::unordered_map;
	using std::vector;

	// holds a table containing all the components
	class Table
	{
	// FUNCTIONS
	public:
		Table(const Type& previousType, size_t index);
		void Init(unordered_map<ComponentID, size_t>& componentSizes, ComponentID changed, size_t previousTable, bool added);
		
		/// <summary>
		/// Calls delete[] on all ComponentData buffers, can't be called from Destructor due to Table being stored in vector
		/// </summary>
		void DeleteComponents();

		void RemoveEntity(size_t index);

		size_t GetEdge(ComponentID component) const;
		void AddEdge(ComponentID component, size_t edge);

		ComponentData* GetComponentData(ComponentID component);
		void AddRecord(EntityRecord* record);

		size_t GetComponentAmount();
		EntityID GetEntity(size_t index);

	// VARIABLES
	public:
		Type type;

	private:
		/// <summary>
		/// The index of where the table is stored in the _tables vector in engine class
		/// </summary>
		size_t _index;

		vector<EntityRecord*> _records;
		unordered_map<ComponentID, ComponentData> _components;
		unordered_map<ComponentID, size_t> _edges;
	};
}