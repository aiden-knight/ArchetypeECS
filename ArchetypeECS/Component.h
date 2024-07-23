#pragma once
#include <vector>

namespace ECS
{
	struct ComponentData
	{
		void* data;
		size_t datumSize;
		size_t number;
		size_t allocated;
		size_t allocSize;

		ComponentData(size_t sizeOfComponent, size_t allocSize)
		{
			datumSize = sizeOfComponent;
			data = nullptr;
			number = 0;
			allocated = 0;
			this->allocSize = allocSize;
		}

		ComponentData()
		{
			datumSize = 0;
			data = nullptr;
			number = 0;
			allocated = 0;
			allocSize = 100;
		}
	};

	struct ComponentInfo
	{
		size_t datumSize;
		size_t allocSize;

		ComponentInfo(size_t datumSize, size_t allocSize)
		{
			this->datumSize = datumSize;
			this->allocSize = allocSize;
		}

		ComponentInfo()
		{
			this->datumSize = 0;
			this->allocSize = 0;
		}
	};
}