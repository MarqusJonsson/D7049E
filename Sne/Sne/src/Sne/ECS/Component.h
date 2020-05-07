#pragma once
#include <cstddef>
#include <EASTL\bitset.h>
#include <EASTL\array.h>
//#include "Entity.h"
//#include "../Core/EastlOverides.h"
using ComponentID = std::size_t;
class Entity; // Fix later


class Component
{
public:
	Entity* entity;
	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}

	virtual ~Component() {}


};

inline ComponentID getComponentID() {
	static ComponentID newID = 0;
	return newID++;
}

template <typename T> inline ComponentID 
getComponentTypeID() noexcept 
{
	static ComponentID typeID = getComponentID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;

using ComponentBitSet = eastl::bitset<maxComponents>;
using ComponentArray = eastl::array<Component*, maxComponents > ;