Whilst exploring methods for creating cache friendly code, I looked into ECS architecture. After having implemented a simple sparse ECS I looked into creating an archetype (a.k.a. table-based) ECS for its benefits in iterating over entities that have the same components. For example, if an entity only has position, it won't lead to cache misses when trying to iterate over entities with position and velocity.

The main features of the Archetype ECS were:

Templated variadic system that takes user provided functions (lambdas) to run on entities with relevant components.

Templated component system with static ID generation.

Custom memory management using type erasure to store components in tables.

Table edges for quick add and remove component operations.

Multi-threading to speed up system running.

Runs at over 60fps with over 5 million entities on tested PCs.

Alongside this the project currently renders an SDL2 window based off of loaded JSON values of width and height. This window surface is blitted to with pixels that are stored as components in the ECS and the SDL2 types are wrapped in smart pointers for memory safety and unique ownership guarantee.
