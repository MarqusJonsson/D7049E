local BUILD_DIR = path.join("build", _ACTION)
if _OPTIONS["cc"] ~= nil then
	BUILD_DIR = BUILD_DIR .. "_" .. _OPTIONS["cc"]
end
local BGFX_DIR = "extlibs/bgfx"
local BIMG_DIR = "extlibs/bimg"
local BX_DIR = "extlibs/bx"
local GLFW_DIR = "extlibs/glfw"
local SOLOUD_DIR = "extlibs/soloud"
local BULLET_DIR = "extlibs/bullet3"
local EASTL_DIR = "extlibs/EASTL"
local EABase_DIR = "extlibs/EASTL/test/packages/EABase"
local SNE_DIR = "Sne"
local SANDBOX_DIR = "Sandbox"
local NOMADTASKS_DIR = "extlibs/NomadTasks"
local OPTICK_DIR = "extlibs/NomadTasks/code/vendor/optick"
local GLM_DIR = "extlibs/glm"

workspace "Sne"
	location (BUILD_DIR)
	configurations { "Release", "Debug" }
	platforms "x86_64"
	cppdialect "c++17"
	startproject "Sandbox"
	flags "MultiProcessorCompile"
	vectorextensions "SSE4.1"
	filter "configurations:Release"
		defines "NDEBUG"
		optimize "Full"
	filter "configurations:Debug*"
		defines "_DEBUG"
		optimize "Debug"
		symbols "On"
	filter "platforms:x86_64"
		architecture "x86_64"

function setBxCompat()
	filter "action:vs*"
		includedirs { path.join(BX_DIR, "include/compat/msvc") }
	filter { "system:windows", "action:gmake" }
		includedirs { path.join(BX_DIR, "include/compat/mingw") }
end

group ""	

project "Sne"
	location (SNE_DIR)
	targetdir (BUILD_DIR .. "/bin/" .. "%{cfg.architecture}/%{cfg.buildcfg}")
	objdir (BUILD_DIR .. "/obj/" .. "%{cfg.architecture}/%{cfg.buildcfg}")
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	defines 
	{
		"_CRT_SECURE_NO_WARNINGS",
		--"GLFW_INCLUDE_NONE",
		"SNE_BUILD_DLL",
		"WIN32_LEAN_AND_MEAN",
		"PUGIXML_WCHAR_MODE",
		"FMT_HEADER_ONLY"
	}
	
	includedirs
	{
		path.join(BGFX_DIR, "include"),
		path.join(BX_DIR, "include"),
		path.join(GLFW_DIR, "include"),
		path.join(SOLOUD_DIR, "include"),
		path.join(BULLET_DIR, "src"),
		path.join(EASTL_DIR, "include"),
		path.join(EASTL_DIR, "test/packages/EABase/include/Common"),
		path.join(NOMADTASKS_DIR, "code/fiber/include"),
		path.join(OPTICK_DIR, "src"),
		path.join(GLM_DIR, "glm"),
	}
	links {
		"bgfx",
		"bimg",
		"bx",
		"glfw",
		"soloud",
		"BulletCollision",
		"BulletDynamics",
		"BulletInverseDynamics",
		"BulletSoftBody",
		"LinearMath",
		"EASTL",
		"nomad-fiber",
	}
	
	--filter "configurations:Debug or Release"
	--	defines { "USE_OPTICK=1" }
	--	links { "optick" }
	--filter "configurations:Shipping"
	--	defines { "USE_OPTICK=0", "SHIPPING" }
	
	filter {}
	
	filter "system:windows"
		links { "opengl32", "gdi32", "kernel32", "psapi" }
	setBxCompat()

project "Sandbox"
	location(SANDBOX_DIR)
	targetdir (BUILD_DIR .. "/bin/" .. "%{cfg.architecture}/%{cfg.buildcfg}")
	objdir (BUILD_DIR .. "/obj/" .. "%{cfg.architecture}/%{cfg.buildcfg}")
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	exceptionhandling "Off"
	rtti "Off"
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	includedirs
	{
		"Sne/src",
		"Sne/extlibs"
	}
	links { "Sne" }

group "dependencies/bgfx"

project "bgfx"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	exceptionhandling "Off"
	rtti "Off"
	defines "__STDC_FORMAT_MACROS"
	files
	{
		path.join(BGFX_DIR, "include/bgfx/**.h"),
		path.join(BGFX_DIR, "src/*.cpp"),
		path.join(BGFX_DIR, "src/*.h"),
	}
	excludes
	{
		path.join(BGFX_DIR, "src/amalgamated.cpp"),
	}
	includedirs
	{
		path.join(BX_DIR, "include"),
		path.join(BIMG_DIR, "include"),
		path.join(BGFX_DIR, "include"),
		path.join(BGFX_DIR, "3rdparty"),
		path.join(BGFX_DIR, "3rdparty/dxsdk/include"),
		path.join(BGFX_DIR, "3rdparty/khronos")
	}
	filter "configurations:Debug"
		defines "BGFX_CONFIG_DEBUG=1"
	filter "action:vs*"
		defines "_CRT_SECURE_NO_WARNINGS"
		excludes
		{
			path.join(BGFX_DIR, "src/glcontext_glx.cpp"),
			path.join(BGFX_DIR, "src/glcontext_egl.cpp")
		}
	setBxCompat()

project "bimg"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	exceptionhandling "Off"
	rtti "Off"
	files
	{
		path.join(BIMG_DIR, "include/bimg/*.h"),
		path.join(BIMG_DIR, "src/image.cpp"),
		path.join(BIMG_DIR, "src/image_gnf.cpp"),
		path.join(BIMG_DIR, "src/*.h"),
		path.join(BIMG_DIR, "3rdparty/astc-codec/src/decoder/*.cc")
	}
	includedirs
	{
		path.join(BX_DIR, "include"),
		path.join(BIMG_DIR, "include"),
		path.join(BIMG_DIR, "3rdparty/astc-codec"),
		path.join(BIMG_DIR, "3rdparty/astc-codec/include"),
	}
	setBxCompat()

project "bx"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	exceptionhandling "Off"
	rtti "Off"
	defines "__STDC_FORMAT_MACROS"
	files
	{
		path.join(BX_DIR, "include/bx/*.h"),
		path.join(BX_DIR, "include/bx/inline/*.inl"),
		path.join(BX_DIR, "src/*.cpp")
	}
	excludes
	{
		path.join(BX_DIR, "src/amalgamated.cpp"),
		path.join(BX_DIR, "src/crtnone.cpp")
	}
	includedirs
	{
		path.join(BX_DIR, "3rdparty"),
		path.join(BX_DIR, "include")
	}
	filter "action:vs*"
		defines "_CRT_SECURE_NO_WARNINGS"
	setBxCompat()


group "dependencies/bullet"		
	include ("extlibs/bullet3/src/BulletInverseDynamics")
 	include "extlibs/bullet3/src/BulletSoftBody"
	include "extlibs/bullet3/src/BulletDynamics"
	include "extlibs/bullet3/src/BulletCollision"
	include "extlibs/bullet3/src/LinearMath"

group "dependencies/NomadTasks"
	include "extlibs/NomadTasks/code/fiber"
	--include "code/threadjob"
	--include "test/fiber-main-test"
	--include "test/threadjob-main-test"

group "dependencies/optick"
	include "extlibs/NomadTasks/code/vendor/optick.lua"

group "dependencies"

project "EAstl"		
	kind "StaticLib"
	language "C"
	
	files
	{
		path.join(EASTL_DIR, "include/EASTL/**.h"),
		path.join(EASTL_DIR, "test/packages/EABase/**.h"),
		path.join(EASTL_DIR, "source/*.cpp"),
	}
	includedirs { path.join(EASTL_DIR, "include"), path.join(EASTL_DIR, "test/packages/EABase/include/Common") }
		
		
project "glfw"
	kind "StaticLib"
	language "C"
	files
	{
		path.join(GLFW_DIR, "include/GLFW/*.h"),
		path.join(GLFW_DIR, "src/context.c"),
		path.join(GLFW_DIR, "src/egl_context.*"),
		path.join(GLFW_DIR, "src/init.c"),
		path.join(GLFW_DIR, "src/input.c"),
		path.join(GLFW_DIR, "src/internal.h"),
		path.join(GLFW_DIR, "src/monitor.c"),
		path.join(GLFW_DIR, "src/osmesa_context.*"),
		path.join(GLFW_DIR, "src/vulkan.c"),
		path.join(GLFW_DIR, "src/window.c"),
	}
	includedirs { path.join(GLFW_DIR, "include") }
	filter "system:windows"
		defines "_GLFW_WIN32"
		files
		{
			path.join(GLFW_DIR, "src/win32_*.*"),
			path.join(GLFW_DIR, "src/wgl_context.*")
		}

	filter "action:vs*"
		defines "_CRT_SECURE_NO_WARNINGS"
		
project "soloud"
	kind "StaticLib"
	language "C++"
	files
	{
		path.join(SOLOUD_DIR, "src/audiosource/**.c*"),
		path.join(SOLOUD_DIR, "src/filter/**.c*"),
		path.join(SOLOUD_DIR, "src/core/**.c*"),
		path.join(SOLOUD_DIR, "src/backend/null/**.c*"),
		path.join(SOLOUD_DIR, "src/backend/winmm/**.c*")
	}
	
	includedirs
	{
		path.join(SOLOUD_DIR, "include"),
		path.join(SOLOUD_DIR, "src/**")
	}
	
	defines { "WITH_NULL", "WITH_WINMM", "_CRT_SECURE_NO_WARNINGS" }
