#pragma once
#include "EASTL/unique_ptr.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"
//ManagerManager manages managers
class ManagerManager
{
public:
	/*void Init()
	{	eastl::unique_ptr<EntityManager> entityManager = eastl::make_unique<EntityManager>();
		eastl::unique_ptr<ComponentManager> componentManager = eastl::make_unique<ComponentManager>();
		eastl::unique_ptr<SystemManager> systemManager = eastl::make_unique<SystemManager>();
	}*/

	// Entity methods
	Entity CreateEntity()
	{
		return entityManager.CreateEntity();
	}

	void DestroyEntity(Entity entity)
	{
		entityManager.DestroyEntity(entity);

		componentManager.EntityDestroyed(entity);

		systemManager.EntityDestroyed(entity);
	}

	// Component methods
	template<typename T>
	void RegisterComponent()
	{
		componentManager.RegisterComponent<T>();
	}

	template<typename T>
	void AddComponent(Entity entity, T component)
	{
		componentManager.AddComponent<T>(entity, component);

		Signature signature = entityManager.GetSignature(entity);
		signature.set(componentManager.GetComponentType<T>(), true);
		entityManager.SetSignature(entity, signature);

		systemManager.EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	void RemoveComponent(Entity entity)
	{
		componentManager.RemoveComponent<T>(entity);

		Signature signature = entityManager.GetSignature(entity);
		signature.set(componentManager.GetComponentType<T>(), false);
		entityManager.SetSignature(entity, signature);

		systemManager.EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	T& GetComponent(Entity entity)
	{
		return componentManager.GetComponent<T>(entity);
	}

	template<typename T>
	ComponentType GetComponentType()
	{
		return componentManager.GetComponentType<T>();
	}


	// System methods
	template<typename T>
	eastl::shared_ptr<T> RegisterSystem()
	{
		return systemManager.RegisterSystem<T>();
	}

	template<typename T>
	void SetSystemSignature(Signature signature)
	{
		systemManager.SetSignature<T>(signature);
	}


	EntityManager getEntityManager()
	{
		return entityManager;
	}

	ComponentManager getComponentManager()
	{
		return componentManager;
	}

	SystemManager getSystemManager()
	{
		return systemManager;
	}

private:
	EntityManager entityManager = EntityManager();
	ComponentManager componentManager = ComponentManager();
	SystemManager systemManager = SystemManager();
};
