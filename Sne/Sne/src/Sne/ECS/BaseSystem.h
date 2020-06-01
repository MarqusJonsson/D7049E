#pragma once
#include "Entity.h"
#include "EASTL/set.h"
//#include "EASTL/unique_ptr.h"
//#include "ManagerManager.h"

// Base class for all systems, a system holds a set of entities which it will interact it.
class BaseSystem
{
public:
	eastl::set<Entity> entities;

};
