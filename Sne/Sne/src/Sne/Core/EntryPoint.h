#pragma once

#ifdef SNE_PLATFORM_WINDOWS

extern Sne::Application* Sne::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Sne::CreateApplication();
	app->Run();
	delete app;
}

#endif