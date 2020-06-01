#pragma once
#include "Core.h"



namespace Sne 
{
	class SNE_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	private:
		int width = 800;
		int height = 600;
		void initWindow();
		void initBGFX();
		void mainLoop();
		void cleanup();
		void initExample();
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}