#pragma once
#include "Engine.h"
#include "Component.h"

namespace ECS
{
	class BaseSystem
	{
	public:
		virtual ~BaseSystem() = default;
		virtual void Run() = 0;
	};

	template<typename... Component>
	class System : public BaseSystem
	{
	private:
		Engine* _engine;
		const Type _type;

		void (*_lambda)(Component&...);

	public:
		System(Engine* engine) :
			_type{ StaticID<Component>::GetID() ... }
		{
			_engine = engine;
			_lambda = nullptr;
		}

		virtual void Run() final
		{
			Each(_lambda);
		}

		void Init(void (*lambda)(Component&...))
		{
			_lambda = lambda;
		}

		/// <summary>
		/// Runs function on each entity with type's components
		/// </summary>
		/// <param name="lambda">The function to run</param>
		void Each(void (*lambda)(Component&...))
		{
			// Get the tables that have the component's specified
			forward_list<Table*> tables = _engine->GetTables(_type);

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
