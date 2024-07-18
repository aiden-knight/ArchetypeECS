#include "PrintSystem.h"
#include "Structures.h"

#include <forward_list>
#include "Engine.h"
#include "Table.h"
#include "Logger.h"

using namespace ECS;

void PrintSystem::Init(ECS::Engine* owner)
{
	_type.insert(owner->GetComponentID<Health>());
	_owner = owner;
}

void PrintSystem::Run()
{
	std::forward_list<Table*> tables = _owner->GetTables(_type);

	for (Table* table : tables)
	{
		ComponentData* health =  table->GetComponentData(_owner->GetComponentID<Health>());

		// iterate over components
		Buffer* healthBuffer = static_cast<Buffer*>(health->data);
		size_t pos = 0;
		for (size_t i = 0; i < health->number; i++,	pos += health->datumSize)
		{
			Health* component = reinterpret_cast<Health*>(healthBuffer + pos);
			Logger::Log("{PrintSystem::Run} printing health: " + std::to_string(component->value));
		}
	}
}
