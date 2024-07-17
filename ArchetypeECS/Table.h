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
		void Init(ComponentID changed, size_t previousTable, bool added);
		
		void DeleteComponents();

		void RemoveEntity(size_t index);

		size_t GetEdge(ComponentID component) const;
		void AddEdge(ComponentID component, size_t edge);

		ComponentData* GetComponentData(ComponentID component);
		void AddRecord(EntityRecord* record);

	// VARIABLES
	public:
		Type type;

		static unordered_map<ComponentID, size_t> ComponentSizes;

	private:
		size_t _index;

		vector<EntityRecord*> _records;
		unordered_map<ComponentID, ComponentData> _components;
		unordered_map<ComponentID, size_t> _edges;
	};
}