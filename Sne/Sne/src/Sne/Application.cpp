#include "Application.h"
#include <GLFW/glfw3.h>
#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include <GLFW/glfw3native.h>
#include <gl/GL.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <stdio.h>

namespace Sne {

	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	static bool s_showStats = false;

	static const uint8_t s_logo[4000] =
	{
		0xdc, 0x03, 0xdc, 0x03, 0xdc, 0x03, 0xdc, 0x03, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, // ........ . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0xdc, 0x08, //  . . . . . . ...
		0xdc, 0x03, 0xdc, 0x07, 0xdc, 0x07, 0xdc, 0x08, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, // ........ . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0xde, 0x03, 0xb0, 0x3b, 0xb1, 0x3b, 0xb2, 0x3b, 0xdb, 0x3b, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, // ...;.;.;.; . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0xdc, 0x03, 0xb1, 0x3b, 0xb2, 0x3b, //  . . . . ....;.;
		0xdb, 0x3b, 0xdf, 0x03, 0xdf, 0x3b, 0xb2, 0x3f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, // .;...;.? . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0xb1, 0x3b, 0xb1, 0x3b, 0xb2, 0x3b, 0xb2, 0x3f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  ..;.;.;.? . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0xb1, 0x3b, 0xb1, 0x3b, 0xb2, 0x3b, //  . . . . ..;.;.;
		0xb2, 0x3f, 0x20, 0x0f, 0x20, 0x0f, 0xdf, 0x03, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, // .? . ... . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0xb1, 0x3b, 0xb1, 0x3b, 0xb1, 0x3b, 0xb1, 0x3f, 0xdc, 0x0b, 0xdc, 0x03, 0xdc, 0x03, //  ..;.;.;.?......
		0xdc, 0x03, 0xdc, 0x03, 0x20, 0x0f, 0x20, 0x0f, 0xdc, 0x08, 0xdc, 0x03, 0xdc, 0x03, 0xdc, 0x03, // .... . .........
		0xdc, 0x03, 0xdc, 0x03, 0xdc, 0x03, 0xdc, 0x08, 0x20, 0x0f, 0xb1, 0x3b, 0xb1, 0x3b, 0xb1, 0x3b, // ........ ..;.;.;
		0xb1, 0x3f, 0xb1, 0x3f, 0xb2, 0x0b, 0x20, 0x0f, 0x20, 0x0f, 0xdc, 0x03, 0xdc, 0x03, 0xdc, 0x03, // .?.?.. . .......
		0x20, 0x0f, 0x20, 0x0f, 0xdc, 0x03, 0xdc, 0x03, 0xdc, 0x03, 0x20, 0x0f, 0x20, 0x01, 0x20, 0x0f, //  . ....... . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0xb2, 0x3b, 0xb1, 0x3b, 0xb0, 0x3b, 0xb0, 0x3f, 0x20, 0x0f, 0xde, 0x03, 0xb0, 0x3f, //  ..;.;.;.? ....?
		0xb1, 0x3f, 0xb2, 0x3f, 0xdd, 0x03, 0xde, 0x03, 0xdb, 0x03, 0xdb, 0x03, 0xb2, 0x3f, 0x20, 0x0f, // .?.?.........? .
		0x20, 0x0f, 0xb0, 0x3f, 0xb1, 0x3f, 0xb2, 0x3f, 0xde, 0x38, 0xb2, 0x3b, 0xb1, 0x3b, 0xb0, 0x3b, //  ..?.?.?.8.;.;.;
		0xb0, 0x3f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0xb0, 0x3b, 0xb1, 0x3b, 0xb2, 0x3b, 0xb2, 0x3f, // .? . . ..;.;.;.?
		0xdd, 0x03, 0xde, 0x03, 0xb0, 0x3f, 0xb1, 0x3f, 0xb2, 0x3f, 0xdd, 0x03, 0x20, 0x01, 0x20, 0x0f, // .....?.?.?.. . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0xb2, 0x3b, 0xb1, 0x3b, 0xb0, 0x3b, 0xb0, 0x3f, 0x20, 0x0f, 0x20, 0x0f, 0xdb, 0x03, //  ..;.;.;.? . ...
		0xb0, 0x3f, 0xb1, 0x3f, 0xdd, 0x03, 0xb1, 0x3b, 0xb0, 0x3b, 0xdb, 0x03, 0xb1, 0x3f, 0x20, 0x0f, // .?.?...;.;...? .
		0x20, 0x0f, 0x20, 0x3f, 0xb0, 0x3f, 0xb1, 0x3f, 0xb0, 0x3b, 0xb2, 0x3b, 0xb1, 0x3b, 0xb0, 0x3b, //  . ?.?.?.;.;.;.;
		0xb0, 0x3f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0xdc, 0x08, 0xdc, 0x3b, 0xb1, 0x3b, 0xb1, 0x3f, // .? . . ....;.;.?
		0xb1, 0x3b, 0xb0, 0x3b, 0xb2, 0x3b, 0xb0, 0x3f, 0xdc, 0x03, 0x20, 0x0f, 0x20, 0x01, 0x20, 0x0f, // .;.;.;.?.. . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0xb2, 0x3b, 0xb1, 0x3b, 0xb0, 0x3b, 0xb0, 0x3f, 0xdc, 0x0b, 0xdc, 0x07, 0xdb, 0x03, //  ..;.;.;.?......
		0xdb, 0x03, 0xdc, 0x38, 0x20, 0x0f, 0xdf, 0x03, 0xb1, 0x3b, 0xb0, 0x3b, 0xb0, 0x3f, 0xdc, 0x03, // ...8 ....;.;.?..
		0xdc, 0x07, 0xb0, 0x3f, 0xb1, 0x3f, 0xb2, 0x3f, 0xdd, 0x3b, 0xb2, 0x3b, 0xb1, 0x3b, 0xdc, 0x78, // ...?.?.?.;.;.;.x
		0xdf, 0x08, 0x20, 0x0f, 0x20, 0x0f, 0xde, 0x08, 0xb2, 0x3b, 0xb1, 0x3b, 0xb0, 0x3b, 0xb0, 0x3f, // .. . ....;.;.;.?
		0x20, 0x0f, 0xdf, 0x03, 0xb1, 0x3b, 0xb2, 0x3b, 0xdb, 0x03, 0xdd, 0x03, 0x20, 0x01, 0x20, 0x0f, //  ....;.;.... . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0xdc, 0x08, 0xdc, 0x08, 0xdc, 0x08, 0x20, 0x0f, //  . . . ....... .
		0x20, 0x0f, 0xb0, 0x3f, 0xb0, 0x3f, 0xb1, 0x3f, 0xdd, 0x3b, 0xdb, 0x0b, 0xdf, 0x03, 0x20, 0x0f, //  ..?.?.?.;.... .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0xdf, 0x08, 0xdf, 0x03, 0xdf, 0x03, 0xdf, 0x08, //  . . . .........
		0x20, 0x0f, 0x20, 0x0f, 0xdf, 0x08, 0xdf, 0x03, 0xdf, 0x03, 0x20, 0x0f, 0x20, 0x01, 0x20, 0x0f, //  . ....... . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0xdb, 0x08, 0xb2, 0x38, 0xb1, 0x38, 0xdc, 0x03, //  . . . ....8.8..
		0xdc, 0x07, 0xb0, 0x3b, 0xb1, 0x3b, 0xdf, 0x3b, 0xdf, 0x08, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, // ...;.;.;.. . . .
		0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, //  . . . . . . . .
		0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, //  . . . . . . . .
		0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, //  . . . . . . . .
		0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, //  . . . . . . . .
		0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, //  . . . . . . . .
		0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, //  . . . . . . . .
		0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, //  . . . . . . . .
		0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0b, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x2d, 0x08, 0x3d, 0x08, 0x20, 0x0a, 0x43, 0x0b, 0x72, 0x0b, 0x6f, 0x0b, 0x73, 0x0b, 0x73, 0x0b, // -.=. .C.r.o.s.s.
		0x2d, 0x0b, 0x70, 0x0b, 0x6c, 0x0b, 0x61, 0x0b, 0x74, 0x0b, 0x66, 0x0b, 0x6f, 0x0b, 0x72, 0x0b, // -.p.l.a.t.f.o.r.
		0x6d, 0x0b, 0x20, 0x0b, 0x72, 0x0b, 0x65, 0x0b, 0x6e, 0x0b, 0x64, 0x0b, 0x65, 0x0b, 0x72, 0x0b, // m. .r.e.n.d.e.r.
		0x69, 0x0b, 0x6e, 0x0b, 0x67, 0x0b, 0x20, 0x0b, 0x6c, 0x0b, 0x69, 0x0b, 0x62, 0x0b, 0x72, 0x0b, // i.n.g. .l.i.b.r.
		0x61, 0x0b, 0x72, 0x0b, 0x79, 0x0b, 0x20, 0x0f, 0x3d, 0x08, 0x2d, 0x08, 0x20, 0x01, 0x20, 0x0f, // a.r.y. .=.-. . .
		0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, //  . . . . . . . .
		0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, //  . . . . . . . .
		0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, //  . . . . . . . .
		0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, //  . . . . . . . .
		0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, 0x20, 0x0a, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
		0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, 0x20, 0x0f, //  . . . . . . . .
	};

	static void glfw_errorCallback(int error, const char* description)
	{
		fprintf(stderr, "GLFW error %d: %s\n", error, description);
	}

	static void glfw_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_F1 && action == GLFW_RELEASE)
			s_showStats = !s_showStats;
	}
	
	void Application::Run()
	{
		// Create a GLFW window without an OpenGL context.
		glfwSetErrorCallback(glfw_errorCallback);
		if (!glfwInit())
			return;
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		GLFWwindow* window = glfwCreateWindow(1024, 768, "helloworld", nullptr, nullptr);
		if (!window)
			return;
		glfwSetKeyCallback(window, glfw_keyCallback);
		// Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
		// Most graphics APIs must be used on the same thread that created the window.
		bgfx::renderFrame();
		// Initialize bgfx using the native window handle and window resolution.
		bgfx::Init init;
		init.platformData.nwh = glfwGetWin32Window(window);
		init.platformData.nwh = glfwGetWindow(window);
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		init.resolution.width = (uint32_t)width;
		init.resolution.height = (uint32_t)height;
		init.resolution.reset = BGFX_RESET_VSYNC;
		if (!bgfx::init(init))
			return;
		// Set view 0 to the same dimensions as the window and to clear the color buffer.
		const bgfx::ViewId kClearView = 0;
		bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR);
		bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
			// Handle window resize.
			int oldWidth = width, oldHeight = height;
			glfwGetWindowSize(window, &width, &height);
			if (width != oldWidth || height != oldHeight) {
				bgfx::reset((uint32_t)width, (uint32_t)height, BGFX_RESET_VSYNC);
				bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
			}
			// This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
			bgfx::touch(kClearView);
			// Use debug font to print information about this example.
			bgfx::dbgTextClear();
			bgfx::dbgTextImage(bx::max<uint16_t>(uint16_t(width / 2 / 8), 20) - 20, bx::max<uint16_t>(uint16_t(height / 2 / 16), 6) - 6, 40, 12, s_logo, 160);
			bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 to toggle stats.");
			bgfx::dbgTextPrintf(0, 1, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
			bgfx::dbgTextPrintf(80, 1, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
			bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");
			const bgfx::Stats* stats = bgfx::getStats();
			bgfx::dbgTextPrintf(0, 2, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.", stats->width, stats->height, stats->textWidth, stats->textHeight);
			// Enable stats or debug text.
			bgfx::setDebug(s_showStats ? BGFX_DEBUG_STATS : BGFX_DEBUG_TEXT);
			// Advance to next frame. Process submitted rendering primitives.
			bgfx::frame();
		}
		bgfx::shutdown();
		glfwTerminate();
        return;
	}
}