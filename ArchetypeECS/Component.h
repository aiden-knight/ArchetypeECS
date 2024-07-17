#pragma once
#include <vector>

namespace ECS
{
	struct ComponentData
	{
		void* data;
		size_t datumSize;
		size_t number;

		ComponentData(size_t sizeOfComponent)
		{
			datumSize = sizeOfComponent;
			data = nullptr;
			number = 0;
		}

		ComponentData()
		{
			datumSize = 0;
			data = nullptr;
			number = 0;
		}
	};
}