#pragma once

// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows x64/x86 */
	#ifdef _WIN64
		/* Windowx x64 */
		#define SNE_PLATFORM_WINDOWS
		#ifdef SNE_BUILD_DLL
			#define SNE_API __declspec(dllexport)
		#else
			#define SNE_API __declspec(dllimport)
		#endif
	#else
		#error Sne only supports Windows x64!
	#endif
#else
	#error Sne only support Windows x64!
#endif // End of platform detection