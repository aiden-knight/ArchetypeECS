#pragma once
#include "Engine.h"
#include "Component.h"

#include <thread>

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
		uint8_t _threadNumber;
		void (*_lambda)(Component&...);

	public:
		System(Engine* engine, uint8_t threadNumber) :
			_type{ StaticID<Component>::GetID() ... }
		{
			_engine = engine;
			_lambda = nullptr;
			_threadNumber = threadNumber;
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
				std::tuple<Component*...> buffers = std::make_tuple(reinterpret_cast<Component*>(table->GetComponentData(StaticID<Component>::GetID())->data) ...);
				
				// get size of each thread section
				size_t section = compAmount / _threadNumber;
				
				// start a thread for each section
				size_t nextStart = 0;
				vector<std::thread> threads;
				for (uint8_t i = 0; i < _threadNumber; i++)
				{
					size_t nextEnd = nextStart + section;
					if (nextEnd > compAmount)
					{
						nextEnd = compAmount;
					}

					threads.emplace_back(&System<Component...>::LoopCall, this, lambda, buffers, nextStart, nextEnd);

					nextStart = nextEnd;
				}
				
				// ensure all threads are finished before continuing
				for (std::thread& thread : threads)
				{
					thread.join();
				}
			}
		}

	private:
		void LoopCall(void (*lambda)(Component&...), std::tuple<Component*...> buffers, size_t start, size_t end)
		{
			([&]
				{
					std::get<Component*>(buffers) += start;
				} (), ...);

			for (size_t i = start; i < end; i++)
			{
				lambda(*std::get<Component*>(buffers)++ ...);
			}
		}
	};
}
