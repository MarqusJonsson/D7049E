#pragma once
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