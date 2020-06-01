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
		int width = 1280;
		int height = 720;
		void initWindow();
		void initBGFX();
		void initExample();
		void mainLoop();
		void cleanup();
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}