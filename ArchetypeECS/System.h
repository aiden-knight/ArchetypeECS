#pragma once
#include "ComponentID.h"

namespace ECS
{
	class Engine;

	class System
	{
	public:
		virtual void Init(Engine* owner) = 0;
		virtual ~System() = default;
		virtual void Run() = 0;

	protected:
		Type _type;
		Engine* _owner;
	};
}