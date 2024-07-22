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

	/// <summary>
	/// Static helper class to generate unique IDs for components
	/// </summary>
	/// <typeparam name="T">The type that acts as a component</typeparam>
	template <typename Component>
	class StaticID
	{
	public:
		static constexpr ComponentID null = std::numeric_limits<ComponentID>::max();

		/// <summary>
		/// Generates a unique ID for component
		/// </summary>
		/// <returns>Generated ID</returns>
		static ComponentID Init()
		{
			if (_typeID == null)
			{
				_typeID = NextID++;
			}
			return _typeID;
		}

		/// <summary>
		/// Gets the ID of the component
		/// </summary>
		/// <returns>The components ID, or StaticID<Component>::null if it hasn't been initialised</returns>
		static ComponentID GetID() { return _typeID; }
	private:
		static ComponentID _typeID;
	};

	template <typename Component>
	ComponentID StaticID<Component>::_typeID = StaticID<Component>::null;
}