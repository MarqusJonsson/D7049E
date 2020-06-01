#pragma once
#include "../../ECS/BaseSystem.h"
#include "../ManagerManager.h"
class ExperienceSystem : public BaseSystem
{
private:
	ManagerManager managerManager;

public:
	ExperienceSystem(ManagerManager managerManager) 
	{
		this->managerManager = managerManager;
	}

	
};