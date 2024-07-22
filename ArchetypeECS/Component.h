#pragma once
#include <vector>

namespace ECS
{
	struct ComponentData
	{
		static constexpr size_t AllocSize = 640000;

		void* data;
		size_t datumSize;
		size_t number;
		size_t allocated;

		ComponentData(size_t sizeOfComponent)
		{
			datumSize = sizeOfComponent;
			data = nullptr;
			number = 0;
			allocated = 0;
		}

		ComponentData()
		{
			datumSize = 0;
			data = nullptr;
			number = 0;
			allocated = 100;
		}
	};
}