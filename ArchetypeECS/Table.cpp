#include "Table.h"
#include <algorithm>
#include <cstdlib>

#include "Logger.h"

namespace ECS
{
	Table::Table(const Type& previousType, size_t index)
	{
		type = previousType;
		_index = index;
	}

	void Table::Init(unordered_map<ComponentID, size_t>& componentSizes, ComponentID changed, size_t previousTable, bool added)
	{
		_edges.emplace(changed, previousTable);

		if (added)	type.insert(changed);
		else		type.erase(changed);

		// create table's columns
		for (ComponentID component : type)
		{
			_components.emplace(std::make_pair(component, ComponentData(componentSizes[component])));
		}
	}

	void Table::DeleteComponents()
	{
		for (auto& pair : _components)
		{
			delete[] pair.second.data;
			pair.second.data = nullptr;
		}
	}

	void Table::RemoveEntity(size_t index)
	{
		if (!type.empty())
		{
			// remove pointer to entities record
			_records.erase(_records.begin() + index);

			// decrement all other entities record's indexes
			for (auto it = _records.begin() + index; it != _records.end(); ++it)
			{
				(*it)->tableIndex--;
			}
		}
	}

	size_t Table::GetEdge(ComponentID component) const
	{
		auto pairIt = _edges.find(component);
		if (pairIt == _edges.end())
		{
			return -1;
		}
		else
		{
			return pairIt->second;
		}
	}
	void Table::AddEdge(ComponentID component, size_t edge)
	{
		_edges.emplace(component, edge);
	}

	ComponentData* Table::GetComponentData(ComponentID component)
	{
		return &_components[component];
	}

	void Table::AddRecord(EntityRecord* record)
	{
		if (type.empty())
		{
			record->tableIndex = -1;
		}
		else
		{
			record->tableIndex = _records.size();
		}

		record->table = _index;
		_records.push_back(record);
	}
}
