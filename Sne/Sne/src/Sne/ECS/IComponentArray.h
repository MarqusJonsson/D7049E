#pragma once
// The one instance of virtual inheritance in the entire implementation.
// An interface is needed so that the ComponentManager
// can tell a generic ComponentArray that an entity has been destroyed
// and that it needs to update its array mappings.
#include "Entity.h"
#include "EASTL/unordered_map.h"
#include "EASTL/array.h"
#include "cassert"

class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity entity) = 0;
};


