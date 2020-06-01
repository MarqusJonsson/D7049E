#pragma once
#include "EASTL/queue.h"
#include "EASTL/array.h"
#include "Entity.h"
#include <cassert>
// EntityManager is a list of functions for manipulating entities
class EntityManager
{
public:
	EntityManager() 
	{
		// Initialize the queue with all possible entity IDs
		for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) 
		{
			availableEntities.push(entity);
		}
	}

	Entity CreateEntity() 
	{
		assert(livingEntityCount < MAX_ENTITIES && "Too many entities exist");
		// Extract ID from the front of availableEntities
		Entity id = availableEntities.front();
		availableEntities.pop();
		++livingEntityCount;

		return id;
	}

	void DestroyEntity(Entity entity)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		// Invalidate the destroyed entity's signature
		signatures[entity].reset();

		// Put the destroyed ID at the back of the queue
		availableEntities.push(entity);
		--livingEntityCount;
	}

	void SetSignature(Entity entity, Signature signature)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		signatures[entity] = signature;
	}

	Signature GetSignature(Entity entity)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		return signatures[entity];
	}

private:
	// Queue of unused entity IDs
	eastl::queue<Entity> availableEntities{};

	// Array of signatures where the index corresponds to the entity ID
	eastl::array<Signature, MAX_ENTITIES> signatures{};

	// Total living entities - used to keep limits on how many exist
	uint32_t livingEntityCount{};
};
