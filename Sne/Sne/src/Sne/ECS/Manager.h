#pragma once
#include <EASTL/vector.h>
#include <EASTL\unique_ptr.h>
#include "Entity.h"
class Manager
{
private:
	eastl::vector<eastl::unique_ptr<Entity>> entities;
public:
	void update ()
	{
		for (auto& e : entities) e->update();
	}
	void draw()
	{
		for (auto& e : entities) e->draw();
	}

	void refresh()
	{
		entities.erase(eastl::remove_if(eastl::begin(entities), eastl::end(entities), 
			[](const eastl::unique_ptr<Entity> &mEntity)
			{
				return !mEntity->isActive();
			}), eastl::end(entities));
	}

	Entity& addEntity()
	{
		Entity* e = new Entity();
		eastl::unique_ptr<Entity> uPtr(e);
		entities.emplace_back(eastl::move(uPtr));
		return *e;
	}

};

