#pragma once
#include "game.h"
#include "EASTL/vector.h"
#include <string>

static float RandomFloat01() { return (float)rand() / (float)RAND_MAX; } // Random functions for initializing velocities in moveComponent
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

struct HealthComponent
{
	int health;
	int maxHealth;

	void Initialize(int health, int maxHealth) 
	{
		this->health = health;
		this->maxHealth = maxHealth;
	}
};

struct ManaComponent
{
	int mana;
	int maxMana;

	void Initialize(int mana , int maxMana)
	{
		this->mana = mana;
		this->maxMana = maxMana;
	}
};

struct ArmorComponent
{
	int armor;
	int maxArmor;

	void Initialize(int armor, int maxArmor)
	{
		this->armor = armor;
		this->maxArmor = maxArmor;
	}
};

// Entity
//typedef size_t EntityID;
struct EntityManager
{
	// names of each object
	eastl::vector<std::string> c_Names;
	// Arrays of data
	eastl::vector<PositionComponent> c_Positions;
	eastl::vector<MoveComponent> c_Moves;
	eastl::vector<HealthComponent> c_Healths;
	eastl::vector<ManaComponent> c_Manas;
	eastl::vector<ArmorComponent> c_Armors;



	// bit flag to indicate whether the object "has a component type enabled"
	//eastl::vector<int> m_Flags;


	void reserve(size_t n)
	{
		c_Names.reserve(n);
		c_Positions.reserve(n);
		c_Moves.reserve(n);
		c_Healths.reserve(n);
		c_Manas.reserve(n);
		c_Armors.reserve(n);
	}

	size_t AddEntity(const std::string&& name)
	{
		size_t id = c_Names.size();
		c_Names.emplace_back(name);
		c_Positions.emplace_back(PositionComponent());
		c_Moves.emplace_back(MoveComponent());
		c_Healths.emplace_back(HealthComponent());
		c_Manas.emplace_back(ManaComponent());
		c_Armors.emplace_back(ArmorComponent());
		return id;
	}
};

// Scene of entities
static EntityManager s_Objects;

// System
struct MoveSystem
{
	//EntityID boundsID;
	eastl::vector<size_t> entities;

	void AddEntity(size_t id)
	{
		entities.emplace_back(id);
	}

	/*void SetBounds(EntityID id)
	{
		boundsID = id;
	}*/

	void Update(double time, float deltaTime)
	{
		for (unsigned int i = 0; i < entities.size(); i++)
		{
			PositionComponent& pos = s_Objects.c_Positions[i];
			MoveComponent& move = s_Objects.c_Moves[i];

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

struct HealthSystem
{
	eastl::vector<size_t> entities;

	void AddEntity(size_t id)
	{
		entities.emplace_back(id);
	}

	void Update(int healthChange)// maxHealth change here aswell or different system to make it more efficient?
	{
		for (unsigned int i = 0; i < entities.size(); i++)
		{
			HealthComponent& healthComponent = s_Objects.c_Healths[i];
			if(healthComponent.health + healthChange < 0)
			{
				healthComponent.health = 0;
			}

			else if(healthComponent.health + healthChange > healthComponent.maxHealth)
			{
				healthComponent.health = healthComponent.maxHealth;
			}

			else
			{
				healthComponent.health += healthChange;
			}
		}
	} 



};

static HealthSystem s_HealthSystem;

struct ManaSystem
{
	eastl::vector<size_t> entities;

	void AddEntity(size_t id)
	{
		entities.emplace_back(id);
	}

	void Update(int manaChange)// maxMana change here aswell or different system to make it more efficient?
	{
		for (unsigned int i = 0; i < entities.size(); i++)
		{
			ManaComponent& manaComponent = s_Objects.c_Manas[i];
			if (manaComponent.mana + manaChange < 0)
			{
				manaComponent.mana = 0;
			}

			else if (manaComponent.mana + manaChange > manaComponent.maxMana)
			{
				manaComponent.mana= manaComponent.maxMana;
			}

			else
			{
				manaComponent.mana+= manaChange;
			}
		}
	}

};

static ManaSystem s_ManaSystem;

struct ArmorSystem
{
	eastl::vector<size_t> entities;

	void AddEntity(size_t id)
	{
		entities.emplace_back(id);
	}

	void Update(int armorChange)// maxMana change here aswell or different system to make it more efficient?
	{
		for (unsigned int i = 0; i < entities.size(); i++)
		{
			ArmorComponent& armorComponent = s_Objects.c_Armors[i];
			if (armorComponent.armor + armorChange < 0)
			{
				armorComponent.armor = 0;
			}

			else if (armorComponent.armor + armorChange > armorComponent.maxArmor)
			{
				armorComponent.armor = armorComponent.maxArmor;
			}

			else
			{
				armorComponent.armor += armorChange;
			}
		}
	}
};

static ArmorSystem s_ArmorSystem;



// DamageSystem unused atm not sure if we want damage calculations separate from HealthSystem etc.
struct DamageSystem
{
	eastl::vector<size_t> entities;

	// returns an array containing arr[0]=newHealth and arr[1]=newArmor to be used in HealthSystem and ArmorSystem respectively
	int* calcDamage(int damage, int health, int armor)
	{
		static int arr[2];

		if(armor-damage < 0 )
		{
			arr[1] = 0;
			if (health + (armor-damage) < 0) {
				arr[0] = 0;
			}
			else
			{
				arr[0] = health + (armor - damage);
			}
		}

		else
		{
			arr[0] = health;
			arr[1] = armor - damage;
		}

		return arr;

	}


};

static DamageSystem s_DamageSystem;

// Game related
const int ObjectCount = 1;



void game_initialize(void)
{
	s_Objects.reserve(1 + ObjectCount);

	// create objects
	for (unsigned int i = 0; i < ObjectCount; i++)
	{
		size_t player = s_Objects.AddEntity("player");
		s_Objects.c_Positions[player].x = 0;
		s_Objects.c_Positions[player].y = 0;
		s_Objects.c_Positions[player].z = 0;

		s_Objects.c_Healths[player].Initialize(1, 20);
		s_Objects.c_Manas[player].Initialize(1, 2000);
		s_Objects.c_Moves[player].Initialize(0.5f, 0.7f);
		s_Objects.c_Armors[player].Initialize(2, 1000);

		s_MoveSystem.AddEntity(player);
		s_HealthSystem.AddEntity(player);
		s_ManaSystem.AddEntity(player);
		s_ArmorSystem.AddEntity(player);
	}
}

void game_update(double time, float deltaTime)
{
	//Update systems
	s_MoveSystem.Update(time, deltaTime);
	s_HealthSystem.Update(1);
	s_ManaSystem.Update(1);
	s_ArmorSystem.Update(1);
	//printf("%f \n", s_Objects.c_Positions[0].x);
	printf("%i \n", s_Objects.c_Manas[0].mana);

}

void game_destroy(void)
{
}
