#include <Sne.h>

class Sandbox : public Sne::Application
{
public:
	Sandbox()
	{
	}

	~Sandbox()
	{
	}
};

Sne::Application* Sne::CreateApplication()
{
	return new Sandbox();
}