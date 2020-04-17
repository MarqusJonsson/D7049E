#pragma once

#ifdef SNE_PLATFORM_WINDOWS
	#ifdef SNE_BUILD_DLL
		#define SNE_API __declspec(dllexport)
	#else
		#define SNE_API __declspec(dllimport)
	#endif
#else
	#error Sne only support Windows!
#endif