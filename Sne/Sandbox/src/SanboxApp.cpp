#include <Sne.h>

// The purpose of the "Sandbox" project was for it to be an example application
// using the "Sne" engine, currently the demo example shown in the D7049E final
// presentation is only within the engine itself.
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