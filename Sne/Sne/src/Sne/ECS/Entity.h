#pragma once
#include <EASTL\bitset.h>

using Entity = size_t;

const Entity MAX_ENTITIES = 5000;

using ComponentType = size_t;

const ComponentType MAX_COMPONENTS = 32;

// Signature is used to indicate what components an entity "has"
using Signature = eastl::bitset<MAX_COMPONENTS>;