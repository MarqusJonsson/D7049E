#pragma once
#include "Entity.h"
#include "EASTL/set.h"
//#include "EASTL/unique_ptr.h"
//#include "ManagerManager.h"

class BaseSystem
{
public:
	eastl::set<Entity> entities;
};
