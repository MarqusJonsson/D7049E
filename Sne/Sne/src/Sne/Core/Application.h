#pragma once
#include "Core.h"
namespace Sne {

	class SNE_API Application
	{
	public:
		Application();
		virtual ~Application();
		
		void Run();
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}