#pragma once
#include <set>
#include <stdint.h>
#include <limits>

namespace ECS
{
	typedef uint8_t ComponentID;

	namespace
	{
		ComponentID NextID = 0;
	}

	// the types of components a table or entity has
	typedef std::set<ComponentID> Type;


	template <typename Component>
	class StaticID
	{
	public:
		static constexpr ComponentID null = std::numeric_limits<ComponentID>::max();

		static ComponentID Init()
		{
			if (_typeID == null)
			{
				_typeID = NextID++;
			}
			return _typeID;
		}

		static ComponentID GetID() { return _typeID; }
	private:
		static ComponentID _typeID;
	};

	template <typename Component>
	ComponentID StaticID<Component>::_typeID = StaticID<Component>::null;
}