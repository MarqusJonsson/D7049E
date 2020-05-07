#pragma once
#include "Component.h"
#include <EASTL\unique_ptr.h>
#include "EASTL/vector.h"
#include "Component.h"
//#include "../Core/EastlOverides.h"

//template<class T>
class Entity
{
private:
	bool active = true;
	int* EntityID;
	eastl::vector<eastl::unique_ptr<Component>> components;

	ComponentArray componentArray;
	ComponentBitSet componentBitSet;
public:
	void update()
	{
		for (auto& c : components) c->update();
		for (auto& c : components) c->draw();
	}
	void draw() {}
	bool isActive() { return active; }
	void destroy() { active = false; }
	template<typename T> bool hasComponent() const
	{
		return componentBitSet[getComponentID<T>];
	}

	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		T* c(new T(eastl::forward<TArgs>(mArgs)...));
		c->entity = this;
		eastl::unique_ptr<Component> uPtr{ c };
		components.emplace_back(eastl::move(uPtr));

		componentArray[getComponentTypeID<T>()] = c;
		componentBitSet[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}


	template<typename T> 
	T& getComponent() const
	{
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}
};

// constant initialization of entity type identifier
//template const EntityTypeId Entity::STATIC_ENTITY_TYPE_ID = util::Internal::FamilyTypeID::Get();