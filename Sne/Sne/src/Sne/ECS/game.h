#pragma once
#include "game.h"
#include "EASTL/vector.h"
#include <string>

static float RandomFloat01() { return (float)rand() / (float)RAND_MAX; }
static float RandomFloat(float from, float to) { return RandomFloat01() * (to - from) + from; }
// Components
struct PositionComponent
{
	float x, y, z;
};

struct MoveComponent
{
	float xVel, yVel, zVel;

	void Initialize(float minSpeed, float maxSpeed)
	{
		// random angle
		float angle = RandomFloat01() * 3.1415926f * 2;

		// random movement speed between given min & max
		float speed = RandomFloat(minSpeed, maxSpeed);

		// velocity x & y components
		xVel = cosf(angle) * speed;
		yVel = sinf(angle) * speed;
		zVel = sinf(angle) * speed;
	}
};

// Entity
typedef size_t EntityID;
struct Entities
{
	// names of each object
	eastl::vector<std::string> c_Names;
	// Arrays of data
	eastl::vector<PositionComponent> c_Positions;
	eastl::vector<MoveComponent> c_Moves;



	// bit flag to indicate whether the object "has it"
	eastl::vector<int> m_Flags;


	void reserve(size_t n)
	{
		c_Names.reserve(n);
		c_Positions.reserve(n);
		c_Moves.reserve(n);
	}

	EntityID AddEntity(const std::string&& name)
	{
		EntityID id = c_Names.size();
		c_Names.emplace_back(name);
		c_Positions.emplace_back(PositionComponent());
		c_Moves.emplace_back(MoveComponent());
		return id;
	}
};

// Scene of entities
static Entities s_Objects;

// System
struct MoveSystem
{
	//EntityID boundsID;
	eastl::vector<EntityID> entities;

	void AddObjectToSystem(EntityID id)
	{
		entities.emplace_back(id);
	}

	/*void SetBounds(EntityID id)
	{
		boundsID = id;
	}*/

	void UpdateSystem(double time, float deltaTime)
	{
		for (size_t io = 0, no = entities.size(); io != no; ++io)
		{
			PositionComponent& pos = s_Objects.c_Positions[io];
			MoveComponent& move = s_Objects.c_Moves[io];

			// update position based on movement velocity & delta time
			pos.x += move.xVel * deltaTime;
			pos.y += move.yVel * deltaTime;
			pos.z += move.zVel * deltaTime;

			/*// check against world bounds; put back onto bounds and mirror the velocity component to "bounce" back
			if (pos.x < bounds.xMin)
			{
				move.velx = -move.velx;
				pos.x = bounds.xMin;
			}
			if (pos.x > bounds.xMax)
			{
				move.velx = -move.velx;
				pos.x = bounds.xMax;
			}
			if (pos.y < bounds.yMin)
			{
				move.vely = -move.vely;
				pos.y = bounds.yMin;
			}
			if (pos.y > bounds.yMax)
			{
				move.vely = -move.vely;
				pos.y = bounds.yMax;
			}*/
		}
	}
};

static MoveSystem s_MoveSystem;


// Game related
const int ObjectCount = 1;



void game_initialize(void)
{
	s_Objects.reserve(1 + ObjectCount);

	// create objects
	for (int i = 0; i < ObjectCount; i++)
	{
		EntityID move = s_Objects.AddEntity("moveObject");
		s_Objects.c_Positions[move].x = 0;
		s_Objects.c_Positions[move].y = 0;
		s_Objects.c_Positions[move].z = 0;

		s_Objects.c_Moves[move].Initialize(0.5f, 0.7f);
		s_MoveSystem.AddObjectToSystem(move);
	}
}

void game_update(double time, float deltaTime)
{
	//Update systems
	s_MoveSystem.UpdateSystem(time, deltaTime);
	printf("%f \n", s_Objects.c_Positions[0].x);

}

void game_destroy(void)
{
}
