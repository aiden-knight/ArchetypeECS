#pragma once
#include "Engine.h"
#include "Component.h"

namespace ECS
{
	template<typename... Component>
	class TempSystem
	{
	public:
		Engine* engine;

		void Each(void (*lambda)(Component&...))
		{
			Type type;
			((type.insert(StaticID<Component>::GetID())), ...);
			forward_list<Table*> tables = engine->GetTables(type);

			constexpr size_t num = sizeof...(Component);
			std::vector<ComponentData*> compData;
			compData.reserve(num);
			for (Table* table : tables)
			{
				ComponentData* temp = table->GetComponentData(*type.begin());

				size_t compAmount = temp->number;
				auto buffers = std::make_tuple<Component*...>(
					(reinterpret_cast<Component*>(table->GetComponentData(StaticID<Component>::GetID())->data), ...)
				);
				for (size_t i = 0; i < compAmount; i++)
				{
					lambda(*std::get<Component*>(buffers)...);
					(std::get<Component*>(buffers)++, ...);
				}
			}
		}
	};
}
