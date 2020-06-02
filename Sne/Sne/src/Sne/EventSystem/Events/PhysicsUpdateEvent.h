#pragma once
#include "../Event.h"
#include "../../Physics/Simulator.h"
struct PhysicsUpdateEvent : public Event
{
	Physics::Simulator* simulator;
	double timeStep;
	int maxSubSteps;

	PhysicsUpdateEvent(Physics::Simulator* simulator, double const& timeStep, int const& maxSubSteps = 1)
	{
		this->simulator = simulator;
		this->timeStep = timeStep;
		this->maxSubSteps = maxSubSteps;
	}
};