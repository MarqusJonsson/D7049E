/*#pragma once
//#include "Entity.h"
#include <cassert>
#include "EASTL/allocator.h"
#include "EASTL/string.h"
//#include "EASTL/memory.h"
//#include <string.h> // memcpy
class HealthComponentManager
{
    struct InstanceData
    {
        unsigned int n;          ///< Number of used instances.
        unsigned int allocated;  ///< Number of allocated instances.
        void* buffer;        ///< Buffer with instance data.

        Entity* entity;
        int* health;
        int* maxHealth;
    };

    InstanceData data;
    
    void allocate(unsigned int size) 
    {
        eastl::allocator allocator; // is it supposed to be a global allocator for all componentmanagers?????
        assert(size > data.n);
        InstanceData newData;
        const unsigned bytes = size * (sizeof(Entity) + sizeof(float) + 2 * sizeof(int));
        newData.buffer = allocator.allocate(bytes);
        newData.n = data.n;
        newData.allocated = size;

        newData.entity = (Entity*)(newData.buffer);
        newData.health = (int*)newData.entity + size;
        newData.maxHealth = newData.health + size;

        memcpy(newData.entity, data.entity, data.n * sizeof(Entity));
        memcpy(newData.health, data.health, data.n * sizeof(int));
        memcpy(newData.maxHealth, data.maxHealth, data.n * sizeof(int));

        allocator.deallocate(data.buffer, sizeof(data.buffer));
        data = newData;
    }	//memcpy(dest, first, (size_t)((uintptr_t)last - (uintptr_t)first)) + (last - first);

};*/