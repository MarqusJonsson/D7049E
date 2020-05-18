#pragma once
#include "EASTL/unordered_map.h"
#include "EASTL/shared_ptr.h"
#include "Entity.h"
#include "BaseSystem.h"
#include <cassert>

// Keeps track of registered systems and their signatures
class SystemManager
{
public:
	template<typename T>
	eastl::shared_ptr<T> RegisterSystem()
	{
		const char* typeName = typeid(T).name();

		assert(systems.find(typeName) == systems.end() && "Registering system more than once.");

		// Create a pointer to the system and return it so it can be used externally
		auto system = eastl::make_shared<T>();
		systems.insert({ typeName, system }); // sketchy function needs to be looked at 2020-05-08
		return system;
	}

	template<typename T>
	void SetSignature(Signature signature)
	{
		const char* typeName = typeid(T).name();

		assert(systems.find(typeName) != systems.end() && "System used before registered.");

		// Set the signature for this system
		signatures.insert({ typeName, signature });
	}

	void EntityDestroyed(Entity entity)
	{
		// Erase a destroyed entity from all system lists
		// Entities is a set so no check needed
		for (const eastl::pair<const char* const, eastl::shared_ptr <BaseSystem>>& pair : systems)
		{
			
			const eastl::shared_ptr <BaseSystem>& system = pair.second;

			system->entities.erase(entity);
		}
	}

	void EntitySignatureChanged(Entity entity, Signature entitySignature)
	{
		// Notify each system that an entity's signature changed
		for (const eastl::pair<const char* const, eastl::shared_ptr <BaseSystem>>& pair : systems)
		{
			const char* type = pair.first;
			const eastl::shared_ptr <BaseSystem>& system = pair.second;
			const Signature& systemSignature = signatures[type];

			// Entity signature matches system signature - insert into set
			if ((entitySignature & systemSignature) == systemSignature)
			{
				system->entities.insert(entity);
			}
			// Entity signature does not match system signature - erase from set
			else
			{
				system->entities.erase(entity);
			}
		}
	}

private:
	// Map from system type string pointer to a signature
	eastl::unordered_map<const char*, Signature> signatures{};

	// Map from system type string pointer to a system pointer
	eastl::unordered_map<const char*, eastl::shared_ptr<BaseSystem>> systems{};
};


