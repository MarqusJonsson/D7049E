#pragma once
#include "Entity.h"
#include "EASTL/set.h"
//#include "EASTL/unique_ptr.h"
//#include "ManagerManager.h"
// Base class for systems to derive from
class BaseSystem
{
public:
	eastl::set<Entity> entities;
};
