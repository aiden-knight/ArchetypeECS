#pragma once
#include "Engine.h"
#include "Component.h"

namespace ECS
{
	template<typename... Component>
	class VariadicSystem
	{
		Engine* _engine;

	public:
		VariadicSystem(Engine* engine)
		{
			_engine = engine;
		}

		/// <summary>
		/// Runs function on each entity with type's components
		/// </summary>
		/// <param name="lambda">The function to run</param>
		void Each(void (*lambda)(Component&...))
		{
			// Get the tables that have the component's specified
			Type type {StaticID<Component>::GetID() ...};
			forward_list<Table*> tables = _engine->GetTables(type);

			// Iterate over the tables
			for (Table* table : tables)
			{
				// get number of components in table
				size_t compAmount = table->GetComponentAmount();

				// get buffers for all components
				auto buffers = std::make_tuple(reinterpret_cast<Component*>(table->GetComponentData(StaticID<Component>::GetID())->data) ...);
				
				// call function with all components in table
				for (size_t i = 0; i < compAmount; i++)
				{
					lambda(*std::get<Component*>(buffers)++ ...); // dereference component buffer then increment
				}
			}
		}
	};
}
